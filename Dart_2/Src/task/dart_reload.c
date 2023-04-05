# include "dart_reload.h"
# include "motor.h"
# include "Delay.h"
# include "CAN_receive.h"
#include "cmsis_os.h"

// 飞镖初始化
void reload_init();

//装填主函数
void dart_reload_task();

//该函数负责计算pid和传递数据给电机
void reload_speed_send();

//模式1 打击前哨站，并全自动发射飞镖
void reload_mode_1();

//模式2 打击基地，此时飞镖会自动转置右方x度，并全自动发射飞镖，发射完成后会自动回中
void reload_mode_2();

//模式3 自由调整模式
void reload_mode_3();

//模式4 半自动发射模式
void reload_mode_4();


//填装主函数
void dart_reload_task(void const * argument)
{

  reload_init();

  for(;;)
  {
    if(rc_ctrl.rc.s[1] == 1)
    {
      //模式1 打击前哨站，此时飞镖会自动转动至正前方，并全自动发射飞镖
      reload_mode_3();
    }
    else if(rc_ctrl.rc.s[1] == 2)
    {
      //模式2 打击基地，此时飞镖会自动转置右方x度，发射完成后会自动回中
      reload_mode_4();
    }
    else if(rc_ctrl.rc.s[1] == 3)
    {
      //模式3 自由调整模式
      reload_mode_3();
    }
  }

}


void reload_init()
{
    //装填设置为10000
    dart.speed_reload = 10000;
  
    //飞镖电机转速置0
    dart.motor[4].give_voltage = 0;

    //飞镖M2006电机pid初始化
    pid_init(&dart.motor_speed_pid[4], 10, 0.01, 0, 30000,30000); 

    //飞镖state标志位值0
    dart.dart_state = 0;

}



//模式3 自由调整模式
//s键 1上 2下 3中
//s[1]左   s[0]右
//
void reload_mode_3()
{

  dart.dart_state = 0;
  fp32 speed = dart.speed_reload;

  if(rc_ctrl.rc.ch[1]>1224) // 右摇杆往前推
  {
    dart.motor[4].target_speed = speed;
  }
  else if(rc_ctrl.rc.ch[1]<824)  // 右摇杆往后拉 
  {
    dart.motor[4].target_speed = -speed;
  }
  else
  {
    dart.motor[4].target_speed = 0;
  }
  reload_speed_send(); 
}


//模式1 打击前哨站，自动发射飞镖
void reload_mode_1()
{
  fp32 speed = dart.speed_reload;
  int16_t current = dart.motor[4].motor_measure.given_current;

  if(dart.dart_state == 0)
  {
    dart.motor[4].target_speed = speed;
  }
  //电流检测，如果检测到电流过大，则反转一段时间
  if(current > 9000 || dart.dart_state == 1) 
  {
    dart.dart_state = 1;   //告诉发射部分发射已完成
    while(current > -9000)
    {
      dart.motor[4].target_speed = -speed; //反转一会儿
    }
    dart.dart_state = 2;
  }
  else
  {
    dart.motor[4].target_speed = 0;
  }

  osDelay(10);

}


//模式2 打击基地，此时飞镖会自动转置右方x度，并全自动发射飞镖，发射完成后会自动回中
void reload_mode_2()
{
  fp32 speed = dart.speed_reload;
  int16_t current = dart.motor[4].motor_measure.given_current;

  if(dart.yaw_state == 1)
  {
    //电流检测，如果检测到电流过大，则反转一段时间
    if(current > 9000 || dart.dart_state == 1) 
    {
      dart.dart_state = 1;   //告诉发射部分发射已完成
      dart.yaw_state = 2;    //告诉yaw轴可以转回去了

      dart.motor[4].target_speed = -speed; //开始反转

      if(current < -9000)
      {
        dart.motor[4].target_speed = speed; //正转一会儿
        dart.dart_state = 2; 
      }
    }
    if(dart.dart_state == 0)
    {
      dart.motor[4].target_speed = speed; //正常工作状态下，飞镖装填飞镖
    }
    
  }
  else
  {
    dart.motor[4].target_speed = 0;
  }
 
  reload_speed_send();
}


// 模式4 半自动模式，yaw会转动到发射角度，到达指定角度之后操作手可以发射，发射完成后会自动转回
void reload_mode_4()
{
  fp32 speed = dart.speed_reload;

  if(dart.yaw_state >= 1)
  {
    if(rc_ctrl.rc.ch[1]>1224) // 右摇杆往前推
    {
      dart.motor[4].target_speed = speed;
    }
    else if(rc_ctrl.rc.ch[1]<824)  // 右摇杆往后拉 
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


// 目标速度带入pid计算，得到的最终值通过can信号发送给电机
void reload_speed_send()
{

    dart.motor[4].give_voltage = pid_calc(&dart.motor_speed_pid[4], 
                                          dart.motor[4].target_speed, 
                                          dart.motor[4].motor_measure.speed_rpm);
    
    //can信号发送至电机
    CAN_cmd_motor(1,dart.motor[4].give_voltage,0,0,0);
    osDelay(1); 
}


