#include"STC15F2K60S2.H"
#include <intrins.h>

//共阳数码管
unsigned char code segtab[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,
                     0x83,0xc6,0xa1,0x86,0x8e,0x89,0x8c,0xff,0xbf,0x8d}; 
unsigned char bitbuf[8]={3,0,2,5,18,18,0,0};        //显示缓存 
/************延时函数***************/
void Delay10ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 108;
	j = 145;
	do
	{
		while (--j);
	} while (--i);
}


void delay()		//@11.0592MHz
{
	unsigned char i = 200;
	 while (i--);
}
/***************************************************
	功能描述：右四位数码管显示函数，位选和段选都由P0完成 		  
	参数说明：无 
	参数返回：无 
****************************************************/
void  disp(void)
{
	unsigned char i;
	unsigned char bitsel=0x01;

	for(i=0;i<8;i++)
	{
		P0=bitsel;//数据准备：位选数据
		P2=P2&0x1f|0xc0;//送入数据	   选中Y6C
		P2&=0x1f;//再次释放片选

		P0=segtab[bitbuf[i]];//数据准备：显示数据
		P2=P2&0x1f|0xe0;//送入数据	    选中Y7C
		delay();//延时，形成视觉暂留
		bitsel=_crol_(bitsel,1);	

		P0|=0xff;//将P0口初始化为：1111 1111  熄灭所有数码管
		P2&=0x1f;//再次释放片选	
	}
}