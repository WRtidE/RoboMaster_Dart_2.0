# include "dart_reload.h"
# include "motor.h"
# include "Delay.h"
# include "CAN_receive.h"
#include "cmsis_os.h"

// ���ڳ�ʼ��
void reload_init();

//װ��������
void dart_reload_task();

//�ú����������pid�ʹ������ݸ����
void reload_speed_send();

//ģʽ1 ���ǰ��վ����ȫ�Զ��������
void reload_mode_1();

//ģʽ2 ������أ���ʱ���ڻ��Զ�ת���ҷ�x�ȣ���ȫ�Զ�������ڣ�������ɺ���Զ�����
void reload_mode_2();

//ģʽ3 ���ɵ���ģʽ
void reload_mode_3();

//ģʽ4 ���Զ�����ģʽ
void reload_mode_4();


//��װ������
void dart_reload_task(void const * argument)
{

  reload_init();

  for(;;)
  {
    if(rc_ctrl.rc.s[1] == 1)
    {
      //ģʽ1 ���ǰ��վ����ʱ���ڻ��Զ�ת������ǰ������ȫ�Զ��������
      reload_mode_3();
    }
    else if(rc_ctrl.rc.s[1] == 2)
    {
      //ģʽ2 ������أ���ʱ���ڻ��Զ�ת���ҷ�x�ȣ�������ɺ���Զ�����
      reload_mode_4();
    }
    else if(rc_ctrl.rc.s[1] == 3)
    {
      //ģʽ3 ���ɵ���ģʽ
      reload_mode_3();
    }
  }

}


void reload_init()
{
    //װ������Ϊ10000
    dart.speed_reload = 10000;
  
    //���ڵ��ת����0
    dart.motor[4].give_voltage = 0;

    //����M2006���pid��ʼ��
    pid_init(&dart.motor_speed_pid[4], 10, 0.01, 0, 30000,30000); 

    //����state��־λֵ0
    dart.dart_state = 0;

}



//ģʽ3 ���ɵ���ģʽ
//s�� 1�� 2�� 3��
//s[1]��   s[0]��
//
void reload_mode_3()
{

  dart.dart_state = 0;
  fp32 speed = dart.speed_reload;

  if(rc_ctrl.rc.ch[1]>1224) // ��ҡ����ǰ��
  {
    dart.motor[4].target_speed = speed;
  }
  else if(rc_ctrl.rc.ch[1]<824)  // ��ҡ�������� 
  {
    dart.motor[4].target_speed = -speed;
  }
  else
  {
    dart.motor[4].target_speed = 0;
  }
  reload_speed_send(); 
}


//ģʽ1 ���ǰ��վ���Զ��������
void reload_mode_1()
{
  fp32 speed = dart.speed_reload;
  int16_t current = dart.motor[4].motor_measure.given_current;

  if(dart.dart_state == 0)
  {
    dart.motor[4].target_speed = speed;
  }
  //������⣬�����⵽����������תһ��ʱ��
  if(current > 9000 || dart.dart_state == 1) 
  {
    dart.dart_state = 1;   //���߷��䲿�ַ��������
    while(current > -9000)
    {
      dart.motor[4].target_speed = -speed; //��תһ���
    }
    dart.dart_state = 2;
  }
  else
  {
    dart.motor[4].target_speed = 0;
  }

  osDelay(10);

}


//ģʽ2 ������أ���ʱ���ڻ��Զ�ת���ҷ�x�ȣ���ȫ�Զ�������ڣ�������ɺ���Զ�����
void reload_mode_2()
{
  fp32 speed = dart.speed_reload;
  int16_t current = dart.motor[4].motor_measure.given_current;

  if(dart.yaw_state == 1)
  {
    //������⣬�����⵽����������תһ��ʱ��
    if(current > 9000 || dart.dart_state == 1) 
    {
      dart.dart_state = 1;   //���߷��䲿�ַ��������
      dart.yaw_state = 2;    //����yaw�����ת��ȥ��

      dart.motor[4].target_speed = -speed; //��ʼ��ת

      if(current < -9000)
      {
        dart.motor[4].target_speed = speed; //��תһ���
        dart.dart_state = 2; 
      }
    }
    if(dart.dart_state == 0)
    {
      dart.motor[4].target_speed = speed; //��������״̬�£�����װ�����
    }
    
  }
  else
  {
    dart.motor[4].target_speed = 0;
  }
 
  reload_speed_send();
}


// ģʽ4 ���Զ�ģʽ��yaw��ת��������Ƕȣ�����ָ���Ƕ�֮������ֿ��Է��䣬������ɺ���Զ�ת��
void reload_mode_4()
{
  fp32 speed = dart.speed_reload;

  if(dart.yaw_state >= 1)
  {
    if(rc_ctrl.rc.ch[1]>1224) // ��ҡ����ǰ��
    {
      dart.motor[4].target_speed = speed;
    }
    else if(rc_ctrl.rc.ch[1]<824)  // ��ҡ�������� 
    {
      dart.motor[4].target_speed = -speed;
      dart.dart_state = 1;
      dart.yaw_state = 2 ;
    }
    else
    {
      dart.motor[4].target_speed = 0;
    }
  }

  reload_speed_send(); 

}


// Ŀ���ٶȴ���pid���㣬�õ�������ֵͨ��can�źŷ��͸����
void reload_speed_send()
{

    dart.motor[4].give_voltage = pid_calc(&dart.motor_speed_pid[4], 
                                          dart.motor[4].target_speed, 
                                          dart.motor[4].motor_measure.speed_rpm);
    
    //can�źŷ��������
    CAN_cmd_motor(1,dart.motor[4].give_voltage,0,0,0);
    osDelay(1); 
}


