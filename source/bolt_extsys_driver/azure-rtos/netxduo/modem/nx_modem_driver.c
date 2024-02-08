/* Copyright (C) 2022 Alif Semiconductor - All Rights Reserved.
 * Use, distribution and modification of this code is permitted under the
 * terms stated in the Alif Semiconductor Software License Agreement
 *
 * You should have received a copy of the Alif Semiconductor Software
 * License Agreement with this file. If not, please write to:
 * contact@alifsemi.com, or visit: https://alifsemi.com/license
 *
 */

/**************************************************************************//**
 * @file     nx_modem_driver.c
 * @author   Silesh C V
 * @email    silesh@alifsemi.com
 * @version  V1.0.0
 * @date     13-Dec-2021
 * @brief    NetXDuo network driver for the modem over IPC.
 * @bug      None.
 * @Note     None
 ******************************************************************************/

// Enable/Disable modem driver debug
#define NX_MODEM_DEBUG

#include "nx_api.h"
#include "mdm_data.h"
#include "nx_modem_driver_private.h"
#include "nx_modem_driver.h"
#include "nx_modem_user.h"

#define NX_LINK_MTU             1500U
#define MDM_BUF_LEN             NX_LINK_MTU
#define BUF_DESC_POOL_SIZE      (2 * BUF_DESC_COUNT * (sizeof(BUF_DESC) + sizeof(void *)))
#define RX_THREAD_STACKSIZE		512

#define MODEM_DATA_INIT_EVENT                   (1 << 0)
#define MODEM_DATA_NOTIFICATION_ENABLED_EVENT   (1 << 1)
#define MAX_PDP_CONTEXTS                        12

/* NetXDuo modem driver instance */
static NX_MODEM_DRIVER nx_modem_driver[NUM_DRIVER_INSTANCES];

/*
 * The context ids to be statically associated with each driver instance.
 * Provided by the user through nx_modem_user.h.
 */
static UCHAR nx_modem_driver_cids[MAX_PDP_CONTEXTS] = { CONTEXT_ID_DRIVER_INSTANCE_0,
                                                        CONTEXT_ID_DRIVER_INSTANCE_1,
                                                        CONTEXT_ID_DRIVER_INSTANCE_2,
                                                        CONTEXT_ID_DRIVER_INSTANCE_3,
                                                        CONTEXT_ID_DRIVER_INSTANCE_4,
                                                        CONTEXT_ID_DRIVER_INSTANCE_5,
                                                        CONTEXT_ID_DRIVER_INSTANCE_6,
                                                        CONTEXT_ID_DRIVER_INSTANCE_7,
                                                        CONTEXT_ID_DRIVER_INSTANCE_8,
                                                        CONTEXT_ID_DRIVER_INSTANCE_9,
                                                        CONTEXT_ID_DRIVER_INSTANCE_10,
                                                        CONTEXT_ID_DRIVER_INSTANCE_11 };

/* Control and status information structure for all the driver instances */
static NX_MODEM_DRIVER_CONTROL nx_modem_driver_control;

static UCHAR buf_desc_block_pool_memory[BUF_DESC_POOL_SIZE];
static ULONG rx_buf_queue_area[RX_BUF_COUNT * TX_4_ULONG];
static UCHAR rx_thread_stack[RX_THREAD_STACKSIZE];

static VOID nx_modem_driver_dequeue_tx_buf(UCHAR *buf);

