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
 * @file     nx_modem_driver_private.h
 * @author   Silesh C V
 * @email    silesh@alifsemi.com
 * @version  V1.0.0
 * @date     13-Dec-2021
 * @brief    Private header file for the NetXDuo modem network driver.
 * @bug      None.
 * @Note     None
 ******************************************************************************/

#ifndef NX_MODEM_DRIVER_PRIVATE_H
#define NX_MODEM_DRIVER_PRIVATE_H

/* Buffer descriptor */
typedef struct _nx_modem_buf_desc {
    NX_PACKET *nx_packet_ptr;
    UCHAR *buf;
    struct _nx_modem_buf_desc *next;
} BUF_DESC;

/* Rx buffer information */
typedef struct _rx_info {
    /* Buffer pointer */
    UCHAR *buf;
    /* Buffer length */
    UINT  len;
    /* PDP context id*/
    UCHAR cid;
} RX_INFO;

/* IPC events received */
typedef struct _ipc_stats {
    /* Number of init events */
    UINT init_event;

    /* Number of Rx buffer unavailable events */
    UINT rx_buf_unavailable;

    /* Number of rx buffer copied events */
    UINT rx_buf_copied;

    /* Number of rx buffer queue full events */
    UINT rx_queue_full;

    /* Number of tx bufer release events */
    UINT tx_buf_rel;

    /* Number of rx buffer release events */
    UINT rx_buf_rel;

    /* Number of reset done events */
    UINT reset_done;

    /* Number of notifications enabled events */
    UINT notify_enabled;

    /* Number of rx buffer len error events */
    UINT rx_buf_len_error;
} IPC_STATS;

typedef struct _nx_modem_driver_instance_type
{
    /* The driver is in use */
    UINT         nx_modem_driver_in_use;

    /* IP interface pointer */
    NX_INTERFACE *nx_modem_driver_interface_ptr;

    /* IP instance attached to this interface */
    NX_IP        *nx_modem_driver_ip_ptr;

    /* PDP context id associated with this driver */
    UCHAR         context_id;
} NX_MODEM_DRIVER;

/* Data structure to hold control and status information of all modem drivers */
typedef struct nx_modem_driver_control {
    /* ThreadX thread used to process rx buffers */
	TX_THREAD	rx_thread;

    /* ThreadX queue used for queing received buffers */
	TX_QUEUE	rx_buf_queue;

    /* ThreadX block pool for buffer descriptors */
    TX_BLOCK_POOL   nx_modem_driver_buf_desc_pool;

    /* ThreadX event flags for modem events */
    TX_EVENT_FLAGS_GROUP mdm_event_flags;

    /* List head for Tx buffers queued to the modem */
    BUF_DESC    *nx_modem_tx_buf_list_head;

    /* List tail for Tx buffers queued to the modem */
    BUF_DESC    *nx_modem_tx_buf_list_tail;

    /* List head for Rx buffers queued to the modem */
    BUF_DESC    *nx_modem_rx_buf_list_head;

    /* List tail for Rx buffers queued to the modem */
    BUF_DESC    *nx_modem_rx_buf_list_tail;

    UINT         modem_ipc_init_done;
#ifdef NX_MODEM_DEBUG
    UINT         nx_modem_driver_tx_buf_queued;

    UINT         nx_modem_driver_tx_buf_dequeued;

    UINT         nx_modem_driver_rx_buf_queued;

    UINT         nx_modem_driver_rx_buf_dequeued;

    UINT         nx_modem_driver_tx_buf_queue_errors;

    UINT         nx_modem_driver_tx_buf_dequeue_errors;

    UINT         nx_modem_driver_rx_buf_queue_errors;

    UINT         nx_modem_driver_rx_buf_dequeue_errors;

    UINT         nx_modem_driver_tx_buf_send_errors;

    UINT         nx_modem_driver_rx_buf_send_errors;

    UINT         nx_modem_driver_desc_release_errors;

    UINT         nx_modem_driver_rx_packet_cid_errors;

    /* IPC event statistics */
    IPC_STATS    nx_modem_driver_ipc_stats;
#endif
} NX_MODEM_DRIVER_CONTROL;

#endif

