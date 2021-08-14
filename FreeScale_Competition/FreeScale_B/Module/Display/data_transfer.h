#ifndef __DATA_TRANSFER_H__
#define __DATA_TRANSFER_H__

#include "include.h"
#include "stdint.h"


typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */
typedef struct 
{
	u8 msg_id;
	u8 msg_data;
	u8 send_version;
	u8 send_status;
	u8 send_senser;
	u8 send_senser2;
	u8 send_pid1;
	u8 send_pid2;
	u8 send_pid3;
	u8 send_pid4;
	u8 send_pid5;
	u8 send_pid6;
	u8 send_rcdata;
	u8 send_offset;
	u8 send_motopwm;
	u8 send_power;
	u8 send_user;
	u8 send_speed;
	u8 send_location;

}dt_flag_t;


void send_user_data1(int16_t *data);
extern int16_t UserData[10];

#endif