/* Callback function for the IPC Data driver */
static VOID mdm_data_cb(MDM_EVENT event, uint8_t *buf, uint32_t arg1, uint32_t arg2)
{
RX_INFO mdm_rx_info;

    switch (event)
    {
    case MDM_INIT_EVENT:
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_ipc_stats.init_event++;
#endif
        tx_event_flags_set(&(nx_modem_driver_control.mdm_event_flags), MODEM_DATA_INIT_EVENT, TX_OR);
        break;
    case MDM_VERSION_EVENT:
        break;
    case MDM_RX_BUF_UNAVAILABLE_EVENT:
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_ipc_stats.rx_buf_unavailable++;
#endif
        break;
    case MDM_RX_BUF_COPIED_EVENT:
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_ipc_stats.rx_buf_copied++;
#endif
        mdm_rx_info.buf = buf;
        mdm_rx_info.len = arg1;
        mdm_rx_info.cid = (uint8_t) arg2;
        tx_queue_send(&(nx_modem_driver_control.rx_buf_queue), &mdm_rx_info, TX_NO_WAIT);
	    break;
    case MDM_RX_QUEUE_FULL_EVENT:
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_ipc_stats.rx_queue_full++;
#endif
        break;
    case MDM_TX_BUF_REL_EVENT:
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_ipc_stats.tx_buf_rel++;
#endif
        nx_modem_driver_dequeue_tx_buf(buf);
        break;
    case MDM_RX_BUF_REL_EVENT:
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_ipc_stats.rx_buf_rel++;
#endif
        break;
    case MDM_RESET_DONE_EVENT:
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_ipc_stats.reset_done++;
#endif
        break;
    case MDM_NOTIFY_ENABLED_EVENT:
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_ipc_stats.notify_enabled++;
#endif
        tx_event_flags_set(&(nx_modem_driver_control.mdm_event_flags), MODEM_DATA_NOTIFICATION_ENABLED_EVENT, TX_OR);
        break;
    case MDM_RX_BUF_LEN_ERROR_EVENT:
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_ipc_stats.rx_buf_len_error++;
#endif
        break;
    default:
       break;
    }
}

/* Wrapper routine to send Rx Buffers to modem over IPC data channel */
static UINT modem_send_rx_buf(const uint8_t *buf, uint16_t buf_len)
{
MDM_RET_STATUS mdm_status;
INT retry_count = 5000;

    do {
        mdm_status = MDM_DATA_SendRxBuf(buf, buf_len);
        if (mdm_status == MDM_SUCCESS)
        {
            break;
        }
        else if (mdm_status == MDM_BUSY)
        {
            retry_count--;
        }
        else
        {
#ifdef NX_MODEM_DEBUG
            nx_modem_driver_control.nx_modem_driver_rx_buf_send_errors++;
#endif
            return NX_NOT_SUCCESSFUL;
        }
    } while (retry_count);

    if (retry_count)
        return NX_SUCCESS;
    else
    {
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_rx_buf_send_errors++;
#endif
        return NX_NOT_SUCCESSFUL;
    }
}

/*
 * Search and dequeue a Rx NX_PACKET from the Rx buffer descriptor
 * list using the buffer address as the key.
 */
NX_PACKET *nx_modem_driver_dequeue_rx_buf(UCHAR *buf)
{
BUF_DESC *head = nx_modem_driver_control.nx_modem_rx_buf_list_head;
BUF_DESC *rx_buf_desc = NX_NULL;
NX_PACKET *ret = NX_NULL;
UINT status;

    if (head == NX_NULL)
    {
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_rx_buf_dequeue_errors++;
#endif
        return ret;
    }

    if (head -> buf == buf)
    {
        rx_buf_desc = head;

        /* Reset the head */
        nx_modem_driver_control.nx_modem_rx_buf_list_head = head->next;

        if (rx_buf_desc == nx_modem_driver_control.nx_modem_rx_buf_list_tail)
        {
            nx_modem_driver_control.nx_modem_rx_buf_list_tail = NX_NULL;
        }
    }

    else
    {
        while (head -> next != NX_NULL)
        {
            if (head -> next -> buf == buf)
            {
                rx_buf_desc = head->next;

                /* Take the descriptor out of the list */
                head -> next = rx_buf_desc -> next;

                /* Check whether we removed the tail, if yes, then reset tail */
                if (rx_buf_desc == nx_modem_driver_control.nx_modem_rx_buf_list_tail)
                {
                    nx_modem_driver_control.nx_modem_rx_buf_list_tail = head;
                }

                break;
            }

            head = head -> next;
        }
    }

    if (rx_buf_desc == NX_NULL)
    {
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_rx_buf_dequeue_errors++;
#endif
        return ret;
    }

    ret = rx_buf_desc -> nx_packet_ptr;

    /* Relese the buffer descriptor */
    status = tx_block_release(rx_buf_desc);

#ifdef NX_MODEM_DEBUG
    if (status)
    {
        nx_modem_driver_control.nx_modem_driver_desc_release_errors++;
    }
#endif

#ifdef NX_MODEM_DEBUG
    nx_modem_driver_control.nx_modem_driver_rx_buf_dequeued++;
#endif

    return ret;
}

