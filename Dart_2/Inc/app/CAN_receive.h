#ifndef CAN_RECEIVE_H
#define CAN_RECEIVE_H

#include "struct_typedef.h"
#include "motor.h"

/**
  * @brief          ���͵�����Ƶ�����������can1��
  * @param[in]      id_range: (1��0)id_range ȡֵΪ0ʱ0x200(id<5),idȡֵΪ1ʱ0x1ff(id>=5);
  * @param[in]      m1: ������Ƶ���
  * @param[in]      m2: ������Ƶ���
  * @param[in]      m3: ������Ƶ���
  * @param[in]      m4: ������Ƶ���
  * @retval         none
  */

extern void CAN_cmd_motor(uint8_t id_range,int16_t m1, int16_t m2, int16_t m3, int16_t m4);
extern void CAN_cmd_motor_2(uint8_t id_range,int16_t m1);
#endif