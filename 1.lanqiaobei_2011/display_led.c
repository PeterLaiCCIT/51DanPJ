#include"STC15F2K60S2.H"
#include <intrins.h>

//���������
unsigned char code segtab[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,
                     0x83,0xc6,0xa1,0x86,0x8e,0x89,0x8c,0xff,0xbf,0x8d}; 
unsigned char bitbuf[8]={3,0,2,5,18,18,0,0};        //��ʾ���� 
/************��ʱ����***************/
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
	��������������λ�������ʾ������λѡ�Ͷ�ѡ����P0��� 		  
	����˵������ 
	�������أ��� 
****************************************************/
void  disp(void)
{
	unsigned char i;
	unsigned char bitsel=0x01;

	for(i=0;i<8;i++)
	{
		P0=bitsel;//����׼����λѡ����
		P2=P2&0x1f|0xc0;//��������	   ѡ��Y6C
		P2&=0x1f;//�ٴ��ͷ�Ƭѡ

		P0=segtab[bitbuf[i]];//����׼������ʾ����
		P2=P2&0x1f|0xe0;//��������	    ѡ��Y7C
		delay();//��ʱ���γ��Ӿ�����
		bitsel=_crol_(bitsel,1);	

		P0|=0xff;//��P0�ڳ�ʼ��Ϊ��1111 1111  Ϩ�����������
		P2&=0x1f;//�ٴ��ͷ�Ƭѡ	
	}
}