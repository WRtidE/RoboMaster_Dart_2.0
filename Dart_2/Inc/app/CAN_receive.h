#ifndef CAN_RECEIVE_H
#define CAN_RECEIVE_H

#include "struct_typedef.h"
#include "motor.h"

/**
  * @brief          发送电机控制电流，挂载在can1上
  * @param[in]      id_range: (1或0)id_range 取值为0时0x200(id<5),id取值为1时0x1ff(id>=5);
  * @param[in]      m1: 电机控制电流
  * @param[in]      m2: 电机控制电流
  * @param[in]      m3: 电机控制电流
  * @param[in]      m4: 电机控制电流
  * @retval         none
  */

extern void CAN_cmd_motor(uint8_t id_range,int16_t m1, int16_t m2, int16_t m3, int16_t m4);
extern void CAN_cmd_motor_2(uint8_t id_range,int16_t m1);
#endif