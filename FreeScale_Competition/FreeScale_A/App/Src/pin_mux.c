/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
PinsProfile:
- !!product 'Pins v2.0'
- !!processor 'MK60DN512xxx10'
- !!package 'MK60DN512VLQ10'
- !!mcu_data 'ksdk2_0'
- !!processor_version '1.2.0'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PCR_DSE_HIGH                  0x01u   /*!< Drive Strength Enable: High drive strength is configured on the corresponding pin, if pin is configured as a digital output. */
#define PCR_PFE_ENABLED               0x01u   /*!< Passive Filter Enable: Passive input filter is enabled on the corresponding pin, if the pin is configured as a digital input. A low pass filter (10 MHz to 30 MHz bandwidth) is enabled on the digital input path. Disable the passive input filter when supporting high speed interfaces (> 2 MHz) on the pin. */
#define PCR_PS_UP                     0x01u   /*!< Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding Port Pull Enable Register field is set. */
#define PCR_SRE_FAST                  0x00u   /*!< Slew Rate Enable: Fast slew rate is configured on the corresponding pin, if the pin is configured as a digital output. */
#define PIN0_IDX                         0u   /*!< Pin number for pin 0 in a port */
#define PIN1_IDX                         1u   /*!< Pin number for pin 1 in a port */
#define PIN2_IDX                         2u   /*!< Pin number for pin 2 in a port */
#define PIN3_IDX                         3u   /*!< Pin number for pin 3 in a port */
#define PIN4_IDX                         4u   /*!< Pin number for pin 4 in a port */
#define PIN5_IDX                         5u   /*!< Pin number for pin 5 in a port */
#define PIN6_IDX                         6u   /*!< Pin number for pin 6 in a port */
#define PIN7_IDX                         7u   /*!< Pin number for pin 7 in a port */
#define PIN8_IDX                         8u   /*!< Pin number for pin 8 in a port */
#define PIN9_IDX                         9u   /*!< Pin number for pin 9 in a port */
#define PIN10_IDX                       10u   /*!< Pin number for pin 10 in a port */
#define PIN11_IDX                       11u   /*!< Pin number for pin 11 in a port */
#define PIN12_IDX                       12u   /*!< Pin number for pin 12 in a port */
#define PIN13_IDX                       13u   /*!< Pin number for pin 13 in a port */
#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */
#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define PIN18_IDX                       18u   /*!< Pin number for pin 18 in a port */
#define PIN19_IDX                       19u   /*!< Pin number for pin 19 in a port */
#define PIN20_IDX                       20u   /*!< Pin number for pin 20 in a port */
#define PIN21_IDX                       21u   /*!< Pin number for pin 21 in a port */
#define PIN22_IDX                       22u   /*!< Pin number for pin 22 in a port */
#define PIN23_IDX                       23u   /*!< Pin number for pin 23 in a port */
#define PIN24_IDX                       24u   /*!< Pin number for pin 24 in a port */
#define PIN25_IDX                       25u   /*!< Pin number for pin 25 in a port */
#define PIN26_IDX                       26u   /*!< Pin number for pin 26 in a port */
#define PIN27_IDX                       27u   /*!< Pin number for pin 27 in a port */
#define PIN28_IDX                       28u   /*!< Pin number for pin 28 in a port */
#define PIN29_IDX                       29u   /*!< Pin number for pin 29 in a port */
#define PORT_DFER_DFE_1_MASK          0x02u   /*!< Digital Filter Enable Mask for item 1. */
#define PORT_DFER_DFE_2_MASK          0x04u   /*!< Digital Filter Enable Mask for item 2. */
#define PORT_DFER_DFE_3_MASK          0x08u   /*!< Digital Filter Enable Mask for item 3. */
#define PORT_DFER_DFE_4_MASK          0x10u   /*!< Digital Filter Enable Mask for item 4. */
#define PORT_DFER_DFE_7_MASK          0x80u   /*!< Digital Filter Enable Mask for item 7. */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '50', peripheral: JTAG, signal: SWD_CLK, pin_signal: TSI0_CH1/PTA0/UART0_CTS_b/UART0_COL_b/FTM0_CH5/JTAG_TCLK/SWD_CLK/EZP_CLK, slew_rate: no_init}
  - {pin_num: '53', peripheral: JTAG, signal: SWD_DIO, pin_signal: TSI0_CH4/PTA3/UART0_RTS_b/FTM0_CH0/JTAG_TMS/SWD_DIO, slew_rate: no_init}
  - {pin_num: '123', peripheral: GPIOC, signal: 'GPIO, 16', pin_signal: PTC16/CAN1_RX/UART3_RX/ENET0_1588_TMR0/FB_CS5_b/FB_TSIZ1/FB_BE23_16_b, slew_rate: no_init,
    pull_select: no_init}
  - {pin_num: '124', peripheral: GPIOC, signal: 'GPIO, 17', pin_signal: PTC17/CAN1_TX/UART3_TX/ENET0_1588_TMR1/FB_CS4_b/FB_TSIZ0/FB_BE31_24_b, slew_rate: no_init,
    pull_select: no_init}
  - {pin_num: '113', peripheral: GPIOC, signal: 'GPIO, 8', pin_signal: ADC1_SE4b/CMP0_IN2/PTC8/I2S0_MCLK/FB_AD7, slew_rate: no_init, pull_select: no_init}
  - {pin_num: '114', peripheral: GPIOC, signal: 'GPIO, 9', pin_signal: ADC1_SE5b/CMP0_IN3/PTC9/I2S0_RX_BCLK/FB_AD6/FTM2_FLT0, slew_rate: no_init, pull_select: no_init}
  - {pin_num: '115', peripheral: GPIOC, signal: 'GPIO, 10', pin_signal: ADC1_SE6b/PTC10/I2C1_SCL/I2S0_RX_FS/FB_AD5, slew_rate: no_init, pull_select: no_init}
  - {pin_num: '116', peripheral: GPIOC, signal: 'GPIO, 11', pin_signal: ADC1_SE7b/PTC11/LLWU_P11/I2C1_SDA/I2S0_RXD1/FB_RW_b, slew_rate: no_init, pull_select: no_init}
  - {pin_num: '117', peripheral: GPIOC, signal: 'GPIO, 12', pin_signal: PTC12/UART4_RTS_b/FB_AD27, slew_rate: no_init, pull_select: no_init}
  - {pin_num: '118', peripheral: GPIOC, signal: 'GPIO, 13', pin_signal: PTC13/UART4_CTS_b/FB_AD26, slew_rate: no_init, pull_select: no_init}
  - {pin_num: '119', peripheral: GPIOC, signal: 'GPIO, 14', pin_signal: PTC14/UART4_RX/FB_AD25, slew_rate: no_init, pull_select: no_init}
  - {pin_num: '120', peripheral: GPIOC, signal: 'GPIO, 15', pin_signal: PTC15/UART4_TX/FB_AD24, slew_rate: no_init, pull_select: no_init}
  - {pin_num: '111', peripheral: GPIOC, signal: 'GPIO, 6', pin_signal: CMP0_IN0/PTC6/LLWU_P10/SPI0_SOUT/PDB0_EXTRG/I2S0_RX_BCLK/FB_AD9/I2S0_MCLK, slew_rate: no_init}
  - {pin_num: '112', peripheral: GPIOC, signal: 'GPIO, 7', pin_signal: CMP0_IN1/PTC7/SPI0_SIN/USB_SOF_OUT/I2S0_RX_FS/FB_AD8, slew_rate: no_init, pull_select: up}
  - {pin_num: '99', peripheral: GPIOB, signal: 'GPIO, 20', pin_signal: PTB20/SPI2_PCS0/FB_AD31/CMP0_OUT, slew_rate: no_init}
  - {pin_num: '100', peripheral: GPIOB, signal: 'GPIO, 21', pin_signal: PTB21/SPI2_SCK/FB_AD30/CMP1_OUT, slew_rate: no_init}
  - {pin_num: '101', peripheral: GPIOB, signal: 'GPIO, 22', pin_signal: PTB22/SPI2_SOUT/FB_AD29/CMP2_OUT, slew_rate: no_init}
  - {pin_num: '102', peripheral: GPIOB, signal: 'GPIO, 23', pin_signal: PTB23/SPI2_SIN/SPI0_PCS5/FB_AD28, slew_rate: no_init}
  - {pin_num: '89', peripheral: GPIOB, signal: 'GPIO, 8', pin_signal: PTB8/UART3_RTS_b/FB_AD21, slew_rate: no_init}
  - {pin_num: '90', peripheral: GPIOB, signal: 'GPIO, 9', pin_signal: PTB9/SPI1_PCS1/UART3_CTS_b/FB_AD20, slew_rate: no_init}
  - {pin_num: '77', peripheral: GPIOA, signal: 'GPIO, 26', pin_signal: PTA26/MII0_TXD3/FB_A27, slew_rate: no_init}
  - {pin_num: '78', peripheral: GPIOA, signal: 'GPIO, 27', pin_signal: PTA27/MII0_CRS/FB_A26, slew_rate: no_init}
  - {pin_num: '79', peripheral: GPIOA, signal: 'GPIO, 28', pin_signal: PTA28/MII0_TXER/FB_A25, slew_rate: no_init}
  - {pin_num: '80', peripheral: GPIOA, signal: 'GPIO, 29', pin_signal: PTA29/MII0_COL/FB_A24, slew_rate: no_init}
  - {pin_num: '97', peripheral: FTM2, signal: 'QD_PH, A', pin_signal: TSI0_CH11/PTB18/CAN0_TX/FTM2_CH0/I2S0_TX_BCLK/FB_AD15/FTM2_QD_PHA}
  - {pin_num: '98', peripheral: FTM2, signal: 'QD_PH, B', pin_signal: TSI0_CH12/PTB19/CAN0_RX/FTM2_CH1/I2S0_TX_FS/FB_OE_b/FTM2_QD_PHB, slew_rate: fast}
  - {pin_num: '104', peripheral: FTM0, signal: 'CH, 0', pin_signal: ADC0_SE15/TSI0_CH14/PTC1/LLWU_P6/SPI0_PCS3/UART1_RTS_b/FTM0_CH0/FB_AD13/I2S0_TXD0, slew_rate: fast,
    pull_select: no_init, passive_filter: enable, digital_filter: enable, drive_strength: high}
  - {pin_num: '105', peripheral: FTM0, signal: 'CH, 1', pin_signal: ADC0_SE4b/CMP1_IN0/TSI0_CH15/PTC2/SPI0_PCS2/UART1_CTS_b/FTM0_CH1/FB_AD12/I2S0_TX_FS, slew_rate: fast,
    pull_select: no_init, passive_filter: enable, digital_filter: enable, drive_strength: high}
  - {pin_num: '106', peripheral: FTM0, signal: 'CH, 2', pin_signal: CMP1_IN1/PTC3/LLWU_P7/SPI0_PCS1/UART1_RX/FTM0_CH2/CLKOUT/I2S0_TX_BCLK, slew_rate: fast, pull_select: no_init,
    passive_filter: enable, digital_filter: enable, drive_strength: high}
  - {pin_num: '109', peripheral: FTM0, signal: 'CH, 3', pin_signal: PTC4/LLWU_P8/SPI0_PCS0/UART1_TX/FTM0_CH3/FB_AD11/CMP1_OUT, slew_rate: fast, pull_select: no_init,
    passive_filter: enable, digital_filter: enable, drive_strength: high}
  - {pin_num: '85', peripheral: ADC1, signal: 'SE, 10', pin_signal: ADC1_SE10/PTB4/ENET0_1588_TMR2/FTM1_FLT0}
  - {pin_num: '86', peripheral: ADC1, signal: 'SE, 11', pin_signal: ADC1_SE11/PTB5/ENET0_1588_TMR3/FTM2_FLT0}
  - {pin_num: '87', peripheral: ADC1, signal: 'SE, 12', pin_signal: ADC1_SE12/PTB6/FB_AD23}
  - {pin_num: '88', peripheral: ADC1, signal: 'SE, 13', pin_signal: ADC1_SE13/PTB7/FB_AD22}
  - {pin_num: '91', peripheral: ADC1, signal: 'SE, 14', pin_signal: ADC1_SE14/PTB10/SPI1_PCS0/UART3_RX/FB_AD19/FTM0_FLT1}
  - {pin_num: '92', peripheral: ADC1, signal: 'SE, 15', pin_signal: ADC1_SE15/PTB11/SPI1_SCK/UART3_TX/FB_AD18/FTM0_FLT2}
  - {pin_num: '45', peripheral: UART4, signal: TX, pin_signal: ADC0_SE17/EXTAL1/PTE24/CAN1_TX/UART4_TX/EWM_OUT_b}
  - {pin_num: '46', peripheral: UART4, signal: RX, pin_signal: ADC0_SE18/XTAL1/PTE25/CAN1_RX/UART4_RX/EWM_IN}
  - {pin_num: '54', peripheral: GPIOA, signal: 'GPIO, 4', pin_signal: TSI0_CH5/PTA4/LLWU_P3/FTM0_CH1/NMI_b/EZP_CS_b, slew_rate: no_init}
  - {pin_num: '55', peripheral: GPIOA, signal: 'GPIO, 5', pin_signal: PTA5/USB_CLKIN/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/I2S0_TX_BCLK/JTAG_TRST_b, slew_rate: no_init}
  - {pin_num: '64', peripheral: GPIOA, signal: 'GPIO, 12', pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/I2S0_TXD0/FTM1_QD_PHA, slew_rate: no_init}
  - {pin_num: '67', peripheral: GPIOA, signal: 'GPIO, 15', pin_signal: PTA15/SPI0_SCK/UART0_RX/RMII0_TXEN/MII0_TXEN/I2S0_RXD0, slew_rate: no_init}
  - {pin_num: '136', peripheral: CMT, signal: IRO, pin_signal: PTD7/CMT_IRO/UART0_TX/FTM0_CH7/FTM0_FLT1, passive_filter: enable, digital_filter: enable, drive_strength: high}
  - {pin_num: '95', peripheral: GPIOB, signal: 'GPIO, 16', pin_signal: TSI0_CH9/PTB16/SPI1_SOUT/UART0_RX/FB_AD17/EWM_IN}
  - {pin_num: '96', peripheral: GPIOB, signal: 'GPIO, 17', pin_signal: TSI0_CH10/PTB17/SPI1_SIN/UART0_TX/FB_AD16/EWM_OUT_b}
  - {pin_num: '81', peripheral: FTM1, signal: 'QD_PH, A', pin_signal: ADC0_SE8/ADC1_SE8/TSI0_CH0/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/RMII0_MDIO/MII0_MDIO/FTM1_QD_PHA}
  - {pin_num: '82', peripheral: FTM1, signal: 'QD_PH, B', pin_signal: ADC0_SE9/ADC1_SE9/TSI0_CH6/PTB1/I2C0_SDA/FTM1_CH1/RMII0_MDC/MII0_MDC/FTM1_QD_PHB}
  - {pin_num: '132', peripheral: GPIOD, signal: 'GPIO, 5', pin_signal: ADC0_SE6b/PTD5/SPI0_PCS2/UART0_CTS_b/UART0_COL_b/FTM0_CH5/FB_AD1/EWM_OUT_b}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortA);                           /* Port A Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortD);                           /* Port D Clock Gate Control: Clock enabled */
  CLOCK_EnableClock(kCLOCK_PortE);                           /* Port E Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTA, PIN0_IDX, kPORT_MuxAlt7);            /* PORTA0 (pin 50) is configured as SWD_CLK */
  PORT_SetPinMux(PORTA, PIN12_IDX, kPORT_MuxAsGpio);         /* PORTA12 (pin 64) is configured as PTA12 */
  PORT_SetPinMux(PORTA, PIN15_IDX, kPORT_MuxAsGpio);         /* PORTA15 (pin 67) is configured as PTA15 */
  PORT_SetPinMux(PORTA, PIN26_IDX, kPORT_MuxAsGpio);         /* PORTA26 (pin 77) is configured as PTA26 */
  PORT_SetPinMux(PORTA, PIN27_IDX, kPORT_MuxAsGpio);         /* PORTA27 (pin 78) is configured as PTA27 */
  PORT_SetPinMux(PORTA, PIN28_IDX, kPORT_MuxAsGpio);         /* PORTA28 (pin 79) is configured as PTA28 */
  PORT_SetPinMux(PORTA, PIN29_IDX, kPORT_MuxAsGpio);         /* PORTA29 (pin 80) is configured as PTA29 */
  PORT_SetPinMux(PORTA, PIN3_IDX, kPORT_MuxAlt7);            /* PORTA3 (pin 53) is configured as SWD_DIO */
  PORT_SetPinMux(PORTA, PIN4_IDX, kPORT_MuxAsGpio);          /* PORTA4 (pin 54) is configured as PTA4 */
  PORT_SetPinMux(PORTA, PIN5_IDX, kPORT_MuxAsGpio);          /* PORTA5 (pin 55) is configured as PTA5 */
  PORT_SetPinMux(PORTB, PIN0_IDX, kPORT_MuxAlt6);            /* PORTB0 (pin 81) is configured as FTM1_QD_PHA */
  PORT_SetPinMux(PORTB, PIN1_IDX, kPORT_MuxAlt6);            /* PORTB1 (pin 82) is configured as FTM1_QD_PHB */
  PORT_SetPinMux(PORTB, PIN10_IDX, kPORT_PinDisabledOrAnalog); /* PORTB10 (pin 91) is configured as ADC1_SE14 */
  PORT_SetPinMux(PORTB, PIN11_IDX, kPORT_PinDisabledOrAnalog); /* PORTB11 (pin 92) is configured as ADC1_SE15 */
  PORT_SetPinMux(PORTB, PIN16_IDX, kPORT_MuxAsGpio);         /* PORTB16 (pin 95) is configured as PTB16 */
  PORT_SetPinMux(PORTB, PIN17_IDX, kPORT_MuxAsGpio);         /* PORTB17 (pin 96) is configured as PTB17 */
  PORT_SetPinMux(PORTB, PIN18_IDX, kPORT_MuxAlt6);           /* PORTB18 (pin 97) is configured as FTM2_QD_PHA */
  PORT_SetPinMux(PORTB, PIN19_IDX, kPORT_MuxAlt6);           /* PORTB19 (pin 98) is configured as FTM2_QD_PHB */
  PORTB->PCR[19] = ((PORTB->PCR[19] &
    (~(PORT_PCR_SRE_MASK | PORT_PCR_ISF_MASK)))              /* Mask bits to zero which are setting */
      | PORT_PCR_SRE(PCR_SRE_FAST)                           /* Slew Rate Enable: Fast slew rate is configured on the corresponding pin, if the pin is configured as a digital output. */
    );
  PORT_SetPinMux(PORTB, PIN20_IDX, kPORT_MuxAsGpio);         /* PORTB20 (pin 99) is configured as PTB20 */
  PORT_SetPinMux(PORTB, PIN21_IDX, kPORT_MuxAsGpio);         /* PORTB21 (pin 100) is configured as PTB21 */
  PORT_SetPinMux(PORTB, PIN22_IDX, kPORT_MuxAsGpio);         /* PORTB22 (pin 101) is configured as PTB22 */
  PORT_SetPinMux(PORTB, PIN23_IDX, kPORT_MuxAsGpio);         /* PORTB23 (pin 102) is configured as PTB23 */
  PORT_SetPinMux(PORTB, PIN4_IDX, kPORT_PinDisabledOrAnalog); /* PORTB4 (pin 85) is configured as ADC1_SE10 */
  PORT_SetPinMux(PORTB, PIN5_IDX, kPORT_PinDisabledOrAnalog); /* PORTB5 (pin 86) is configured as ADC1_SE11 */
  PORT_SetPinMux(PORTB, PIN6_IDX, kPORT_PinDisabledOrAnalog); /* PORTB6 (pin 87) is configured as ADC1_SE12 */
  PORT_SetPinMux(PORTB, PIN7_IDX, kPORT_PinDisabledOrAnalog); /* PORTB7 (pin 88) is configured as ADC1_SE13 */
  PORT_SetPinMux(PORTB, PIN8_IDX, kPORT_MuxAsGpio);          /* PORTB8 (pin 89) is configured as PTB8 */
  PORT_SetPinMux(PORTB, PIN9_IDX, kPORT_MuxAsGpio);          /* PORTB9 (pin 90) is configured as PTB9 */
  PORT_EnablePinsDigitalFilter(                              /* Configure digital filter */
    PORTC,                                                   /* Digital filter is configured on port C */
      PORT_DFER_DFE_1_MASK                                   /* Digital filter is configured for PORTC0 */
    | PORT_DFER_DFE_2_MASK                                   /* Digital filter is configured for PORTC1 */
    | PORT_DFER_DFE_3_MASK                                   /* Digital filter is configured for PORTC2 */
    | PORT_DFER_DFE_4_MASK,                                  /* Digital filter is configured for PORTC3 */
    true                                                     /* Enable digital filter */
  );
  PORT_SetPinMux(PORTC, PIN1_IDX, kPORT_MuxAlt4);            /* PORTC1 (pin 104) is configured as FTM0_CH0 */
  PORTC->PCR[1] = ((PORTC->PCR[1] &
    (~(PORT_PCR_SRE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_SRE(PCR_SRE_FAST)                           /* Slew Rate Enable: Fast slew rate is configured on the corresponding pin, if the pin is configured as a digital output. */
      | PORT_PCR_PFE(PCR_PFE_ENABLED)                        /* Passive Filter Enable: Passive input filter is enabled on the corresponding pin, if the pin is configured as a digital input. A low pass filter (10 MHz to 30 MHz bandwidth) is enabled on the digital input path. Disable the passive input filter when supporting high speed interfaces (> 2 MHz) on the pin. */
      | PORT_PCR_DSE(PCR_DSE_HIGH)                           /* Drive Strength Enable: High drive strength is configured on the corresponding pin, if pin is configured as a digital output. */
    );
  PORT_SetPinMux(PORTC, PIN10_IDX, kPORT_MuxAsGpio);         /* PORTC10 (pin 115) is configured as PTC10 */
  PORT_SetPinMux(PORTC, PIN11_IDX, kPORT_MuxAsGpio);         /* PORTC11 (pin 116) is configured as PTC11 */
  PORT_SetPinMux(PORTC, PIN12_IDX, kPORT_MuxAsGpio);         /* PORTC12 (pin 117) is configured as PTC12 */
  PORT_SetPinMux(PORTC, PIN13_IDX, kPORT_MuxAsGpio);         /* PORTC13 (pin 118) is configured as PTC13 */
  PORT_SetPinMux(PORTC, PIN14_IDX, kPORT_MuxAsGpio);         /* PORTC14 (pin 119) is configured as PTC14 */
  PORT_SetPinMux(PORTC, PIN15_IDX, kPORT_MuxAsGpio);         /* PORTC15 (pin 120) is configured as PTC15 */
  PORT_SetPinMux(PORTC, PIN16_IDX, kPORT_MuxAsGpio);         /* PORTC16 (pin 123) is configured as PTC16 */
  PORT_SetPinMux(PORTC, PIN17_IDX, kPORT_MuxAsGpio);         /* PORTC17 (pin 124) is configured as PTC17 */
  PORT_SetPinMux(PORTC, PIN2_IDX, kPORT_MuxAlt4);            /* PORTC2 (pin 105) is configured as FTM0_CH1 */
  PORTC->PCR[2] = ((PORTC->PCR[2] &
    (~(PORT_PCR_SRE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_SRE(PCR_SRE_FAST)                           /* Slew Rate Enable: Fast slew rate is configured on the corresponding pin, if the pin is configured as a digital output. */
      | PORT_PCR_PFE(PCR_PFE_ENABLED)                        /* Passive Filter Enable: Passive input filter is enabled on the corresponding pin, if the pin is configured as a digital input. A low pass filter (10 MHz to 30 MHz bandwidth) is enabled on the digital input path. Disable the passive input filter when supporting high speed interfaces (> 2 MHz) on the pin. */
      | PORT_PCR_DSE(PCR_DSE_HIGH)                           /* Drive Strength Enable: High drive strength is configured on the corresponding pin, if pin is configured as a digital output. */
    );
  PORT_SetPinMux(PORTC, PIN3_IDX, kPORT_MuxAlt4);            /* PORTC3 (pin 106) is configured as FTM0_CH2 */
  PORTC->PCR[3] = ((PORTC->PCR[3] &
    (~(PORT_PCR_SRE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_SRE(PCR_SRE_FAST)                           /* Slew Rate Enable: Fast slew rate is configured on the corresponding pin, if the pin is configured as a digital output. */
      | PORT_PCR_PFE(PCR_PFE_ENABLED)                        /* Passive Filter Enable: Passive input filter is enabled on the corresponding pin, if the pin is configured as a digital input. A low pass filter (10 MHz to 30 MHz bandwidth) is enabled on the digital input path. Disable the passive input filter when supporting high speed interfaces (> 2 MHz) on the pin. */
      | PORT_PCR_DSE(PCR_DSE_HIGH)                           /* Drive Strength Enable: High drive strength is configured on the corresponding pin, if pin is configured as a digital output. */
    );
  PORT_SetPinMux(PORTC, PIN4_IDX, kPORT_MuxAlt4);            /* PORTC4 (pin 109) is configured as FTM0_CH3 */
  PORTC->PCR[4] = ((PORTC->PCR[4] &
    (~(PORT_PCR_SRE_MASK | PORT_PCR_PFE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_SRE(PCR_SRE_FAST)                           /* Slew Rate Enable: Fast slew rate is configured on the corresponding pin, if the pin is configured as a digital output. */
      | PORT_PCR_PFE(PCR_PFE_ENABLED)                        /* Passive Filter Enable: Passive input filter is enabled on the corresponding pin, if the pin is configured as a digital input. A low pass filter (10 MHz to 30 MHz bandwidth) is enabled on the digital input path. Disable the passive input filter when supporting high speed interfaces (> 2 MHz) on the pin. */
      | PORT_PCR_DSE(PCR_DSE_HIGH)                           /* Drive Strength Enable: High drive strength is configured on the corresponding pin, if pin is configured as a digital output. */
    );
  PORT_SetPinMux(PORTC, PIN6_IDX, kPORT_MuxAsGpio);          /* PORTC6 (pin 111) is configured as PTC6 */
  PORT_SetPinMux(PORTC, PIN7_IDX, kPORT_MuxAsGpio);          /* PORTC7 (pin 112) is configured as PTC7 */
  PORTC->PCR[7] = ((PORTC->PCR[7] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK)))               /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding Port Pull Enable Register field is set. */
    );
  PORT_SetPinMux(PORTC, PIN8_IDX, kPORT_MuxAsGpio);          /* PORTC8 (pin 113) is configured as PTC8 */
  PORT_SetPinMux(PORTC, PIN9_IDX, kPORT_MuxAsGpio);          /* PORTC9 (pin 114) is configured as PTC9 */
  PORT_EnablePinsDigitalFilter(                              /* Configure digital filter */
    PORTD,                                                   /* Digital filter is configured on port D */
    PORT_DFER_DFE_7_MASK,                                    /* Digital filter is configured for PORTD0 */
    true                                                     /* Enable digital filter */
  );
  PORT_SetPinMux(PORTD, PIN5_IDX, kPORT_MuxAsGpio);          /* PORTD5 (pin 132) is configured as PTD5 */
  PORT_SetPinMux(PORTD, PIN7_IDX, kPORT_MuxAlt2);            /* PORTD7 (pin 136) is configured as CMT_IRO */
  PORTD->PCR[7] = ((PORTD->PCR[7] &
    (~(PORT_PCR_PFE_MASK | PORT_PCR_DSE_MASK | PORT_PCR_ISF_MASK))) /* Mask bits to zero which are setting */
      | PORT_PCR_PFE(PCR_PFE_ENABLED)                        /* Passive Filter Enable: Passive input filter is enabled on the corresponding pin, if the pin is configured as a digital input. A low pass filter (10 MHz to 30 MHz bandwidth) is enabled on the digital input path. Disable the passive input filter when supporting high speed interfaces (> 2 MHz) on the pin. */
      | PORT_PCR_DSE(PCR_DSE_HIGH)                           /* Drive Strength Enable: High drive strength is configured on the corresponding pin, if pin is configured as a digital output. */
    );
  PORT_SetPinMux(PORTE, PIN24_IDX, kPORT_MuxAlt3);           /* PORTE24 (pin 45) is configured as UART4_TX */
  PORT_SetPinMux(PORTE, PIN25_IDX, kPORT_MuxAlt3);           /* PORTE25 (pin 46) is configured as UART4_RX */

	
//2401_FA
 PORT_SetPinMux(PORTD, PIN14_IDX, kPORT_MuxAsGpio);         /* PORTD14 (pin 143) is configured as PTD14 */
  PORTD->PCR[14] = ((PORTD->PCR[14] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK)))               /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding Port Pull Enable Register field is set. */
    );
  PORT_SetPinMux(PORTD, PIN15_IDX, kPORT_MuxAsGpio);         /* PORTD15 (pin 144) is configured as PTD15 */
  PORTD->PCR[15] = ((PORTD->PCR[15] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK)))               /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding Port Pull Enable Register field is set. */
    );
	
	PORT_SetPinMux(PORTD, PIN10_IDX, kPORT_MuxAsGpio);         /* PORTD10 (pin 139) is configured as PTD10 */
  PORT_SetPinMux(PORTD, PIN11_IDX, kPORT_MuxAsGpio);         /* PORTD11 (pin 140) is configured as PTD11 */
  PORT_SetPinMux(PORTD, PIN12_IDX, kPORT_MuxAsGpio);         /* PORTD12 (pin 141) is configured as PTD12 */
  PORT_SetPinMux(PORTD, PIN13_IDX, kPORT_MuxAsGpio);         /* PORTD13 (pin 142) is configured as PTD13 */
//2401_SHOU
  PORT_SetPinMux(PORTE, PIN4_IDX, kPORT_MuxAsGpio);          /* PORTE4 (pin 7) is configured as PTE4 */
  PORTE->PCR[4] = ((PORTE->PCR[4] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK)))               /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding Port Pull Enable Register field is set. */
    );
  PORT_SetPinMux(PORTE, PIN5_IDX, kPORT_MuxAsGpio);          /* PORTE5 (pin 8) is configured as PTE5 */
  PORTE->PCR[5] = ((PORTE->PCR[5] &
    (~(PORT_PCR_PS_MASK | PORT_PCR_ISF_MASK)))               /* Mask bits to zero which are setting */
      | PORT_PCR_PS(PCR_PS_UP)                               /* Pull Select: Internal pullup resistor is enabled on the corresponding pin, if the corresponding Port Pull Enable Register field is set. */
    );
	
 PORT_SetPinMux(PORTE, PIN0_IDX, kPORT_MuxAsGpio);          /* PORTE0 (pin 1) is configured as PTE0 */
  PORT_SetPinMux(PORTE, PIN1_IDX, kPORT_MuxAsGpio);          /* PORTE1 (pin 2) is configured as PTE1 */
  PORT_SetPinMux(PORTE, PIN2_IDX, kPORT_MuxAsGpio);          /* PORTE2 (pin 3) is configured as PTE2 */
  PORT_SetPinMux(PORTE, PIN3_IDX, kPORT_MuxAsGpio);          /* PORTE3 (pin 4) is configured as PTE3 */
  PORT_SetPinMux(PORTE, PIN10_IDX, kPORT_MuxAsGpio);         /* PORTE10 (pin 13) is configured as PTE10 */
  PORT_SetPinMux(PORTE, PIN9_IDX, kPORT_MuxAsGpio);          /* PORTE9 (pin 12) is configured as PTE9 */
  
}

/*******************************************************************************
 * EOF
 ******************************************************************************/
