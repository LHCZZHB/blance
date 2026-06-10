#include "oled.h"
#include "../IIC/ol_iic.h"
#include "../../SYSTEM/delay/delay.h"
#include "oled_data.h"

//写命令
void oled_writecommand(uint8_t command)
{
    ol_iic_start();
    ol_iic_send_byte(0x78);//寻址
    if(ol_iic_wait_ack())
    {
        ol_iic_stop();
    }
    ol_iic_send_byte(0x00);//Control Byte:非连续模式，写命令
    if(ol_iic_wait_ack())
    {
        ol_iic_stop();
    }
    ol_iic_send_byte(command);//想要写入的命令
    if(ol_iic_wait_ack())
    {
        ol_iic_stop();
    }
    ol_iic_stop();
}

//写数据
void oled_writedata(uint8_t data)
{
    ol_iic_start();
    ol_iic_send_byte(0x78);//寻址
    if(ol_iic_wait_ack())
    {
        ol_iic_stop();
    }
    ol_iic_send_byte(0x40);//Control Byte:非连续模式，写命令
    if(ol_iic_wait_ack())
    {
        ol_iic_stop();
    }
    ol_iic_send_byte(data);//想要写入的数据
    if(ol_iic_wait_ack())
    {
        ol_iic_stop();
    }
    ol_iic_stop();
}

void oled_init(void)
{
    ol_iic_init();
    
    delay_ms(100);
    
    oled_writecommand(0xAE);
    
    oled_writecommand(0xD5);
    oled_writecommand(0x80);
    
    oled_writecommand(0xA8);
    oled_writecommand(0x3F); 
    
    oled_writecommand(0xD3);
    oled_writecommand(0x00);
    
    oled_writecommand(0x40);
    
    oled_writecommand(0xA1);
    
    oled_writecommand(0xC8);
    
    oled_writecommand(0xDA);
    oled_writecommand(0x12);
    
    oled_writecommand(0x81);    
    oled_writecommand(0xCF);
    
    oled_writecommand(0xD9);   
    oled_writecommand(0xF1);
    
    oled_writecommand(0xDB);
    oled_writecommand(0x30);
        
    oled_writecommand(0xA4);
    
    oled_writecommand(0xA6);
    
    oled_writecommand(0x8D);
    oled_writecommand(0x14);
        
    oled_writecommand(0xAF);
    
    delay_ms(100);
}

//显示起始坐标,x:0-127,y:0-7
//命令高4位为0000时，则低4位表示X坐标低位
//命令高4位为0001时，则低4位表示X坐标高位

void oled_setcursor(uint8_t x, uint8_t y)
{
    oled_writecommand(0x00 | (x & 0x0F));
    oled_writecommand(0x10 | (x & 0xF0) >> 4);
    oled_writecommand(0xB0 | y);
}    

void oled_clear(void)
{
  for(uint8_t j = 0;j < 8;j ++)
  {
    oled_setcursor(0,j);
    for(uint8_t i = 0;i < 128;i ++)
      {
        oled_writedata(0x00);
      }
  }
}

//显示一个字符的函数
void oled_showchar(uint8_t x,uint8_t y,char byte, uint8_t Fontsize)
{
    if(Fontsize == 6)
    {
        oled_setcursor(x, y);
        for(uint8_t i = 0;i < 6;i ++)
        {
            oled_writedata(OLED_F6x8[byte - ' '][i]);
        }
    }
    else if(Fontsize == 8)
    {
        oled_setcursor(x, y);
        for(uint8_t i = 0;i < 8;i ++)
        {
            oled_writedata(OLED_F8x16[byte - ' '][i]);
        }        
        
        oled_setcursor(x, y + 1);
        for(uint8_t i = 0;i < 8;i ++)
        {
            oled_writedata(OLED_F8x16[byte - ' '][i + 8]);
        } 
    }
}

//显示一个字符串
void oled_showstring(uint8_t x,uint8_t y,char *string, uint8_t Fontsize)
{
    for(uint8_t i = 0;string[i] != '\0';i ++)
    {
        oled_showchar(x + i * Fontsize,y ,string[i], Fontsize);
    }
}
