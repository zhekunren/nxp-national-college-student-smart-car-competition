#ifndef _LQOLED_H
#define _LQOLED_H


#define byte unsigned char  //自己加的
#define word unsigned int   //自己加的

 void LCD_Init(void);
 void LCD_CLS(void);
 void LCD_P6x8Str(byte x,byte y,byte ch[]);
 void LCD_P8x16Str(byte x,byte y,byte ch[]);
 void LCD_P14x16Str(byte x,byte y,byte ch[]);
 void LCD_Print(byte x, byte y, byte ch[]);
 void LCD_PutPixel(byte x,byte y);
 void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif);
 void Draw_LQLogo(void);
 void Draw_LibLogo(void);
 void Draw_BMP(byte x0,byte y0,byte x1,byte y1,byte bmp[]); 
 void LCD_Fill(byte dat);
 void Dis_Num(byte y, byte x, int num,byte N);
 void Dis_Num2(byte y, byte x,float num1,byte N);
 void Dis_Num3(byte y, byte x, int nummm,byte N);
 void Dis_Num4(byte y, byte x,float num1,byte N);
 int Absolute(int a);

#endif



