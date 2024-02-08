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
 * @file     nx_modem_user.h
 * @author   Silesh C V
 * @email    silesh@alifsemi.com
 * @version  V1.0.0
 * @date     13-Dec-2021
 * @brief    User configuration file for the NetXDuo modem network driver.
 * @bug      None.
 * @Note     None
 ******************************************************************************/

#ifndef NX_MODEM_USER_H
#define NX_MODEM_USER_H
//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h> NetXDuo Modem driver Configuration
// =========================================


//   <o> Driver Buffer Descriptors
//   <i> Defines the total (including Tx and Rx) number of buffers that can be handed over to the modem at the same time.
//   <i> Default:128
#define BUF_DESC_COUNT              128
//   <o> Rx thread priority
//   <i> Defines the threadX priority of the thread handling packet reception.
//   <i> Default:10
#define RX_THREAD_PRIORITY	        10
//   <o> Preallocated Rx packet count
//   <i> Defines the number of NetX packets that are allocated and handed over to the modem to be used during packet reception.
//   <i> Default:8
#define RX_BUF_COUNT                8
//   <o> Number of modem driver instances
//   <i> Defines the number of driver instances. Each instance corresponds to one PDP contex id.
//   <i> Default:1
#define NUM_DRIVER_INSTANCES        1
//   <o> The PDP context id to be associated with modem driver intance 0
//   <i> Defines the PDP context id to be associated with modem driver instance 0.
//   <i> Default:5
#define CONTEXT_ID_DRIVER_INSTANCE_0    5
//   <o> The PDP context id to be associated with modem driver intance 1
//   <i> Defines the PDP context id to be associated with modem driver instance 1.
//   <i> Default:0
#define CONTEXT_ID_DRIVER_INSTANCE_1    0
//   <o> The PDP context id to be associated with modem driver intance 2
//   <i> Defines the PDP context id to be associated with modem driver instance 2.
//   <i> Default:0
#define CONTEXT_ID_DRIVER_INSTANCE_2    0
//   <o> The PDP context id to be associated with modem driver intance 3
//   <i> Defines the PDP context id to be associated with modem driver instance 3.
//   <i> Default:0
#define CONTEXT_ID_DRIVER_INSTANCE_3    0
//   <o> The PDP context id to be associated with modem driver intance 4
//   <i> Defines the PDP context id to be associated with modem driver instance 4.
//   <i> Default:0
#define CONTEXT_ID_DRIVER_INSTANCE_4    0
//   <o> The PDP context id to be associated with modem driver intance 5
//   <i> Defines the PDP context id to be associated with modem driver instance 5.
//   <i> Default:0
#define CONTEXT_ID_DRIVER_INSTANCE_5    0
//   <o> The PDP context id to be associated with modem driver intance 6
//   <i> Defines the PDP context id to be associated with modem driver instance 6.
//   <i> Default:0
#define CONTEXT_ID_DRIVER_INSTANCE_6    0
//   <o> The PDP context id to be associated with modem driver intance 7
//   <i> Defines the PDP context id to be associated with modem driver instance 7.
//   <i> Default:0
#define CONTEXT_ID_DRIVER_INSTANCE_7    0
//   <o> The PDP context id to be associated with modem driver intance 8
//   <i> Defines the PDP context id to be associated with modem driver instance 8.
//   <i> Default:0
#define CONTEXT_ID_DRIVER_INSTANCE_8    0
//   <o> The PDP context id to be associated with modem driver intance 9
//   <i> Defines the PDP context id to be associated with modem driver instance 9.
//   <i> Default:0
#define CONTEXT_ID_DRIVER_INSTANCE_9    0
//   <o> The PDP context id to be associated with modem driver intance 10
//   <i> Defines the PDP context id to be associated with modem driver instance 10.
//   <i> Default:0
#define CONTEXT_ID_DRIVER_INSTANCE_10    0
//   <o> The PDP context id to be associated with modem driver intance 11
//   <i> Defines the PDP context id to be associated with modem driver instance 11.
//   <i> Default:0
#define CONTEXT_ID_DRIVER_INSTANCE_11    0
// </h>
//------------- <<< end of configuration section >>> ---------------------------
#endif /* NX_MODEM_USER_H */
