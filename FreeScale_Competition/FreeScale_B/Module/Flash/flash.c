#include "include.h"


/*! @brief Buffer for program */
uint32_t S_buffer[BUFFER_LEN];
/*! @brief Buffer for readback */
uint32_t DianGan__Buffer[BUFFER_LEN];
 
/*
* @brief Gets called when an error occurs.
*
* @details Print error message and trap forever.
*/

void error_trap(void)
{
//    LCD_P6x8Str(0,8,"flash err");
//    while (1)
//    {
//        
//    }
}

void Flash_control(void)
{
    uint32_t i;
    status_t result;    /* Return code from each flash driver function */
    uint32_t destAdrss; /* Address of the target location */
    flash_config_t flash_init;
    uint32_t pflashBlockBase = 0;
    uint32_t pflashTotalSize = 0;
    uint32_t pflashSectorSize = 0;
    
    /* Clean up Flash driver Structure*/
    memset(&flash_init, 0, sizeof(flash_config_t));//将flash_init中值清零
    
    /* Setup flash driver structure for device and initialize variables. */
    result = FLASH_Init(&flash_init);
    if (kStatus_FLASH_Success != result)
    {
        error_trap();
    }
    /* Get flash properties*/
    FLASH_GetProperty(&flash_init, kFLASH_PropertyPflashBlockBaseAddr, &pflashBlockBase);
    FLASH_GetProperty(&flash_init, kFLASH_PropertyPflashTotalSize, &pflashTotalSize);
    FLASH_GetProperty(&flash_init, kFLASH_PropertyPflashSectorSize, &pflashSectorSize);
    
     
    /* Erase a sector from destAdrss. */
#if defined(FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP) && FSL_FEATURE_FLASH_HAS_PFLASH_BLOCK_SWAP
        /* Note: we should make sure that the sector shouldn't be swap indicator sector*/
        destAdrss = pflashBlockBase + (pflashTotalSize - (pflashSectorSize * 2));
#else
        destAdrss = pflashBlockBase + (pflashTotalSize - pflashSectorSize);
#endif
    
    result = FLASH_Erase(&flash_init, destAdrss, pflashSectorSize, kFLASH_ApiEraseKey);
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
        }
        
     /* Verify sector if it's been erased. */
        result = FLASH_VerifyErase(&flash_init, destAdrss, pflashSectorSize, kFLASH_MarginValueUser);
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
        }
        
      /* Prepare user buffer. */
        for (i = 0; i < BUFFER_LEN; i++)
        {
            S_buffer[i] = i;
        }
        
        /* Program user buffer into flash*/
        result = FLASH_Program(&flash_init, destAdrss, S_buffer, sizeof(S_buffer));
        if (kStatus_FLASH_Success != result)
        {
            error_trap();
        }

        
        /* Verify programming by reading back from flash directly*/
        for (uint32_t i = 0; i < BUFFER_LEN; i++)
        {
            DianGan__Buffer[i] = *(volatile uint32_t *)(destAdrss + i * 4);
            if (DianGan__Buffer[i] != S_buffer[i])
            {
                error_trap();
            }
        }
        
    /* Erase the context we have progeammed before*/
    /* Note: we should make sure that the sector which will be set as swap indicator should be blank*/
    FLASH_Erase(&flash_init, destAdrss, pflashSectorSize, kFLASH_ApiEraseKey);  
}

