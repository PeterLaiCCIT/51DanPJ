/*  蓝桥杯2011预赛试卷   
	v1.0   2015.5.4   22:00   by   yk


*/
#include "STC15F2K60S2.H"

extern void  disp(void);
extern void Delay10ms();
extern unsigned char bitbuf[8];

extern bit Start18B20(void);
extern bit Get18B20Temp(int *temp);

bit flag1s = 0;          //1s定时标志
unsigned char T0RH = 0;  //T0重载值的高字节
unsigned char T0RL = 0;  //T0重载值的低字节

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
    int temp;        //读取到的当前温度值
    int int_temp ;      //温度值的整数部分

    EA = 1;            //开总中断
    ConfigTimer0(10);  //T0定时10ms
 	//Start18B20();      //启动DS18B20

	while(1)
	{
	   disp();

	   if (flag1s)  //每秒更新一次温度
        {
            flag1s = 0;
            res = Get18B20Temp(&temp);  //读取当前温度
		    
            if (res)                    //读取成功时，刷新当前温度显示
            {
                int_temp = temp >> 4;             //分离出温度值整数部分
				bitbuf[7] = int_temp % 10 ;
			    bitbuf[6] = int_temp / 10 ;
            }
			else
			{
			 	bitbuf[7] = 18 ;
			    bitbuf[6] = 18 ;
			}
            Start18B20();               //重新启动下一次转换          
        }

	   keybuf4 = S4;
	   if (keybuf4 != backup4)	           //S4加上限
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
	   if (keybuf5 != backup5)		   //S5加下限
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
	   if (keybuf6 != backup6)		   //S6减上限
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
	   if (keybuf7 != backup7)		   //S7减下限
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

/* 配置并启动T0，ms-T0定时时间 */
void ConfigTimer0(unsigned int ms)
{
    unsigned long tmp;  //临时变量
    
    tmp = 11059200 / 12;      //定时器计数频率
    tmp = (tmp * ms) / 1000;  //计算所需的计数值
    tmp = 65536 - tmp;        //计算定时器重载值
    tmp = tmp + 12;           //补偿中断响应延时造成的误差
    T0RH = (unsigned char)(tmp>>8);  //定时器重载值拆分为高低字节
    T0RL = (unsigned char)tmp;
    TMOD &= 0xF0;   //清零T0的控制位
    TMOD |= 0x01;   //配置T0为模式1
    TH0 = T0RH;     //加载T0重载值
    TL0 = T0RL;
    ET0 = 1;        //使能T0中断
    TR0 = 1;        //启动T0
}
/* T0中断服务函数，完成1秒定时 */
void InterruptTimer0() interrupt 1
{
    static unsigned char tmr1s = 0;
    
    TH0 = T0RH;  //重新加载重载值
    TL0 = T0RL;
    tmr1s++;
    if (tmr1s >= 100)  //定时1s
    {
        tmr1s = 0;
        flag1s = 1;
    }
}




