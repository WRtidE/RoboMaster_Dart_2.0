#include "main.h"
#include "Delay.h"

// ΢����ʱ����
void Delay_us(uint32_t nus)
{
       uint32_t ticks;
       uint32_t told,tnow,reload,tcnt=0;
       if((0x0001&(SysTick->CTRL)) ==0)    //��ʱ��δ����
        //   vPortSetupTimerInterrupt();  //��ʼ����ʱ��
 
       reload = SysTick->LOAD;                     //��ȡ��װ�ؼĴ���ֵ
       ticks = nus * (SystemCoreClock / 1000000);  //����ʱ��ֵ
       told=SysTick->VAL;                          //��ȡ��ǰ��ֵ�Ĵ���ֵ����ʼʱ��ֵ��
 
       while(1)
       {
              tnow=SysTick->VAL;          //��ȡ��ǰ��ֵ�Ĵ���ֵ
              if(tnow!=told)              //��ǰֵ�����ڿ�ʼֵ˵�����ڼ���
              {         
 
                     if(tnow<told)             //��ǰֵС�ڿ�ʼ��ֵ��˵��δ�Ƶ�0
                          tcnt+=told-tnow;     //����ֵ=��ʼֵ-��ǰֵ
 
                     else                  //��ǰֵ���ڿ�ʼ��ֵ��˵���ѼƵ�0�����¼���
                            tcnt+=reload-tnow+told;   //����ֵ=��װ��ֵ-��ǰֵ+��ʼֵ  ����
                                                      //�ӿ�ʼֵ�Ƶ�0�� 
 
                     told=tnow;                //���¿�ʼֵ
                     if(tcnt>=ticks)break;     //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
              } 
       }     
}
