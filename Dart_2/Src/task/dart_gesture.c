# include "motor.h"
# include "Delay.h"
# include "CAN_receive.h"
# include "dart_gesture.h"

//yaw �� pitch ���ʼ��
void gesture_init();

//���ڷ�����̬������
void dart_gesture_task();

//��̬���ƺ���
void gesture_control();

//ģʽ1 ���ǰ��վ��ȫ�Զ��������
void gesture_mode_1();

//ģʽ2 ������أ���ʱ���ڻ��Զ�ת���ҷ�x�ȣ���ȫ�Զ�������ڣ�������ɺ���Զ�����
void gesture_mode_2();

//ģʽ3 ���ɵ���ģʽ
void gesture_mode_3();



//s�� 1�� 2�� 3��
void dart_gesture_task(void const * argument)
{
  gesture_init();

  for(;;)
  {
    //��ֹһЩĪ�����������
    while(rc_ctrl.rc.ch[0]<300||((rc_ctrl.rc.ch[0]>900 && rc_ctrl.rc.ch[0]<1300)&&(rc_ctrl.rc.ch[3]>900 && rc_ctrl.rc.ch[3]<1300)))
    {
    }
    if(rc_ctrl.rc.s[0] == 1)
    {
      gesture_mode_1();
    }
    if(rc_ctrl.rc.s[0] == 2)
    {
      gesture_mode_2();
    }
    if(rc_ctrl.rc.s[0] == 3)
    {
      gesture_mode_3();
    }
  }

}


// yaw��pitch���ʼ��
void gesture_init()
{
  dart.yaw_state = 0;
}



//ģʽ1 ���ǰ��վ,ȫ�Զ��������
void gesture_mode_1()
{
  //��ûɶ��������
}


//ģʽ2 ������أ���ʱ���ڻ��Զ�ת���ҷ�x�ȣ���ȫ�Զ�������ڣ�������ɺ���Զ�����
/*
yaw_state = 0:yaw��δƫת��ָ��λ��
yaw_state = 1:yaw��ƫת��ָ��λ��
yaw_state = 2:���ڷ�����ɣ�yaw����Ի�ת
yaw_state = 3:yaw���ת���

dart_state = 0: ����δ����
dart_state = 1: �����ѷ���
*/

void gesture_mode_2()
{
  
  if(dart.yaw_state == 0 || dart.dart_state == 0) // ���yaw��δƫ��δ����
  {
    for(int i=0;i<20;i++)
    {
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,RESET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,SET);
    Delay_us(50);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,RESET);
    Delay_us(50);
    }
    dart.yaw_state = 1;
  }

  if(dart.yaw_state == 2 || dart.dart_state == 1) // ���yaw��ƫת���Ѿ�����
  {
    for(int i=0;i<20;i++)
    {
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,SET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,SET);
    Delay_us(50);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,RESET);
    Delay_us(50);
    }
    dart.yaw_state = 3; //��ʾ�����Ѿ����
  }

}


//ģʽ3 ���ɵ���ģʽ
void gesture_mode_3()
{
  dart.yaw_state = 0;
  gesture_control();
}




void gesture_control()
{
   //���ò������ת��(50�ǿ��Եģ�10����)
  uint8_t set_speed = 100;


  if(rc_ctrl.rc.ch[4] < 824) // С���ݼ�����yaw����
  {
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,SET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,SET);
    Delay_us(set_speed);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,RESET);
    Delay_us(set_speed);
  }
  if(rc_ctrl.rc.ch[4] > 1224) // С���ݼ����ң�yaw����  
  {
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,RESET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,SET);
    Delay_us(set_speed);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,RESET);
    Delay_us(set_speed);
  }
  if(rc_ctrl.rc.ch[0]>1224) // �����ǰ�������������ǰ
  {
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_7,SET);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,SET);
    Delay_us(set_speed);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,RESET);
    Delay_us(set_speed);
  }
  if(rc_ctrl.rc.ch[0]<824) // ������󲦣������������
  {
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_7,RESET);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,SET);
    Delay_us(set_speed);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,RESET);
    Delay_us(set_speed);
  }
}