/* Enqueue a Rx buffer descriptor to the rx buffer desc list. */
static UINT nx_modem_driver_enqueue_rx_buf(NX_PACKET *packet)
{
TX_INTERRUPT_SAVE_AREA
UINT status;
BUF_DESC *rx_buf_desc;

    status = tx_block_allocate(&(nx_modem_driver_control.nx_modem_driver_buf_desc_pool),
                                (VOID **) &rx_buf_desc, TX_NO_WAIT);

    if (status)
    {
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_rx_buf_queue_errors++;
#endif
        return NX_NOT_SUCCESSFUL;
    }

    /* Prepare the buffer descriptor */
    rx_buf_desc -> nx_packet_ptr = packet;
    rx_buf_desc -> buf = packet -> nx_packet_prepend_ptr;
    rx_buf_desc -> next = NX_NULL;

    TX_DISABLE

    /* Add the buffer descriptor to the list */
    if (nx_modem_driver_control.nx_modem_rx_buf_list_head == NX_NULL)
    {
        /* No packets are queued now, add this packet to the list */
        nx_modem_driver_control.nx_modem_rx_buf_list_tail = nx_modem_driver_control.nx_modem_rx_buf_list_head = rx_buf_desc;
    }
    else
    {
        /* Other packets are already in the list, add this packet to the tail */
        nx_modem_driver_control.nx_modem_rx_buf_list_tail -> next = rx_buf_desc;

        /* Update the tail pointer */
        nx_modem_driver_control.nx_modem_rx_buf_list_tail = rx_buf_desc;
    }

    TX_RESTORE

#ifdef NX_MODEM_DEBUG
    nx_modem_driver_control.nx_modem_driver_rx_buf_queued++;
#endif
    return NX_SUCCESS;
}

/* Wrapper routine to send a Tx buffer to the modem using IPC data driver */
static UINT modem_send_tx_buf(uint8_t *buf, uint16_t buf_len, uint8_t cid)
{
MDM_RET_STATUS mdm_status;
UINT retry_count = 5000;
    do
    {
        mdm_status = MDM_DATA_SendTxBuf(buf, buf_len, cid);
        if (mdm_status == MDM_SUCCESS)
        {
            break;
        }
        else if (mdm_status == MDM_BUSY)
        {
            retry_count--;
        }
        else
        {
#ifdef NX_MODEM_DEBUG
            nx_modem_driver_control.nx_modem_driver_tx_buf_send_errors++;
#endif
            return NX_NOT_SUCCESSFUL;
        }
    } while (retry_count);

    if (retry_count)
    {
        return NX_SUCCESS;
    }
    else
    {
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_tx_buf_send_errors++;
#endif
        return NX_NOT_SUCCESSFUL;
    }
}

/*
 * Search and dequeue a Tx NX_PACKET from the Tx buffer descriptor
 * list using the buffer address as the key.
 */
static VOID nx_modem_driver_dequeue_tx_buf(UCHAR *buf)
{
BUF_DESC *head = nx_modem_driver_control.nx_modem_tx_buf_list_head;
BUF_DESC *tx_buf_desc = NX_NULL;
UINT status;

    if (head == NX_NULL)
    {
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_tx_buf_dequeue_errors++;
#endif
        return;
    }

    if (head -> buf == buf)
    {
        tx_buf_desc = head;

        /* Reset the head */
        nx_modem_driver_control.nx_modem_tx_buf_list_head = head->next;

        if (tx_buf_desc == nx_modem_driver_control.nx_modem_tx_buf_list_tail)
        {
            nx_modem_driver_control.nx_modem_tx_buf_list_tail = NX_NULL;
        }
    }

    else
    {
        while (head -> next != NX_NULL)
        {
            if (head -> next -> buf == buf)
            {
                tx_buf_desc = head->next;

                /* Take the descriptor out of the list */
                head -> next = tx_buf_desc -> next;

                /* Check whether we removed the tail, if yes, then reset tail */
                if (tx_buf_desc == nx_modem_driver_control.nx_modem_tx_buf_list_tail)
                {
                    nx_modem_driver_control.nx_modem_tx_buf_list_tail = head;
                }

                break;
            }

            head = head -> next;
        }
    }

    if (tx_buf_desc == NX_NULL)
    {
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_tx_buf_dequeue_errors++;
#endif
        return;
    }

    /* Release the packet */
    nx_packet_transmit_release(tx_buf_desc -> nx_packet_ptr);

    /* Relese the buffer descriptor */
    status = tx_block_release(tx_buf_desc);
#ifdef NX_MODEM_DEBUG
    if (status)
    {
        nx_modem_driver_control.nx_modem_driver_desc_release_errors++;
    }
#endif

#ifdef NX_MODEM_DEBUG
    nx_modem_driver_control.nx_modem_driver_tx_buf_dequeued++;
#endif
}

