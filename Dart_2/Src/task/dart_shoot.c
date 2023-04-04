# include "dart_shoot.h"
# include "motor.h"
# include "Delay.h"
# include "CAN_receive.h"
#include "cmsis_os.h"

//���ڷ���������
void dart_shoot_task();

//��ʼ�����ڸ������
void dart_init();

//�ú����������pid�ʹ������ݸ����
void dart_speed_send();

//ģʽ1 ���ǰ��վ����ʱ���ڻ��Զ�ת������ǰ������ȫ�Զ��������
void dart_mode_1();

//ģʽ2 ������أ���ʱ���ڻ��Զ�ת���ҷ�x�ȣ���ȫ�Զ��������
void dart_mode_2();

//ģʽ3 ���ɵ���ģʽ
void dart_mode_3();


//s�� 1�� 2�� 3��
void dart_shoot_task(void const * argument)
{
  dart_init();
  for(;;)
  {
    //ģʽ1 ���ǰ��վ����ʱ���ڻ��Զ�ת������ǰ������ȫ�Զ��������
    if(rc_ctrl.rc.s[0] == 1)
    {
      dart_mode_1();
    }
    //ģʽ2 ������أ���ʱ���ڻ��Զ�ת���ҷ�x�ȣ���ȫ�Զ��������
    else if(rc_ctrl.rc.s[0] == 2)
    {
      dart_mode_2();
    }
    //ģʽ3 ���ɵ���ģʽ
    else if(rc_ctrl.rc.s[0] == 3)
    {
      dart_mode_3();
    }
  }
}



void dart_init()
{
    //��ʼ�ٶ�����Ϊ10000�����25m������Ϊ6700�����16m
    dart.speed_25  = 10000;
    dart.speed_16  = 6700;
  
    //���ڵ��ת����0
    dart.motor[0].give_voltage = 0;
    dart.motor[1].give_voltage = 0;
    dart.motor[2].give_voltage = 0;
    dart.motor[3].give_voltage = 0;

    //����pid��ʼ��
    pid_init(&dart.motor_speed_pid[0], 10, 0.01, 0, 30000,30000); 
    pid_init(&dart.motor_speed_pid[1], 10, 0.01, 0, 30000,30000);
    pid_init(&dart.motor_speed_pid[2], 10, 0.01, 0, 30000,30000);
    pid_init(&dart.motor_speed_pid[3], 10, 0.01, 0, 30000,30000);
}

//���ɵ���ģʽ
//s�� 1�� 2�� 3��
//s[0]��   s[1]��
void dart_mode_3()
{
  fp32 speed_16 = dart.speed_16;
  fp32 speed_25 = dart.speed_25;

  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_SET);

         
  if(rc_ctrl.rc.s[1]==3) 
  {
   
    dart.motor[0].target_speed =  speed_16; 
    dart.motor[1].target_speed =  speed_16;
    dart.motor[2].target_speed = -speed_16;
    dart.motor[3].target_speed = -speed_16;
  }
  if(rc_ctrl.rc.s[1]==1)
  {

    dart.motor[0].target_speed =  speed_25; 
    dart.motor[1].target_speed =  speed_25;
    dart.motor[2].target_speed = -speed_25;
    dart.motor[3].target_speed = -speed_25;
  }
  else //�м��ٶ���0
  {
    dart.motor[0].target_speed =  0;
    dart.motor[1].target_speed =  0;
    dart.motor[2].target_speed =  0;
    dart.motor[3].target_speed =  0;
  }
  dart_speed_send();
  osDelay(1); 
}


//ȫ�Զ����ǰ��վ
//s�� 1�� 2�� 3��
void dart_mode_1()
{
  fp32 speed = dart.speed_16;

  //����δ�����ʱ��ŷ���
  if(dart.dart_state == 0)
  {
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_SET);

    dart.motor[0].target_speed =  speed; 
    dart.motor[1].target_speed =  speed;
    dart.motor[2].target_speed = -speed;
    dart.motor[3].target_speed = -speed;
  }
  else
  {
    dart.motor[0].target_speed =  0; 
    dart.motor[1].target_speed =  0;
    dart.motor[2].target_speed =  0;
    dart.motor[3].target_speed =  0;
  }
  dart_speed_send();
  osDelay(1);
}


//ȫ�Զ��������
//s�� 1�� 2�� 3��
void dart_mode_2()
{
  fp32 speed = dart.speed_25;

  // ֻ���ڷ���δ������yaw�ᵽ��ָ���ǶȺ���ڷ���
  if(dart.dart_state == 0 && dart.yaw_state == 1)
  {
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_RESET);
    //�����ٶ�����ֵ
    dart.motor[0].target_speed =  speed; 
    dart.motor[1].target_speed =  speed;
    dart.motor[2].target_speed = -speed;
    dart.motor[3].target_speed = -speed;
  }
  else
  {
    dart.motor[0].target_speed =  0; 
    dart.motor[1].target_speed =  0;
    dart.motor[2].target_speed =  0;
    dart.motor[3].target_speed =  0;
  }
  dart_speed_send();
  osDelay(1);
}




// Ŀ���ٶȴ���pid���㣬�õ�������ֵͨ��can�źŷ��͸����
void dart_speed_send()
{
      for(int i=0;i<4;i++)
    {
    dart.motor[i].give_voltage = pid_calc(&dart.motor_speed_pid[i], 
                                          dart.motor[i].target_speed, 
                                          dart.motor[i].motor_measure.speed_rpm);
    }
    //can�źŷ��������
    CAN_cmd_motor(0,
                  dart.motor[0].give_voltage,
                  dart.motor[1].give_voltage,
                  dart.motor[2].give_voltage,
                  dart.motor[3].give_voltage);
}