/****
void  Flash_control1(void)
{
   
    flash_config_t flash_con1;
    //memset (&flash_con1, 0, sizeof (flash_con1));
	int32_t result = 0;
    result = FLASH_Init(&flash_con1);
    if(result != kStatus_FLASH_Success)
    {
        error_trap();
    }
//    for (int i = 0; i < BUFFER_LEN; i++)
//        {
//            S_buffer[i] = i;
//        }
//     result = FLASH_Program(&flash_con1, 255*2048, S_buffer, sizeof(S_buffer));
//        if (kStatus_FLASH_Success != result)
//        {
//            error_trap();
//        }
    for(int i = 0; i < 4; i++)
    {
        DianGan__Buffer[i] = *(volatile uint32_t *)(255 * 2048 + i * 4);
    }
    
    // FLASH_Erase(&flash_con1, 255 * 2048, sizeof(S_buffer), kFLASH_ApiEraseKey);

}
*****/
uint8_t read;
flash_config_t flash_con1;
void Flash_Init(void)
{
	FLASH_Init(&flash_con1);
}
void Flash_Heng(void)
{
	if(0==Dial0)   read='R';
    if(1==Dial0)   read='W';
	
	switch(read)
    {
		case 'W':
			FLASH_Erase(&flash_con1, 255 * 2048, sizeof(S_buffer), kFLASH_ApiEraseKey); 
			
			GetHengMaxMin();
		
			S_buffer[0]=GetADMax[0]/100;
			S_buffer[1]=GetADMax[0]%100;
			S_buffer[2]=GetADMax[1]/100;
			S_buffer[3]=GetADMax[1]%100;
			S_buffer[4]=GetADMax[2]/100;
			S_buffer[5]=GetADMax[2]%100;

			S_buffer[6]=GetADMin[0]/100;
			S_buffer[7]=GetADMin[0]%100;
			S_buffer[8]=GetADMin[1]/100;
			S_buffer[9]=GetADMin[1]%100;
			S_buffer[10]=GetADMin[2]/100;
			S_buffer[11]=GetADMin[2]%100;
			
			FLASH_Program(&flash_con1, 255*2048, S_buffer, sizeof(S_buffer));
		break;
		case 'R':
			for(int i = 0; i < 12; i++)//i: 0..11
			{
				DianGan__Buffer[i] = *(volatile uint32_t *)(255 * 2048 + i * 4);
			}
			
			GetADMax[0]= DianGan__Buffer[0]*100+DianGan__Buffer[1];
			GetADMax[1]= DianGan__Buffer[2]*100+DianGan__Buffer[3];
			GetADMax[2]= DianGan__Buffer[4]*100+DianGan__Buffer[5];		
			
			GetADMin[0]= DianGan__Buffer[6]*100+DianGan__Buffer[7];
			GetADMin[1]= DianGan__Buffer[8]*100+DianGan__Buffer[9];
			GetADMin[2]= DianGan__Buffer[10]*100+DianGan__Buffer[11];
		break;  
		default:break;
    }
}


void Flash_Shu(void)
{
	if(0==Dial1)   read='R';//du
    if(1==Dial1)   read='W';//xie
	
	switch(read)
    {
		case 'W': 
			FLASH_Erase(&flash_con1, 255 * 2048, sizeof(S_buffer), kFLASH_ApiEraseKey); 
		
			GetShuMaxMin();
			
			S_buffer[12]=GetADMax[3]/100;
			S_buffer[13]=GetADMax[3]%100;
			S_buffer[14]=GetADMax[4]/100;
			S_buffer[15]=GetADMax[4]%100;

			S_buffer[16]=GetADMin[3]/100;
			S_buffer[17]=GetADMin[3]%100;
			S_buffer[18]=GetADMin[4]/100;
			S_buffer[19]=GetADMin[4]%100;
		
			S_buffer[20]=GetADMax[5]/100;
			S_buffer[21]=GetADMax[5]%100;
			S_buffer[22]=GetADMin[5]/100;
			S_buffer[23]=GetADMin[5]%100;
			FLASH_Program(&flash_con1, 255*2048, S_buffer, sizeof(S_buffer));
		break;
		case 'R':
			for(int i = 12; i < 24; i++)
			{
				DianGan__Buffer[i] = *(volatile uint32_t *)(255 * 2048 + i * 4);
			}
			GetADMax[3] = DianGan__Buffer[12]*100+DianGan__Buffer[13];
			GetADMax[4] = DianGan__Buffer[14]*100+DianGan__Buffer[15];
			
			GetADMin[3] = DianGan__Buffer[16]*100+DianGan__Buffer[17];
			GetADMin[4] = DianGan__Buffer[18]*100+DianGan__Buffer[19];
			GetADMax[5] = DianGan__Buffer[20]*100+DianGan__Buffer[21];
			GetADMin[5] = DianGan__Buffer[22]*100+DianGan__Buffer[23];
		break;
		default:break;	
	}
}

