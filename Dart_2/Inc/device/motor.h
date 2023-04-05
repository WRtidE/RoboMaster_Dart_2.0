#ifndef MOTOR_H
#define MOTOR_H
#include "struct_typedef.h"
#include "PID.h"
#include "remote_control.h"

//rm motor data
//接收电机的返回值
typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;     // 电机转速
    int16_t given_current; // 给定电流
    uint8_t temperate;     
    int16_t last_ecd;
} motor_measure_t;

//dart motor data
//单个电机数据结构体
typedef struct
{
  motor_measure_t motor_measure; // 电机的基本数据
  fp32 accel;     // 加速度
  fp32 speed;	  // 速度
  fp32 target_speed; // 设定速度
  int16_t give_voltage;  // 电压赋值 

} dart_motor_t;

//dart total data
//飞镖整体控制结构体
typedef struct 
{
    dart_motor_t motor[5];     //四个发射电机3508,一个装填电机2006
    pid_struct_t motor_speed_pid[5];    //四个电机的加速pid
    pid_struct_t motor_reset_pid[5];    //四个电机的减速pid

    uint8_t dart_state; //飞镖状态标志位
    uint8_t yaw_state; //飞镖yaw轴状态标志位

    fp32 speed_16;//打击前哨站的转速
    fp32 speed_25;//打击基地的转速
    fp32 speed_reload; //飞镖装填速度

}dart_control;

dart_control   dart;

#endif
