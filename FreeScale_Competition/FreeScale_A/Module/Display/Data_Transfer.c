#include "data_transfer.h"
#include "init.h"

#define BYTE0(dwTemp)       ( *( (u8 *)(&dwTemp)	) )
#define BYTE1(dwTemp)       ( *( (u8 *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (u8 *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (u8 *)(&dwTemp) + 3) )



void ANO_DT_Send_Data(u8 *dataToSend ,uint16_t length)
{
    Uart0_Send(dataToSend,length);
}


//×Ô¶¨Òå
int16_t UserData[10];
void send_user_data1(int16_t *data)
{
    static uint8_t data_to_send[25];
    uint8_t dataCnt = 0;
    int16_t dataTemp = 0;
	uint8_t i,sum = 0;
	data_to_send[dataCnt++] = 0xAA;
	data_to_send[dataCnt++] = 0xAA;
	data_to_send[dataCnt++] = 0xF1;
	data_to_send[dataCnt++] = 0;
	
    for(i = 0; i < 9; i++)
    {
        dataTemp = *data;
        data_to_send[dataCnt++]=BYTE1(dataTemp);
        data_to_send[dataCnt++]=BYTE0(dataTemp);
        data++;
    }
    dataTemp = *data;
    data_to_send[dataCnt++]=BYTE1(dataTemp);
    data_to_send[dataCnt++]=BYTE0(dataTemp);
    data_to_send[3] = i * 2 + 2;
   	
	for(uint8_t i = 0;i < dataCnt;i++)
    {
		sum += data_to_send[i];
    }
	data_to_send[dataCnt++] = sum;
	ANO_DT_Send_Data(data_to_send, dataCnt);
}
