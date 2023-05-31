#include "TM1638.h"

//共阴数码管显示代码
unsigned char tab[]={	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
                      0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};//0-f

unsigned char DisBuffer[8]={0,0,0,0,0,0,0,0};    /*显示缓存区*/	//各个数码管显示的值


void TM1638_Write(unsigned char	DATA)			//写数据函数
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		CLK=0;
		if(DATA&0X01)
		DIO=1;
		else
		DIO=0;
		DATA>>=1;
		CLK=1;
	}
}
unsigned char TM1638_Read(void)					//读数据函数
{
	unsigned char i;
	unsigned char DI;
	unsigned char temp=0;
	DIO=1;	//设置为输入
	for(i=0;i<8;i++)
	{
		temp>>=1;
		CLK=0;
		DI=DIO_INT;
		if(DI)
		temp|=0x80;
		CLK=1;
	}
	return temp;
}
void Write_COM(unsigned char cmd)		//发送命令字
{
	STB=0;
	TM1638_Write(cmd);
	STB=1;
}
unsigned char Read_key(void)
{
	unsigned char c[4],i,key_value=0;
	STB=0;
	TM1638_Write(0x42);
	Delay_ms(100);
	for(i=0;i<4;i++)
		c[i]=TM1638_Read();
	STB=1;					//4个字节数据合成一个字节
	if(c[0]==0x04) key_value=1;
	if(c[0]==0x40) key_value=2;
	if(c[1]==0x04) key_value=3;
	if(c[1]==0x40) key_value=4;
	if(c[2]==0x04) key_value=5;
	if(c[2]==0x40) key_value=6;
	if(c[3]==0x04) key_value=7;
	if(c[3]==0x40) key_value=8;
	if(c[0]==0x02) key_value=9;
	if(c[0]==0x20) key_value=10;
	if(c[1]==0x02) key_value=11;
	if(c[1]==0x20) key_value=12;
	if(c[2]==0x02) key_value=13;
	if(c[2]==0x20) key_value=14;
	if(c[3]==0x02) key_value=15;
	if(c[3]==0x20) key_value=16;
	return (key_value);
}
void Write_DATA(unsigned char add,unsigned char DATA)		//指定地址写入数据
{
	Write_COM(0x44);
	STB=0;
	TM1638_Write(0xc0|add);
	TM1638_Write(DATA);
	STB=1;
}
/*
void Write_oneLED(unsigned char num,unsigned char flag)	//单独控制一个LED函数，num为需要控制的led序号，flag为0时熄灭，不为0时点亮
{
	if(flag)
		Write_DATA(2*num+1,1);
	else
		Write_DATA(2*num+1,0);
}  	*/
void Write_allLED(unsigned char LED_flag)					//控制全部LED函数，LED_flag表示各个LED状态
{
	unsigned char i;
	for(i=0;i<8;i++)
		{
			if(LED_flag&(1<<i))
				//Write_DATA(2*i+1,3);
				Write_DATA(2*i+1,1);
			else
				Write_DATA(2*i+1,0);
		}
}

//TM1638初始化函数
void init_TM1638(void)
{
	unsigned char i;
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PB,PE端口时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_SetBits(GPIOC,GPIO_Pin_7);						 //PC.7 输出高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	    		 //LED1-->PE.5 端口配置, 推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOC,GPIO_Pin_8); 						 //PC.8 输出高 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	    		 //LED1-->PE.5 端口配置, 推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
	GPIO_SetBits(GPIOC,GPIO_Pin_9); 						 //PC.9 输出高 

	Write_COM(0x8b);       //亮度 (0x88-0x8f)8级亮度可调
	Write_COM(0x40);       //采用地址自动加1
	STB=0;		           //
	TM1638_Write(0xc0);    //设置起始地址

	for(i=0;i<16;i++)	   //传送16个字节的数据
	TM1638_Write(0x00);
	STB=1;
}
void LedDisplay(unsigned char data0,unsigned char data1,unsigned char data2,unsigned char data3,
               unsigned char data4,unsigned char data5,unsigned char data6,unsigned char data7)

