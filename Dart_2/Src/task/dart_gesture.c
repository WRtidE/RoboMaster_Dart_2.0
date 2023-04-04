# include "motor.h"
# include "Delay.h"
# include "CAN_receive.h"
# include "dart_gesture.h"

//yaw 和 pitch 轴初始化
void gesture_init();

//飞镖发射姿态主函数
void dart_gesture_task();

//姿态控制函数
void gesture_control();

//模式1 打击前哨站，全自动发射飞镖
void gesture_mode_1();

//模式2 打击基地，此时飞镖会自动转置右方x度，并全自动发射飞镖，发射完成后会自动回中
void gesture_mode_2();

//模式3 自由调整模式
void gesture_mode_3();



//s键 1上 2下 3中
void dart_gesture_task(void const * argument)
{
  gesture_init();

  for(;;)
  {
    //防止一些莫名其妙的噪声
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


// yaw和pitch轴初始化
void gesture_init()
{
  dart.yaw_state = 0;
}



//模式1 打击前哨站,全自动发射飞镖
void gesture_mode_1()
{
  //真没啥可以做的
}


//模式2 打击基地，此时飞镖会自动转置右方x度，并全自动发射飞镖，发射完成后会自动回中
/*
yaw_state = 0:yaw轴未偏转置指定位置
yaw_state = 1:yaw轴偏转至指定位置
yaw_state = 2:飞镖发射完成，yaw轴可以回转
yaw_state = 3:yaw轴回转完成

dart_state = 0: 飞镖未发射
dart_state = 1: 飞镖已发射
*/

void gesture_mode_2()
{
  
  if(dart.yaw_state == 0 || dart.dart_state == 0) // 如果yaw轴未偏且未发射
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

  if(dart.yaw_state == 2 || dart.dart_state == 1) // 如果yaw轴偏转且已经发射
  {
    for(int i=0;i<20;i++)
    {
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,SET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,SET);
    Delay_us(50);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,RESET);
    Delay_us(50);
    }
    dart.yaw_state = 3; //表示任务已经完成
  }

}


//模式3 自由调整模式
void gesture_mode_3()
{
  dart.yaw_state = 0;
  gesture_control();
}




void gesture_control()
{
   //设置步进电机转速(50是可以的，10不行)
  uint8_t set_speed = 100;


  if(rc_ctrl.rc.ch[4] < 824) // 小陀螺键往左，yaw向左
  {
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,SET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,SET);
    Delay_us(set_speed);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,RESET);
    Delay_us(set_speed);
  }
  if(rc_ctrl.rc.ch[4] > 1224) // 小陀螺键往右，yaw向右  
  {
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,RESET);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,SET);
    Delay_us(set_speed);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,RESET);
    Delay_us(set_speed);
  }
  if(rc_ctrl.rc.ch[0]>1224) // 左边往前拨，步进电机往前
  {
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_7,SET);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,SET);
    Delay_us(set_speed);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,RESET);
    Delay_us(set_speed);
  }
  if(rc_ctrl.rc.ch[0]<824) // 左边往后拨，步进电机往后
  {
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_7,RESET);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,SET);
    Delay_us(set_speed);
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_2,RESET);
    Delay_us(set_speed);
  }
}