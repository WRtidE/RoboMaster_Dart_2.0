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

void gesture_mode_4();



//s�� 1�� 2�� 3��
void dart_gesture_task(void const * argument)
{
  gesture_init();

  for(;;)
  {
    //��ֹһЩĪ�����������
    if(rc_ctrl.rc.s[1] == 1)
    {
      gesture_mode_1();
    }
    if(rc_ctrl.rc.s[1] == 2)
    {
      gesture_mode_4();
    }
    if(rc_ctrl.rc.s[1] == 3)
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
  //��ûɶ�������ģ��ͷ���
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
  dart.yaw_state = 0; //��־λ����
  gesture_control();
}



void gesture_control()
{
   //���ò������ת��(50�ǿ��Եģ�10����)
  uint8_t set_speed = 200;

  if(rc_ctrl.rc.ch[3] >1224) // �����ǰ����pitch��ǰ
  {
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,SET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,SET);
    Delay_us(set_speed);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,RESET);
    Delay_us(set_speed);
  }
  if(rc_ctrl.rc.ch[3] < 824) // �ұ�pitch���ң�pitch���� 
  {
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,RESET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,SET);
    Delay_us(set_speed);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,RESET);
    Delay_us(set_speed);
  }
  if(rc_ctrl.rc.ch[4]<824) // С���ݰ���������̨����
  {
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_7,SET);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,SET);
    Delay_us(set_speed);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_7,RESET);
    Delay_us(set_speed);
  }
  if(rc_ctrl.rc.ch[4]>1224) // С���ݰ������ң���̨����
  {
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_7,SET);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,RESET);
    Delay_us(set_speed);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_7,RESET);
    Delay_us(set_speed);
  }
}

// �Զ����ڽǶȣ���⵽��������󣬽ǶȻ�ת
void gesture_mode_4()
{
  uint8_t set_speed = 200;

  if(dart.dart_state == 0 && dart.yaw_state == 0)
  {
    for(int i=0;i<9000;i++)
    {
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_7,SET);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,SET);
    Delay_us(set_speed);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_7,RESET);
    Delay_us(set_speed);
    }
    dart.yaw_state = 1; //��ʾ�Ѿ�ת��ָ���Ƕ�
  }

  if(dart.yaw_state == 2)
  {
    for(int i=0;i<9000;i++)
    {
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_7,SET);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,RESET);
    Delay_us(set_speed);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_7,RESET);
    Delay_us(set_speed);
    }
    dart.yaw_state = 3; //��ʾ���в������
  }
  
}