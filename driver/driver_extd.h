/************************************************************************
 *
 *  Module      :
 *  Description : driver definition file
 *
 *
 *  Author(s)   : Morris Ku
 *  Company     : SUNIX Co., Ltd.
 *  Maintain    : Moris Ku
 ************************************************************************/


#ifndef _SNXHW_DRVR_EXTR_H_
#define _SNXHW_DRVR_EXTR_H_


// ioctl define
#ifndef SNX_IOCTL
#define SNX_IOCTL 0x900
#endif


/***********************************************************************
   The IOCTL function codes.
***********************************************************************/


// common func
#define SNX_COMM_GET_BOARD_CNT          (SNX_IOCTL + 100)
#define SNX_COMM_GET_BOARD_INFO         (SNX_IOCTL + 101)


// gpio func
#define SNX_GPIO_GET                    (SNX_IOCTL + 200)
#define SNX_GPIO_SET                    (SNX_IOCTL + 201)
#define SNX_GPIO_READ                   (SNX_IOCTL + 202)
#define SNX_GPIO_WRITE                  (SNX_IOCTL + 203)
#define SNX_GPIO_SET_DEFAULT            (SNX_IOCTL + 204)
#define SNX_GPIO_WRITE_DEFAULT          (SNX_IOCTL + 205)
#define SNX_GPIO_GET_INPUT_INVERT       (SNX_IOCTL + 206)
#define SNX_GPIO_SET_INPUT_INVERT       (SNX_IOCTL + 207)


// uart func
#define SNX_UART_GET_TYPE               (SNX_IOCTL + 300)
#define SNX_UART_SET_TYPE               (SNX_IOCTL + 301)
#define SNX_UART_GET_ACS                (SNX_IOCTL + 302)
#define SNX_UART_SET_ACS                (SNX_IOCTL + 303)


/***********************************************************************
   The IOCTL function struct define.
***********************************************************************/


#define SNX_BOARD_MAX_UARTCNT     32


// gpio define
#define SNX_GPIO_IN               0
#define SNX_GPIO_OUT              1
#define SNX_GPIO_LOW              0
#define SNX_GPIO_HI               1
#define SNX_GPIO_INPUT_INVERT_D   0
#define SNX_GPIO_INPUT_INVERT_E   1


// uart define
#define SNX_UART_RS232            0
#define SNX_UART_RS422            1
#define SNX_UART_RS485            2
#define SNX_UART_A422485          3
#define SNX_UART_ACS_D            0
#define SNX_UART_ACS_E            1


// board gpio type define
#define SNX_GPIO_TYPE_STANDARD    0
#define SNX_GPIO_TYPE_CASHDRAWER  1


// board uart type define
#define SNX_UART_TYPE_RS232       0
#define SNX_UART_TYPE_RS422485    1
#define SNX_UART_TYPE_3IN1        2


typedef struct _SNX_DRVR_BOARD_CNT {
  int                 cnt;                              // total sunix board installed

} SNX_DRVR_BOARD_CNT, *PSNX_DRVR_BOARD_CNT;


typedef struct _SNX_DRVR_UART_INFO {
  int                 status;                           // port status, see com port status define
  int                 node_num;                         // device node number
  int                 uart_type;                        // port uart type


} SNX_DRVR_UART_INFO, *PSNX_DRVR_UART_INFO;


typedef struct _SNX_DRVR_BOARD_INFO {
  int                 board_id;                         // board uniqe id, (1 ~ 255)
  int                 subvender_id;                     // sub-vender id (ex. 0x1fd4)
  int                 subsystem_id;                     // sub-system id (ex. 0x0004)
  int                 oem_id;                           // oemid (3 bytes)
  int                 uart_cnt;                         // uart count
  SNX_DRVR_UART_INFO  uart_info[SNX_BOARD_MAX_UARTCNT]; // uart info
  int                 gpio_chl_cnt;                     // gpio channel count
  int                 board_uart_type;                  // board uart type, refer to board uart type define
  int                 board_gpio_type;                  // board gpio type, refer to board gpio type define

} SNX_DRVR_BOARD_INFO, *PSNX_DRVR_BOARD_INFO;


typedef struct _SNX_DRVR_GPIO_GET {
  int                 board_id;                         // board uniqe id, (1 ~ 255)
  unsigned char       bank1_direct;                     // bank1 direction (chl 1  ~ 8)
  unsigned char       bank2_direct;                     // bank2 direction (chl 9  ~ 16)
  unsigned char       bank3_direct;                     // bank3 direction (chl 17 ~ 24)
  unsigned char       bank4_direct;                     // bank4 direction (chl 25 ~ 32)

} SNX_DRVR_GPIO_GET, *PSNX_DRVR_GPIO_GET;


