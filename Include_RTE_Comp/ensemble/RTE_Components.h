
/*
 * RTE_Component.h header file, for both M55_HP and M55_HE devices of Ensemble pack.
 * Update manually the macro definitions as per the requirement.
*/

#ifndef RTE_COMPONENTS_H
#define RTE_COMPONENTS_H

/*
 * Define the Device Header File:
*/
#define CMSIS_device_header "M55_HE.h"

/* AlifSemiconductor::Netxduo support */
#define RTE_AZURE_RTOS_NETXDUO                      1           /* Include or Exclude Netxduo for the build */
/* AlifSemiconductor::USBX support */
#define RTE_AZURE_RTOS_USBX                         0           /* Include or Exclude USBX for the build    */
/* AlifSemiconductor::FILEX support */
#define RTE_AZURE_RTOS_FILEX                        0           /* Include or Exclude FILEX for the build   */

/* AlifSemiconductor::Device.SOC Peripherals.ADC */
#define RTE_Drivers_ADC                             0           /* Driver ADC                               */
/* AlifSemiconductor::Device.SOC Peripherals.DAC */
#define RTE_Drivers_DAC                             0           /* Driver DAC                               */
/* AlifSemiconductor::Device.SOC Peripherals.Analog Config */
#define RTE_Drivers_ANALOG_CONFIG                   0           /* Driver Analog Config                     */
/* AlifSemiconductor::Device.SOC Peripherals.Comparator */
#define RTE_Drivers_COMPARATOR                      0           /* Driver COMPARATOR                        */
/* AlifSemiconductor::Device.SOC Peripherals.CRC */
#define RTE_Drivers_CRC                             0           /* Driver CRC                               */
/* AlifSemiconductor::CMSIS Driver.SAI.I2S */
#define RTE_Drivers_SAI                             0           /* Driver SAI                               */
/* AlifSemiconductor::CMSIS Driver.SOC Peripherals.CAMERA Controller */
#define RTE_Drivers_CAMERA0                         0           /* Driver CAMREA Controller                 */
/* AlifSemiconductor::CMSIS Driver.SOC Peripherals.CAMERA Sensor MT9M114 */
#define RTE_Drivers_CAMERA_SENSOR_MT9M114           0           /* Driver CAMREA Sensor                     */
/* AlifSemiconductor::BSP.External peripherals.CAMERA Sensor ARX3A0 */
#define RTE_Drivers_CAMERA_SENSOR_ARX3A0            0           /* Driver CAMERA Sensor                     */
/* AlifSemiconductor::Device.SOC Peripherals.CDC200 */
#define RTE_Drivers_CDC200                          0           /* Driver CDC200                            */
/* AlifSemiconductor::BSP.External peripherals.ILI9806E LCD panel */
#define RTE_Drivers_MIPI_DSI_ILI9806E_PANEL         0           /* Driver ILI9806E LCD panel                */
/* AlifSemiconductor::Device.SOC Peripherals.HWSEM */
#define RTE_Drivers_HWSEM                           0           /* Driver HWSEM                             */
/* AlifSemiconductor::Device.SOC Peripherals.MRAM Flash */
#define RTE_Drivers_FLASH_MRAM                      0           /* Driver MRAM FLASH                        */
/* AlifSemiconductor::CMSIS Driver.SOC Peripherals.ETHERNET */
#define RTE_Drivers_ETH                             1           /* Driver ETHERNET                          */
/* AlifSemiconductor::CMSIS Driver.SOC Peripherals.GPIO */
#define RTE_Drivers_GPIO                            1           /* Driver GPIO                              */
/* AlifSemiconductor::CMSIS Driver.SOC Peripherals.I3C */
#define RTE_Drivers_I3C0                            0           /* Driver I3C                               */
/* AlifSemiconductor::CMSIS Driver.SOC Peripherals.LPTIMER */
#define RTE_Drivers_LPTIMER                         0           /* Driver LPTIMER                           */
/* AlifSemiconductor::CMSIS Driver.SOC Peripherals.PINCONF */
#define RTE_Drivers_PINCONF                         1           /* Driver PinPAD and PinMux                 */
/* AlifSemiconductor::CMSIS Driver.SOC Peripherals.RTC */
#define RTE_Drivers_RTC                             0           /* Driver RTC                               */
/* AlifSemiconductor::CMSIS Driver.SOC Peripherals.UTIMER */
#define RTE_Drivers_UTIMER                          0           /* Driver UTIMER                            */
/* AlifSemiconductor::CMSIS Driver.SOC Peripherals.WDT */
#define RTE_Drivers_WDT                             0           /* Driver WDT                               */
/* AlifSemiconductor::CMSIS Driver.SPI.SPI */
#define RTE_Drivers_SPI                             0           /* Driver SPI                               */
/* AlifSemiconductor::CMSIS Driver.Touchscreen.GT911 */
#define RTE_Drivers_GT911                           0
/* AlifSemiconductor::Device.SOC Peripherals.DMA */
#define RTE_Drivers_DMA                             0           /* Driver DMA                               */
/* AlifSemiconductor::Device.SOC Peripherals.MIPI CSI2 */
#define RTE_Drivers_MIPI_CSI2                       0           /* Driver MIPI CSI2                         */
/* AlifSemiconductor::Device.SOC Peripherals.MIPI DSI */
#define RTE_Drivers_MIPI_DSI                        0           /* Driver MIPI DSI                          */
/* AlifSemiconductor::BSP.External peripherals.OSPI Flash ISSI */
#define RTE_Drivers_ISSI_FLASH                      0
/* AlifSemiconductor::Device.SOC Peripherals.OSPI Controller */
#define RTE_Drivers_OSPI                            0           /* Driver OSPI                              */
/* AlifSemiconductor::SD Driver */
#define RTE_Drivers_SD                              0
/* AlifSemiconductor::GNSS Driver */
#define RTE_Drivers_GNSS                            0
/* AlifSemiconductor::Device.SOC Peripherals.PDM */
#define RTE_Drivers_PDM                             0           /* Driver PDM                               */
/* AlifSemiconductor::Device.SOC Peripherals.CPI */
#define RTE_Drivers_CPI                             0           /* Driver CPI                               */
/* AlifSemiconductor::Device.SOC Peripherals.I2C */
#define RTE_Drivers_I2C                             0           /* Driver I2C                               */
/* AlifSemiconductor::Device.SOC Peripherals.LPI2C */
#define RTE_Drivers_LPI2C                           0           /* Driver Low Power I2C                     */
/* AlifSemiconductor::Device.SOC Peripherals.CANFD */
#define RTE_Drivers_CANFD                           0           /* Driver CANFD                             */
/* AlifSemiconductor::CMSIS Driver.USART.USART */
#define RTE_Drivers_USART0                          0           /* Driver UART0                             */
    #define RTE_Drivers_USART1                      0           /* Driver UART1                             */
    #define RTE_Drivers_USART2                      0           /* Driver UART1                             */
    #define RTE_Drivers_USART3                      0           /* Driver UART1                             */
    #define RTE_Drivers_USART4                      0           /* Driver UART1                             */
    #define RTE_Drivers_USART5                      0           /* Driver UART1                             */
    #define RTE_Drivers_USART6                      0           /* Driver UART1                             */
    #define RTE_Drivers_USART7                      0           /* Driver UART1                             */