/* Enqueue a Tx buffer descriptor to the tx buffer desc list */
UINT nx_modem_driver_enqueue_tx_buf(NX_PACKET *packet)
{
TX_INTERRUPT_SAVE_AREA
UINT status;
BUF_DESC *tx_buf_desc;

    status = tx_block_allocate(&(nx_modem_driver_control.nx_modem_driver_buf_desc_pool),
                (VOID **) &tx_buf_desc, TX_NO_WAIT);

    if (status)
    {
#ifdef NX_MODEM_DEBUG
        nx_modem_driver_control.nx_modem_driver_tx_buf_queue_errors++;
#endif
        return NX_NOT_SUCCESSFUL;
    }

    /* Prepare the buffer descriptor */
    tx_buf_desc -> nx_packet_ptr = packet;
    tx_buf_desc -> buf = packet -> nx_packet_prepend_ptr;
    tx_buf_desc -> next = NX_NULL;

    TX_DISABLE

    /* Add the buffer descriptor to the list */
    if (nx_modem_driver_control.nx_modem_tx_buf_list_head == NX_NULL)
    {
        /* No packets are queued now, add this packet to the list */
        nx_modem_driver_control.nx_modem_tx_buf_list_tail = nx_modem_driver_control.nx_modem_tx_buf_list_head = tx_buf_desc;
    }
    else
    {
        /* Other packets are already in the list, add this packet to the tail */
        nx_modem_driver_control.nx_modem_tx_buf_list_tail -> next = tx_buf_desc;

        /* Update the tail pointer */
        nx_modem_driver_control.nx_modem_tx_buf_list_tail = tx_buf_desc;
    }

#ifdef NX_MODEM_DEBUG
    nx_modem_driver_control.nx_modem_driver_tx_buf_queued++;
#endif

    TX_RESTORE

    return NX_SUCCESS;
}

