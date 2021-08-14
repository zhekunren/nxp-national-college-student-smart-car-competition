#ifndef _FLASH_H_
#define _FLASH_H_

#define BUFFER_LEN 24

/*! @brief Buffer for program */
extern uint32_t S_buffer[BUFFER_LEN];
/*! @brief Buffer for readback */
extern uint32_t DianGan__Buffer[BUFFER_LEN];

void Flash_control(void);
//void  Flash_control1(void);
void Flash_Init(void);
void Flash_Heng(void);
void Flash_Shu(void);
#endif
