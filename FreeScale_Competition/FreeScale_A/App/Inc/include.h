#ifndef _INCLUDE_H_
#define _INCLUDE_H_

//定义NVIC中断分组
#define NVIC_PriorityGroup_0         ((uint32_t)0x07) /*!< 0 bits for pre-emption priority
                                                            4 bits for subpriority */
#define NVIC_PriorityGroup_1         ((uint32_t)0x06) /*!< 1 bits for pre-emption priority
                                                            3 bits for subpriority */
#define NVIC_PriorityGroup_2         ((uint32_t)0x05) /*!< 2 bits for pre-emption priority
                                                            2 bits for subpriority */
#define NVIC_PriorityGroup_3         ((uint32_t)0x04) /*!< 3 bits for pre-emption priority
                                                            1 bits for subpriority */
#define NVIC_PriorityGroup_4         ((uint32_t)0x03) /*!< 4 bits for pre-emption priority
                                                            0 bits for subpriority */


#include "fsl_common.h"
#include "stdio.h"
#include "mk60d10.h"
#include "clock_config.h"
#include "pin_mux.h"


#include "init.h"
#include "fsl_uart.h"


#include "fsl_gpio.h"

#include "board.h"

#include "fsl_debug_console.h"

#include "fsl_pit.h"
#include "fsl_adc16.h"
#include "fsl_ftm.h"
#include "fsl_port.h"
#include "fsl_edma.h"

#include "led.h"
#include "gpio_port.h"
#include "disply_send.h"
#include "fsl_edma.h"
#include "osc.h"
#include "scc_cmt.h"
#include "data_transfer.h"
#include "fsl_flash.h"

#include "camera.h"
#include "sccb.h"
#include "ov7725_reg.h"

#include "speed_control.h"
#include "deal_image.h"
#include "deal_elec.h"
#include "Servo_Control.h"
#include "flash.h"
#include "math.h"


#include "6050_i2c.h"
//#include "mpu6050.h"
#include "user_i2c.h"
#include "mandarin.h"

#include "24l01_f.h"
#include "24l01_s.h"

#include "meeting_car.h"

void Delay_Time(uint32_t n);
extern int16_t shou1,shou2;
extern int32_t Satrt_Count;


#endif
