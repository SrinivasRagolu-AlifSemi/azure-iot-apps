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
 * @file     nx_modem_driver.h
 * @author   Silesh C V
 * @email    silesh@alifsemi.com
 * @version  V1.0.0
 * @date     13-Dec-2021
 * @brief    Public header file for the NetXDuo modem network driver.
 * @bug      None.
 * @Note     None
 ******************************************************************************/

#ifndef NX_MODEM_DRIVER_H
#define NX_MODEM_DRIVER_H
/* The driver entry point */
VOID  _nx_modem_driver(NX_IP_DRIVER *driver_req_ptr);
#endif
