#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "fsl_edma.h"

#define ARR_SIZE(a) (sizeof((a))/sizeof(((a)[0])))
    
#define LPLD_DMA_LoadDstAddr(CHX, ADDR)   (DMA0->TCD[CHX].DADDR=DMA_DADDR_DADDR(ADDR))

#define LPLD_DMA_EnableReq(CHX)  (DMA0->ERQ |= (1<<CHX))


typedef struct
{
  uint8_t addr;
  uint8_t val;
}reg_s;


#define CAMERA_SIZE         OV7725_EAGLE_SIZE           //????????
#define CAMERA_DMA_NUM      OV7725_EAGLE_DMA_NUM        //DMA????
#define CAMERA_W            160              //?????????
#define CAMERA_H            120             //?????????
#define COLUMN_MID          80
#define COLUMN_MIN          0
#define ROW_MIN             0

//????? ??
#define OV7725_EAGLE_W            CAMERA_W                                    //?????????
#define OV7725_EAGLE_H            CAMERA_H                                    //?????????
#define OV7725_EAGLE_SIZE         (OV7725_EAGLE_W * OV7725_EAGLE_H/8)  //????????             //DMA????


extern uint8_t imgbuff[OV7725_EAGLE_W*OV7725_EAGLE_H/8];
extern uint8_t imgbuff1[CAMERA_H*CAMERA_W/8];
extern uint8_t image_buffer[CAMERA_H][CAMERA_W];


typedef enum
{
    IMG_NOTINIT = 0,
    IMG_FINISH,             //??????
    IMG_FAIL,               //??????(??????)
    IMG_GATHER,             //?????
    IMG_START,              //??????
    IMG_STOP,               //??????
} IMG_STATUS_e;

extern uint8_t ImgFlag;
extern char Image_Finish;
void OV7725_EAGLE_Delay_ms(uint16_t ms);
void OV7725_eagle_init(uint8_t *imgaddr);
void StartGetImg(void);
void DMA_Callback(void);

extern uint8_t Camera;
#endif