typedef struct _SNX_DRVR_GPIO_SET {
  int                 board_id;                         // board uniqe id, (1 ~ 255)
  unsigned char       bank1_direct;                     // bank1 direction (chl 1  ~ 8)
  unsigned char       bank2_direct;                     // bank2 direction (chl 9  ~ 16)
  unsigned char       bank3_direct;                     // bank3 direction (chl 17 ~ 24)
  unsigned char       bank4_direct;                     // bank4 direction (chl 25 ~ 32)

} SNX_DRVR_GPIO_SET, *PSNX_DRVR_GPIO_SET;


typedef struct _SNX_DRVR_GPIO_READ {
  int                 board_id;                         // board uniqe id, (1 ~ 255)
  unsigned char       bank1_signal;                     // bank1 signal (chl 1  ~ 8)
  unsigned char       bank2_signal;                     // bank2 signal (chl 9  ~ 16)
  unsigned char       bank3_signal;                     // bank3 signal (chl 17 ~ 24)
  unsigned char       bank4_signal;                     // bank4 signal (chl 25 ~ 32)

} SNX_DRVR_GPIO_READ, *PSNX_DRVR_GPIO_READ;


typedef struct _SNX_DRVR_GPIO_WRITE {
  int                 board_id;                         // board uniqe id, (1 ~ 255)
  unsigned char       bank1_signal;                     // bank1 signal (chl 1  ~ 8)
  unsigned char       bank2_signal;                     // bank2 signal (chl 9  ~ 16)
  unsigned char       bank3_signal;                     // bank3 signal (chl 17 ~ 24)
  unsigned char       bank4_signal;                     // bank4 signal (chl 25 ~ 32)

} SNX_DRVR_GPIO_WRITE, *PSNX_DRVR_GPIO_WRITE;


typedef struct _SNX_DRVR_GPIO_GET_INPUT_INVERT {
  int                 board_id;                         // board uniqe id, (1 ~ 255)
  unsigned char       bank1_invert;                     // bank1 input invert (chl 1  ~ 8)
  unsigned char       bank2_invert;                     // bank2 input invert (chl 9  ~ 16)
  unsigned char       bank3_invert;                     // bank3 input invert (chl 17 ~ 24)
  unsigned char       bank4_invert;                     // bank4 input invert (chl 25 ~ 32)

} SNX_DRVR_GPIO_GET_INPUT_INVERT, *PSNX_DRVR_GPIO_GET_INPUT_INVERT;


typedef struct _SNX_DRVR_GPIO_SET_INPUT_INVERT {
  int                 board_id;                         // board uniqe id, (1 ~ 255)
  unsigned char       bank1_invert;                     // bank1 input invert (chl 1  ~ 8)
  unsigned char       bank2_invert;                     // bank2 input invert (chl 9  ~ 16)
  unsigned char       bank3_invert;                     // bank3 input invert (chl 17 ~ 24)
  unsigned char       bank4_invert;                     // bank4 input invert (chl 25 ~ 32)

} SNX_DRVR_GPIO_SET_INPUT_INVERT, *PSNX_DRVR_GPIO_SET_INPUT_INVERT;


typedef struct _SNX_DRVR_UART_GET_TYPE {
  int                 board_id;                         // board uniqe id, (1 ~ 255)
  int                 uart_num;                         // uart number, (1 ~ SNX_BOARD_MAX_UARTCNT)
  int                 uart_type;                        // uart type, refer to uart define

} SNX_DRVR_UART_GET_TYPE, *PSNX_DRVR_UART_GET_TYPE;


typedef struct _SNX_DRVR_UART_SET_TYPE {
  int                 board_id;                         // board uniqe id, (1 ~ 255)
  int                 uart_num;                         // uart number, (1 ~ SNX_BOARD_MAX_UARTCNT)
  int                 uart_type;                        // uart type, refer to uart define

} SNX_DRVR_UART_SET_TYPE, *PSNX_DRVR_UART_SET_TYPE;


typedef struct _SNX_DRVR_UART_GET_ACS {
  int                 board_id;                         // board uniqe id, (1 ~ 255)
  int                 uart_num;                         // uart number, (1 ~ SNX_BOARD_MAX_UARTCNT)
  int                 uart_acs;                         // uart ACS, refer to uart define

} SNX_DRVR_UART_GET_ACS, *PSNX_DRVR_UART_GET_ACS;


typedef struct _SNX_DRVR_UART_SET_ACS {
  int                 board_id;                         // board uniqe id, (1 ~ 255)
  int                 uart_num;                         // uart number, (1 ~ SNX_BOARD_MAX_UARTCNT)
  int                 uart_acs;                         // uart ACS, refer to uart define

} SNX_DRVR_UART_SET_ACS, *PSNX_DRVR_UART_SET_ACS;


#endif
