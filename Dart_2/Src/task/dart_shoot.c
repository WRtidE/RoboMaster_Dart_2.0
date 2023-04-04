# include "dart_shoot.h"
# include "motor.h"
# include "Delay.h"
# include "CAN_receive.h"
#include "cmsis_os.h"

//飞镖发射主任务
void dart_shoot_task();

//初始化飞镖各项参数
void dart_init();

//该函数负责计算pid和传递数据给电机
void dart_speed_send();

//模式1 打击前哨站，此时飞镖会自动转动至正前方，并全自动发射飞镖
void dart_mode_1();

//模式2 打击基地，此时飞镖会自动转置右方x度，并全自动发射飞镖
void dart_mode_2();

//模式3 自由调整模式
void dart_mode_3();


//s键 1上 2下 3中
void dart_shoot_task(void const * argument)
{
  dart_init();
  for(;;)
  {
    //模式1 打击前哨站，此时飞镖会自动转动至正前方，并全自动发射飞镖
    if(rc_ctrl.rc.s[0] == 1)
    {
      dart_mode_1();
    }
    //模式2 打击基地，此时飞镖会自动转置右方x度，并全自动发射飞镖
    else if(rc_ctrl.rc.s[0] == 2)
    {
      dart_mode_2();
    }
    //模式3 自由调整模式
    else if(rc_ctrl.rc.s[0] == 3)
    {
      dart_mode_3();
    }
  }
}



void dart_init()
{
    //初始速度设置为10000则击打25m，设置为6700则击打16m
    dart.speed_25  = 10000;
    dart.speed_16  = 6700;
  
    //飞镖电机转速置0
    dart.motor[0].give_voltage = 0;
    dart.motor[1].give_voltage = 0;
    dart.motor[2].give_voltage = 0;
    dart.motor[3].give_voltage = 0;

    //飞镖pid初始化
    pid_init(&dart.motor_speed_pid[0], 10, 0.01, 0, 30000,30000); 
    pid_init(&dart.motor_speed_pid[1], 10, 0.01, 0, 30000,30000);
    pid_init(&dart.motor_speed_pid[2], 10, 0.01, 0, 30000,30000);
    pid_init(&dart.motor_speed_pid[3], 10, 0.01, 0, 30000,30000);
}

//自由调整模式
//s键 1上 2下 3中
//s[0]左   s[1]右
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
  else //中间速度置0
  {
    dart.motor[0].target_speed =  0;
    dart.motor[1].target_speed =  0;
    dart.motor[2].target_speed =  0;
    dart.motor[3].target_speed =  0;
  }
  dart_speed_send();
  osDelay(1); 
}


//全自动打击前哨站
//s键 1上 2下 3中
void dart_mode_1()
{
  fp32 speed = dart.speed_16;

  //飞镖未发射的时候才发射
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


//全自动打击基地
//s键 1上 2下 3中
void dart_mode_2()
{
  fp32 speed = dart.speed_25;

  // 只有在飞镖未发射且yaw轴到达指定角度后飞镖发射
  if(dart.dart_state == 0 && dart.yaw_state == 1)
  {
    HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_RESET);
    //给出速度期望值
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




// 目标速度带入pid计算，得到的最终值通过can信号发送给电机
void dart_speed_send()
{
      for(int i=0;i<4;i++)
    {
    dart.motor[i].give_voltage = pid_calc(&dart.motor_speed_pid[i], 
                                          dart.motor[i].target_speed, 
                                          dart.motor[i].motor_measure.speed_rpm);
    }
    //can信号发送至电机
    CAN_cmd_motor(0,
                  dart.motor[0].give_voltage,
                  dart.motor[1].give_voltage,
                  dart.motor[2].give_voltage,
                  dart.motor[3].give_voltage);
}