/* Rx thread function to process received packets */
VOID rx_thread_function(ULONG arg)
{
UINT status;
RX_INFO mdm_rx_info;
NX_PACKET *rx_packet, *new_packet;
NX_IP *ip_ptr;
NX_INTERFACE *interface_ptr;
UINT i;

	while (1)
	{
		/* Wait for a buffer to be available in the queue */
        status = tx_queue_receive(&(nx_modem_driver_control.rx_buf_queue), &mdm_rx_info, TX_WAIT_FOREVER);

        if (status == TX_SUCCESS)
        {
            rx_packet = nx_modem_driver_dequeue_rx_buf(mdm_rx_info.buf);

            if (rx_packet == NX_NULL)
            {
                continue;
            }

            /* Find out the IP instance associated with this context id from the driver structure */
            for (i = 0; i < NUM_DRIVER_INSTANCES; i++)
            {
                if (nx_modem_driver[i].nx_modem_driver_in_use == 0)
                {
                    continue;
                }

                if (nx_modem_driver[i].context_id == mdm_rx_info.cid)
                {
                    break;
                }
            }

            if (i == NUM_DRIVER_INSTANCES)
            {
#ifdef NX_MODEM_DEBUG
                nx_modem_driver_control.nx_modem_driver_rx_packet_cid_errors++;
#endif
                /* Invalid context id, drop the packet and queue the buffer back to modem */
                status = nx_modem_driver_enqueue_rx_buf(rx_packet);

                if (status == NX_SUCCESS)
                {
                    status = modem_send_rx_buf(rx_packet -> nx_packet_prepend_ptr, MDM_BUF_LEN);

                    if (status)
                    {
#ifdef NX_MODEM_DEBUG
                        nx_modem_driver_control.nx_modem_driver_rx_buf_send_errors++;
#endif
                    }
                }
                continue;
            }

            /* Retrieve the ip instance pointer */
            ip_ptr = nx_modem_driver[i].nx_modem_driver_ip_ptr;

            /* Retrieve the interface pointer */
            interface_ptr = nx_modem_driver[i].nx_modem_driver_interface_ptr;

            /* Allocate a packet to be queued to the modem */
            status = nx_packet_allocate(ip_ptr->nx_ip_default_packet_pool, &new_packet,
                        NX_RECEIVE_PACKET, TX_NO_WAIT);

            if (status == NX_SUCCESS)
            {
                status = nx_modem_driver_enqueue_rx_buf(new_packet);

                if (status == NX_SUCCESS)
                {
                    status = modem_send_rx_buf(new_packet -> nx_packet_prepend_ptr,
                                                MDM_BUF_LEN);

                    if (status)
                    {
#ifdef NX_MODEM_DEBUG
                        nx_modem_driver_control.nx_modem_driver_rx_buf_send_errors++;
#endif
                    }
                }
                else
                {
                    new_packet = nx_modem_driver_dequeue_rx_buf(new_packet -> nx_packet_prepend_ptr);
                    nx_packet_release(new_packet);
                }

                rx_packet -> nx_packet_length = mdm_rx_info.len;

                rx_packet -> nx_packet_append_ptr = rx_packet -> nx_packet_prepend_ptr + mdm_rx_info.len;

                rx_packet -> nx_packet_address.nx_packet_interface_ptr = interface_ptr;

                /* Forward the packet to IP */
                _nx_ip_packet_deferred_receive(ip_ptr, rx_packet);
            }
            else
            {
                /* No packets available in the pool, we have to drop the rx_packet and give it back
                 * to the modem.
                 */

                status = nx_modem_driver_enqueue_rx_buf(rx_packet);

                if (status == NX_SUCCESS)
                {
                    status = modem_send_rx_buf(rx_packet -> nx_packet_prepend_ptr, MDM_BUF_LEN);

                    if (status)
                    {
#ifdef NX_MODEM_DEBUG
                        nx_modem_driver_control.nx_modem_driver_rx_buf_send_errors++;
#endif
                    }
                }
            }

        }
	}
}

/* Allocate and hand over packets to the modem */
static UINT nx_modem_driver_init_rx_buffers(NX_MODEM_DRIVER *driver)
{
NX_IP *ip_ptr = driver -> nx_modem_driver_ip_ptr;
NX_PACKET *rx_packet;
UINT status = NX_SUCCESS;
INT i;

    for (i = 0; i < RX_BUF_COUNT; i++)
    {
        /* Allocate a packet from the default packet pool */
        status = nx_packet_allocate(ip_ptr -> nx_ip_default_packet_pool, &rx_packet,
                                    NX_RECEIVE_PACKET, TX_NO_WAIT);

        if (status)
        {
            break;
        }

        /* Enqueue this buffer in to the buf desc list */
        status = nx_modem_driver_enqueue_rx_buf(rx_packet);

        if (status)
        {
            break;
        }

        /* Hand over the packet to the modem */
        status = modem_send_rx_buf(rx_packet -> nx_packet_prepend_ptr,
                                            MDM_BUF_LEN);

        if (status)
        {
            break;
        }
    }

    return status;
}

/* Enable notifications from IPC Data driver */
static UINT modem_enable_notification(void)
{
MDM_RET_STATUS mdm_status;
UINT           tx_status;
ULONG          actual_flags;

    mdm_status = MDM_DATA_EnableNotification();
    if (mdm_status != MDM_SUCCESS)
    {
#ifdef NX_MODEM_DEBUG
        printf("\r\nError in enabling Modem notifications\n");
#endif
        return NX_NOT_SUCCESSFUL;
    }

    /* Wait till we get the notification */
    tx_status = tx_event_flags_get(&(nx_modem_driver_control.mdm_event_flags),
                                   MODEM_DATA_NOTIFICATION_ENABLED_EVENT,
                                   TX_OR_CLEAR,
                                   &actual_flags,
                                   5000);
    if ((tx_status == TX_SUCCESS) && (actual_flags & MODEM_DATA_NOTIFICATION_ENABLED_EVENT))
        return NX_SUCCESS;
    else
        return NX_NOT_SUCCESSFUL;
}

