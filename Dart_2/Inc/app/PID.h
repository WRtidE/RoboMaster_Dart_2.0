#ifndef __PID_H
#define __PID_H
#include "main.h"


//越界则赋边界值
#define LIMIT_MIN_MAX(x,min,max) (x) = (((x)<=(min))?(min):(((x)>=(max))?(max):(x)))

// pid结构体
typedef struct
{
  float kp;
  float ki;
  float kd;
  float i_max;
  float out_max;
  
  float ref;      // target value
  float fdb;      // feedback value  
  float err[2];   // error and last error

  float p_out;
  float i_out;
  float d_out;
  float output;
}pid_struct_t;

// 启动pid算法
void pid_init(pid_struct_t *pid,
              float kp,
              float ki,
              float kd,
              float i_max,
              float out_max);


//调用pid
float pid_calc(pid_struct_t *pid, float ref, float fdb);						
#endif
							