//#define RTE_Compiler_IO_STDIN                                 /* Enable STDIN  Control                    */
#ifdef  RTE_Compiler_IO_STDIN
    //#define STDIN_ECHO                              1         /* Enable Local ECHO after entering input   */
    //#define RTE_Compiler_IO_STDIN_BKPT                        /* All STDIN call will be redirected to BKPT*/
    //#define RTE_Compiler_IO_STDIN_EVR                         /* All STDIN call will be redirected to EVR */
    //#define RTE_Compiler_IO_STDIN_ITM                         /* All STDIN call will be redirected to ITM */
    #define RTE_Compiler_IO_STDIN_User                          /* All STDIN call will be redirected to user*/
#endif  /* RTE_Compiler_IO_STDIN */

#if (STDIN_ECHO == 1)
    #define RTE_Compiler_IO_STDOUT                              /* Enable STDOUT                            */
#endif  /*STDIN_ECHO */

//#define RTE_Compiler_IO_STDOUT                                  /* Enable STDOUT  Control                   */
#ifdef RTE_Compiler_IO_STDOUT
    //#define STDOUT_CR_LF                                      /* Append CR at end on STDOUT               */
    //#define RTE_Compiler_IO_STDOUT_BKPT                       /* All STDIN call will be redirected to BKPT*/
    //#define RTE_Compiler_IO_STDOUT_EVR                        /* All STDIN call will be redirected to EVR */
    //#define RTE_Compiler_IO_STDOUT_ITM                        /* All STDIN call will be redirected to ITM */
    #define RTE_Compiler_IO_STDOUT_User                         /* All STDIN call will be redirected to user*/
#endif  /* RTE_Compiler_IO_STDOUT */

//#define RTE_Compiler_IO_STDERR                                /* Enable STDERR  Control                   */
#ifdef  RTE_Compiler_IO_STDERR
    //#define STDERR_CR_LF                                      /* Append CR at end on STDOUT               */
    //#define RTE_Compiler_IO_STDERR_BKPT                       /* All STDIN call will be redirected to BKPT*/
    //#define RTE_Compiler_IO_STDERR_EVR                        /* All STDIN call will be redirected to EVR */
    //#define RTE_Compiler_IO_STDERR_ITM                        /* All STDIN call will be redirected to ITM */
    #define RTE_Compiler_IO_STDERR_User                         /* All STDIN call will be redirected to user*/
#endif /* RTE_Compiler_IO_STDERR */

//define RTE_Compiler_IO_TTY

#endif /* RTE_COMPONENTS_H */