/* Initialize the IPC Data driver and register our callback */
static UINT modem_data_init(void)
{
MDM_RET_STATUS mdm_status;
UINT           tx_status;
ULONG          actual_flags;
UINT           retry_count = 500;

    do
    {
        mdm_status = MDM_DATA_Init(&mdm_data_cb);

        if (mdm_status != MDM_SUCCESS)
        {
#ifdef NX_MODEM_DEBUG
            printf("\r\nMDM_DATA_Init: Failed\n");
#endif
            return NX_NOT_SUCCESSFUL;
        }

        /* Wait till we get the notification */
        tx_status = tx_event_flags_get(&(nx_modem_driver_control.mdm_event_flags),
                                       MODEM_DATA_INIT_EVENT,
                                       TX_OR_CLEAR,
                                       &actual_flags,
                                       500);
        if ((tx_status == TX_SUCCESS) && (actual_flags & MODEM_DATA_INIT_EVENT))
            break;
        else if (tx_status != TX_NO_EVENTS)
        {
#ifdef NX_MODEM_DEBUG
            printf("\r\n MDM_DATA_Init: did not receive INIT event\n");
#endif
            return NX_NOT_SUCCESSFUL;
        }
        else
        {
            /* Re-initialize and try again */
            MDM_DATA_DeInit();
        }

    } while (retry_count);

    if (retry_count == 0)
    {
#ifdef NX_MODEM_DEBUG
        printf("\r\n MDM_DATA_Init: timed out\n");
#endif
        return NX_NOT_SUCCESSFUL;
    }
    else
    {
        return NX_SUCCESS;
    }
}

/* Packet send routine, enqueue and hand over tx packet to the modem */
VOID nx_modem_driver_packet_send(NX_PACKET *packet_ptr, NX_MODEM_DRIVER *driver)
{
UINT status;
TX_INTERRUPT_SAVE_AREA

    /* Add this packet to the list of queued packets */
    status = nx_modem_driver_enqueue_tx_buf(packet_ptr);

    if (status)
    {
        nx_packet_transmit_release(packet_ptr);
        return;
    }

    /* Hand over the packet to modem */
    status = modem_send_tx_buf(packet_ptr -> nx_packet_prepend_ptr, packet_ptr -> nx_packet_length,
                                    driver -> context_id);

    if (status)
    {
        TX_DISABLE
        /* dequeue and release the packet */
        nx_modem_driver_dequeue_tx_buf(packet_ptr -> nx_packet_prepend_ptr);
        TX_RESTORE
    }
}