{
	unsigned char  writedata0,writedata1,writedata2,writedata3,writedata4,writedata5,writedata6,writedata7;

	writedata0=(tab[data7]&0x01)+((tab[data6]&0x01)<<1)+((tab[data5]&0x01)<<2)+((tab[data4]&0x01)<<3)+((tab[data3]&0x01)<<4)
		+((tab[data2]&0x01)<<5)+((tab[data1]&0x01)<<6)+((tab[data0]&0x01)<<7);

	writedata1=((tab[data7]&0x02)>>1)+((tab[data6]&0x02))+((tab[data5]&0x02)<<1)+((tab[data4]&0x02)<<2)+((tab[data3]&0x02)<<3)
		+((tab[data2]&0x02)<<4)+((tab[data1]&0x02)<<5)+((tab[data0]&0x02)<<6);

	writedata2=((tab[data7]&0x04)>>2)+((tab[data6]&0x04)>>1)+((tab[data5]&0x04))+((tab[data4]&0x04)<<1)+((tab[data3]&0x04)<<2)
		+((tab[data2]&0x04)<<3)+((tab[data1]&0x04)<<4)+((tab[data0]&0x04)<<5);

	writedata3=((tab[data7]&0x08)>>3)+((tab[data6]&0x08)>>2)+((tab[data5]&0x08)>>1)+((tab[data4]&0x08))+((tab[data3]&0x08)<<1)
		+((tab[data2]&0x08)<<2)+((tab[data1]&0x08)<<3)+((tab[data0]&0x08)<<4);

	writedata4=((tab[data7]&0x10)>>4)+((tab[data6]&0x10)>>3)+((tab[data5]&0x10)>>2)+((tab[data4]&0x10)>>1)+((tab[data3]&0x10))
		+((tab[data2]&0x10)<<1)+((tab[data1]&0x10)<<2)+((tab[data0]&0x10)<<3);

	writedata5=((tab[data7]&0x20)>>5)+((tab[data6]&0x20)>>4)+((tab[data5]&0x20)>>3)+((tab[data4]&0x20)>>2)+((tab[data3]&0x20)>>1)
		+((tab[data2]&0x20))+((tab[data1]&0x20)<<1)+((tab[data0]&0x20)<<2);

	writedata6=((tab[data7]&0x40)>>6)+((tab[data6]&0x40)>>5)+((tab[data5]&0x40)>>4)+((tab[data4]&0x40)>>3)+((tab[data3]&0x40)>>2)
		+((tab[data2]&0x40)>>1)+((tab[data1]&0x40))+((tab[data0]&0x40)<<1);

	writedata7=((tab[data7]&0x80)>>7)+((tab[data6]&0x80)>>6)+((tab[data5]&0x80)>>5)+((tab[data4]&0x80)>>4)+((tab[data3]&0x80)>>3)
		+((tab[data2]&0x80)>>2)+((tab[data1]&0x80)>>1)+((tab[data0]&0x80));

	Write_COM(0x8a);//亮度
	Write_COM(0x40); //写 数据命令
	STB=0;
	TM1638_Write(0xc0);		//写地址命令
	
	TM1638_Write(writedata0);
	TM1638_Write(0x80);
	TM1638_Write(writedata1);
	TM1638_Write(0x00);
	TM1638_Write(writedata2);
	TM1638_Write(0x00);
	TM1638_Write(writedata3);
	TM1638_Write(0x00);
	TM1638_Write(writedata4);
	TM1638_Write(0x00);
	TM1638_Write(writedata5);
	TM1638_Write(0x00);
	TM1638_Write(writedata6);
	TM1638_Write(0x00);
	TM1638_Write(writedata7);
	TM1638_Write(0x00);

	STB=1;


}

/*	switch (key_value) 
	{ 
		case 1: 
		LedDisplay(0,0,0,0,0,0,0,1); key=key_value;
		break; 
		case 2 : 
		LedDisplay(0,0,0,0,0,0,0,2);key=key_value;
		break; 
		case 3 : 
		LedDisplay(0,0,0,0,0,0,0,3);key=key_value;
		break; 
		case 4 : 
		LedDisplay(0,0,0,0,0,0,0,4);key=key_value;
		break; 
		case 5 : 
		LedDisplay(0,0,0,0,0,0,0,5);key=key_value;
		break; 
		case 6 : 
		LedDisplay(0,0,0,0,0,0,0,6);key=key_value;
		break;  
		case 7 : 
		LedDisplay(0,0,0,0,0,0,0,7);key=key_value;
		break; 
		case 8 : 
		LedDisplay(0,0,0,0,0,0,0,8);key=key_value;
		break;  
		case 9: 
		LedDisplay(0,0,0,0,0,0,0,9); key=key_value;
		break; 
		case 10 : 
		LedDisplay(0,0,0,0,0,0,1,0);key=key_value;
		break; 
		case 11 : 
		LedDisplay(0,0,0,0,0,0,1,1);key=key_value;
		break; 
		case 12 : 
		LedDisplay(0,0,0,0,0,0,1,2);key=key_value;
		break; 
		case 13 : 
		LedDisplay(0,0,0,0,0,0,1,3);key=key_value;
		break; 
		case 14 : 
		LedDisplay(0,0,0,0,0,0,1,4);key=key_value;
		break;  
		case 15: 
		LedDisplay(0,0,0,0,0,0,1,5);key=key_value;
		break; 
		case 16 : 
		LedDisplay(0,0,0,0,0,0,1,6);key=key_value;
		break;  
		default : 
		break;//LedDisplay(0,0,0,0,0,0,0,0); 
		*/
