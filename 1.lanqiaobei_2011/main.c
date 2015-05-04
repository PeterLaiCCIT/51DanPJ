/*  ���ű�2011Ԥ���Ծ�   
	v1.0   2015.5.4   22:00   by   yk


*/
#include "STC15F2K60S2.H"

extern void  disp(void);
extern void Delay10ms();
extern unsigned char bitbuf[8];

extern bit Start18B20(void);
extern bit Get18B20Temp(int *temp);

bit flag1s = 0;          //1s��ʱ��־
unsigned char T0RH = 0;  //T0����ֵ�ĸ��ֽ�
unsigned char T0RL = 0;  //T0����ֵ�ĵ��ֽ�

void ConfigTimer0(unsigned int ms);


sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;

void main()
{
	bit keybuf7, keybuf6, keybuf5, keybuf4;
	bit backup7 = 1  , backup6 = 1, backup5 = 1, backup4  = 1;
	unsigned char cnt_up = 30, cnt_down= 25;

	bit res;
    int temp;        //��ȡ���ĵ�ǰ�¶�ֵ
    int int_temp ;      //�¶�ֵ����������

    EA = 1;            //�����ж�
    ConfigTimer0(10);  //T0��ʱ10ms
 	//Start18B20();      //����DS18B20

	while(1)
	{
	   disp();

	   if (flag1s)  //ÿ�����һ���¶�
        {
            flag1s = 0;
            res = Get18B20Temp(&temp);  //��ȡ��ǰ�¶�
		    
            if (res)                    //��ȡ�ɹ�ʱ��ˢ�µ�ǰ�¶���ʾ
            {
                int_temp = temp >> 4;             //������¶�ֵ��������
				bitbuf[7] = int_temp % 10 ;
			    bitbuf[6] = int_temp / 10 ;
            }
			else
			{
			 	bitbuf[7] = 18 ;
			    bitbuf[6] = 18 ;
			}
            Start18B20();               //����������һ��ת��          
        }

	   keybuf4 = S4;
	   if (keybuf4 != backup4)	           //S4������
	   {
		    Delay10ms();
			if (keybuf4 == S4)
			{
				  if (backup4 == 0)
				  {
					  cnt_up++;
					  bitbuf[1] = cnt_up % 10 ;
					  bitbuf[0] = cnt_up / 10 ;
				  }
				  backup4 = keybuf4;
			}
	   }
	   
	   keybuf5 = S5;
	   if (keybuf5 != backup5)		   //S5������
	   {
		   	  Delay10ms();
			if (keybuf5 == S5)
			{
				  if (backup5 == 0)
				  {
					  cnt_down++;
					  bitbuf[3] = cnt_down % 10 ;
					  bitbuf[2] = cnt_down / 10 ;
				  }
				   backup5 = keybuf5;
			}
	   }

	   keybuf6 = S6;
	   if (keybuf6 != backup6)		   //S6������
	   {
		   	  Delay10ms();
			if (keybuf6 == S6)
			{
				  if (backup6 == 0)
				  {
					  cnt_up--;
					  bitbuf[1] = cnt_up % 10 ;
					  bitbuf[0] = cnt_up / 10 ;
				  }
				  backup6 = keybuf6;
			}
	   }
	   
	   keybuf7 = S7;
	   if (keybuf7 != backup7)		   //S7������
	   {
		   	  Delay10ms();
			if (keybuf7 == S7)
			{
				  if (backup7 == 0)
				  {
					  cnt_down--;
					  bitbuf[3] = cnt_down % 10 ;
					  bitbuf[2] = cnt_down / 10 ;
				  }
				  backup7 = keybuf7;
			}
	   }
	}
}

/* ���ò�����T0��ms-T0��ʱʱ�� */
void ConfigTimer0(unsigned int ms)
{
    unsigned long tmp;  //��ʱ����
    
    tmp = 11059200 / 12;      //��ʱ������Ƶ��
    tmp = (tmp * ms) / 1000;  //��������ļ���ֵ
    tmp = 65536 - tmp;        //���㶨ʱ������ֵ
    tmp = tmp + 12;           //�����ж���Ӧ��ʱ��ɵ����
    T0RH = (unsigned char)(tmp>>8);  //��ʱ������ֵ���Ϊ�ߵ��ֽ�
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;   //����T0�Ŀ���λ
    TMOD |= 0x01;   //����T0Ϊģʽ1
    TH0 = T0RH;     //����T0����ֵ
    TL0 = T0RL;
    ET0 = 1;        //ʹ��T0�ж�
    TR0 = 1;        //����T0
}
/* T0�жϷ����������1�붨ʱ */
void InterruptTimer0() interrupt 1
{
    static unsigned char tmr1s = 0;
    
    TH0 = T0RH;  //���¼�������ֵ
    TL0 = T0RL;
    tmr1s++;
    if (tmr1s >= 100)  //��ʱ1s
    {
        tmr1s = 0;
        flag1s = 1;
    }
}




