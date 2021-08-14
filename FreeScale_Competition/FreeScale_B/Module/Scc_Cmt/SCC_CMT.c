#include "scc_cmt.h"

volatile  uint16_t temp_num;

//CMT函数初始化  默认引脚PTD7不能修改  频率50-300应该没问题  占空比设置0-1000
void SCC_CMT_Init(uint16_t freq, uint16_t duty)
{
    uint32_t temp_clk;
    uint32_t  temp_high_num, temp_low_num;
    uint32_t temp_div;
    uint32_t bus_clk_mhz=100;
    
    SIM->SCGC4 |= SIM_SCGC4_CMT_MASK;
    
    CMT->OC |= CMT_OC_IROPEN_MASK;
    
//    bus_clk_mhz = (50 * ((MCG->C6 & MCG_C6_VDIV_MASK) + 24) / ((MCG->C5 & MCG_C5_PRDIV_MASK) + 1)) 
//                  / (((SIM->CLKDIV1 >> SIM_CLKDIV1_OUTDIV2_SHIFT) & 0x0f) + 1);
    
    bus_clk_mhz = CLOCK_GetFreq(kCLOCK_BusClk);//CLOCK_GetOutClkFreq() / ((uint32_t)((SIM->CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> SIM_CLKDIV1_OUTDIV2_SHIFT)+ 1u);
    
    temp_clk = bus_clk_mhz/**1000*1000*//8;
    
    temp_div = temp_clk/freq;
    temp_div = temp_div>>16;
    if(temp_div>0x0f)   temp_div = 0x0f;
    
    CMT->PPS = CMT_PPS_PPSDIV(temp_div);
    
    temp_num = temp_clk/(temp_div+1)/freq;
    
    temp_low_num = (temp_num*(CMT_PRECISON-duty)/CMT_PRECISON);
    temp_high_num = (temp_num*(duty)/CMT_PRECISON);
    
    temp_low_num--;
    CMT->CMD1 = temp_low_num >> 8;
    CMT->CMD2 = (uint8_t)temp_low_num;
    
    CMT->CMD3 = temp_high_num >> 8;
    CMT->CMD4 = (uint8_t)temp_high_num;
     
    CMT->MSC = CMT_MSC_BASE_MASK | CMT_MSC_MCGEN_MASK;
}

//占空比改变  0-1000
void SCC_CMT_Duty(uint16_t duty)
{
    uint32_t /*temp_num,*/ temp_high_num, temp_low_num;
    /*uint32 temp_data;*/
    
//    temp_data = CMT->CMD1;
//    temp_low_num = (temp_data << 8) | CMT->CMD1;
//    
//    temp_low_num++;    
//    
//    temp_data = CMT->CMD3;
//    temp_high_num = (temp_data << 8) | CMT->CMD4;
//    
//    temp_num = temp_high_num + temp_low_num;
    
    temp_low_num = (temp_num*(CMT_PRECISON-duty)/CMT_PRECISON);
    temp_high_num = (temp_num*(duty)/CMT_PRECISON);
    
    temp_low_num--;
    CMT->CMD1 = temp_low_num >> 8;
    CMT->CMD2 = (uint8_t)temp_low_num;
    
    CMT->CMD3 = temp_high_num >> 8;
    CMT->CMD4 = (uint8_t)temp_high_num;
}