/* Initialize the modem driver */
static UINT nx_modem_driver_init(void)
{
UINT status, i;

	if (nx_modem_driver_control.modem_ipc_init_done)
        return NX_SUCCESS;

    status = tx_event_flags_create(&(nx_modem_driver_control.mdm_event_flags), "MDM EVENT FLAGS");

    if (status)
    {
        return NX_NOT_SUCCESSFUL;
    }

    /* Initialize the modem data channel */
    status = modem_data_init();

    if (status)
    {
        goto err_data_init;
    }

    /* Enable notifications from the modem */
    status = modem_enable_notification();

    if (status)
    {
        goto err_notification;
    }

    /* Create a block pool for buffer descriptors */
	status = tx_block_pool_create(&(nx_modem_driver_control.nx_modem_driver_buf_desc_pool),
                                  "nx modem driver buf desc pool",
                                  sizeof(BUF_DESC),
                                  buf_desc_block_pool_memory,
                                  BUF_DESC_POOL_SIZE);

    if (status)
    {
        goto err_block_pool;
    }

    /* Allocate and hand over Rx buffers to the modem */
    /* Note that these are allocated from the primary IP instance */
    status = nx_modem_driver_init_rx_buffers(&nx_modem_driver[0]);

    if (status)
    {
        goto err_init_rx_buf;
    }

	/* create the rx thread */
    status = tx_thread_create(&(nx_modem_driver_control.rx_thread),
                                "nx modem rx thread",
                                rx_thread_function, 0,
                                rx_thread_stack,
                                RX_THREAD_STACKSIZE,
                                RX_THREAD_PRIORITY,
                                RX_THREAD_PRIORITY,
                                TX_NO_TIME_SLICE, TX_AUTO_START);

	if (status)
	{
        goto err_thread_create;
	}

	/*
     * Create a tx queue to queue recycled rx buffers from modem callback
     * to the rx queue thread.
     */
    status = tx_queue_create(&(nx_modem_driver_control.rx_buf_queue),
                                "nx modem rx buf queue", TX_4_ULONG, rx_buf_queue_area,
                                sizeof(rx_buf_queue_area));

	if (status)
	{
        goto err_queue_create;
	}

    /* Prepare the linked list of queued transmit buffers */
    nx_modem_driver_control.nx_modem_tx_buf_list_head = NX_NULL;
    nx_modem_driver_control.nx_modem_tx_buf_list_tail = NX_NULL;

#ifdef NX_MODEM_DEBUG
    printf("\r\nNetXDuo Modem Driver: Initialized\n");
#endif
    nx_modem_driver_control.modem_ipc_init_done = 1;
    return NX_SUCCESS;

err_queue_create:
    tx_thread_delete(&(nx_modem_driver_control.rx_thread));
err_thread_create:
    tx_block_pool_delete(&(nx_modem_driver_control.nx_modem_driver_buf_desc_pool));
err_block_pool:
err_init_rx_buf:
err_notification:
    MDM_DATA_DeInit();
err_data_init:
    tx_event_flags_delete(&(nx_modem_driver_control.mdm_event_flags));

#ifdef NX_MODEM_DEBUG
    printf("\r\n NetXDuo Modem Driver: Initialization failed\n");
#endif
    return NX_NOT_SUCCESSFUL;
}

/* Main entry point for NetXDuo modem driver */
VOID _nx_modem_driver(NX_IP_DRIVER *driver_req_ptr)
{
NX_IP        *ip_ptr;
NX_PACKET    *packet_ptr;
NX_INTERFACE *interface_ptr;
UINT          interface_index;
ULONG 	      status;
UINT          i = 0;

    /* Setup the IP pointer from the driver request.  */
    ip_ptr =  driver_req_ptr -> nx_ip_driver_ptr;

    /* Default to successful return.  */
    driver_req_ptr -> nx_ip_driver_status =  NX_SUCCESS;

    /* Setup interface pointer.  */
    interface_ptr = driver_req_ptr -> nx_ip_driver_interface;

    /* Obtain the index number of the network interface. */
    interface_index = interface_ptr -> nx_interface_index;

        /* Find out the driver interface if the driver command is not ATTACH. */
    if (driver_req_ptr -> nx_ip_driver_command != NX_LINK_INTERFACE_ATTACH)
    {
        for (i = 0; i < NUM_DRIVER_INSTANCES; i++)
        {
            if (nx_modem_driver[i].nx_modem_driver_in_use == 0)
            {
                continue;
            }

            if (nx_modem_driver[i].nx_modem_driver_ip_ptr != ip_ptr)
            {
                continue;
            }

            if (nx_modem_driver[i].nx_modem_driver_interface_ptr == driver_req_ptr -> nx_ip_driver_interface)
            {
                break;
            }
        }

        if (i == NUM_DRIVER_INSTANCES)
        {
            driver_req_ptr -> nx_ip_driver_status =  NX_INVALID_INTERFACE;
            return;
        }
    }

    /* Process according to the driver request type in the IP control
    block.  */
    switch (driver_req_ptr -> nx_ip_driver_command)
    {
    case NX_LINK_INTERFACE_ATTACH:
    {
        /* Find an available driver instance to attach the interface. */
        for (i = 0; i < NUM_DRIVER_INSTANCES; i++)
        {
            if (nx_modem_driver[i].nx_modem_driver_in_use == 0)
            {
                break;
            }
        }

        /* An available entry is found. */
        if (i < NUM_DRIVER_INSTANCES)
        {
            /* Set the "in use" flag.*/
            nx_modem_driver[i].nx_modem_driver_in_use = 1;

            /* Record the interface attached to the IP instance. */
            nx_modem_driver[i].nx_modem_driver_interface_ptr = driver_req_ptr -> nx_ip_driver_interface;

            /* Record the IP instance. */
            nx_modem_driver[i].nx_modem_driver_ip_ptr = ip_ptr;

            /* Record the PDP context ID as instructed by the user for the instance */
            nx_modem_driver[i].context_id = nx_modem_driver_cids[i];
        }
        else
        {
            driver_req_ptr -> nx_ip_driver_status =  NX_INVALID_INTERFACE;
        }
        break;
    }


    case NX_LINK_INTERFACE_DETACH:
    {
        /* Zero out the driver instance. */
        memset(&(nx_modem_driver[i]), 0, sizeof(NX_MODEM_DRIVER));

        break;
    }

    case NX_LINK_INITIALIZE:
    {
        nx_ip_interface_mtu_set(ip_ptr, interface_index, NX_LINK_MTU);

        /* Initialize the modem IPC interface */
        status = nx_modem_driver_init();

        if (status != NX_SUCCESS)
        {
            driver_req_ptr -> nx_ip_driver_status =  status;
        }

        break;
    }

    case NX_LINK_UNINITIALIZE:
    {
        /* Zero out the driver instance. */
        memset(&(nx_modem_driver[i]), 0, sizeof(NX_MODEM_DRIVER));

        break;
    }

    case NX_LINK_ENABLE:
    {
        interface_ptr -> nx_interface_link_up =  NX_TRUE;

        break;
    }

    case NX_LINK_DISABLE:
    {
        interface_ptr -> nx_interface_link_up =  NX_FALSE;

        break;
    }

    case NX_LINK_PACKET_SEND:
    {
        packet_ptr =  driver_req_ptr -> nx_ip_driver_packet;

        /* Send the packet */
        nx_modem_driver_packet_send(packet_ptr, &nx_modem_driver[i]);

        break;

    }

    case NX_LINK_MULTICAST_JOIN:
    {
        /* Nothing to be done for the modem hardware here */
        break;
    }

    case NX_LINK_MULTICAST_LEAVE:
    {
        /* Nothing to be done for the modem hardware here */
        break;
    }

    case NX_LINK_GET_STATUS:
    {

        /* Return the link status in the supplied return pointer.  */
        *(driver_req_ptr -> nx_ip_driver_return_ptr) =  ip_ptr -> nx_ip_interface[0].nx_interface_link_up;
        break;
    }


    case NX_LINK_GET_SPEED:
    {

        /* Unsupported feature.  */
        *(driver_req_ptr -> nx_ip_driver_return_ptr) = 0;
        break;
    }

    case NX_LINK_GET_DUPLEX_TYPE:
    {

        /* Unsupported feature.  */
        *(driver_req_ptr -> nx_ip_driver_return_ptr) = 0;
        break;
    }

    case NX_LINK_GET_ERROR_COUNT:
    {

        /* Unsupported feature.  */
        *(driver_req_ptr -> nx_ip_driver_return_ptr) = 0;
        break;
    }

    case NX_LINK_GET_RX_COUNT:
    {

        /* Unsupported feature.  */
        *(driver_req_ptr -> nx_ip_driver_return_ptr) = 0;
        break;
    }

    case NX_LINK_GET_TX_COUNT:
    {

        /* Unsupported feature.  */
        *(driver_req_ptr -> nx_ip_driver_return_ptr) = 0;
        break;
    }

    case NX_LINK_GET_ALLOC_ERRORS:
    {

        /* Unsupported feature.  */
        *(driver_req_ptr -> nx_ip_driver_return_ptr) = 0;
        break;
    }

    case NX_LINK_DEFERRED_PROCESSING:
    {
        /* Unsupported feature.  */
        *(driver_req_ptr -> nx_ip_driver_return_ptr) = 0;
        break;
    }

    case NX_LINK_SET_PHYSICAL_ADDRESS:
    {
        /* Unsupported feature.  */
        *(driver_req_ptr -> nx_ip_driver_return_ptr) = 0;
        break;
    }

    default:
        /* Invalid request */
        driver_req_ptr -> nx_ip_driver_status =  NX_UNHANDLED_COMMAND;
        break;

    }
}
