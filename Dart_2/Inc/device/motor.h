#ifndef MOTOR_H
#define MOTOR_H
#include "struct_typedef.h"
#include "PID.h"
#include "remote_control.h"

//rm motor data
//���յ���ķ���ֵ
typedef struct
{
    uint16_t ecd;
    int16_t speed_rpm;     // ���ת��
    int16_t given_current; // ��������
    uint8_t temperate;     
    int16_t last_ecd;
} motor_measure_t;

//dart motor data
//����������ݽṹ��
typedef struct
{
  motor_measure_t motor_measure; // ����Ļ�������
  fp32 accel;     // ���ٶ�
  fp32 speed;	  // �ٶ�
  fp32 target_speed; // �趨�ٶ�
  int16_t give_voltage;  // ��ѹ��ֵ 

} dart_motor_t;

//dart total data
//����������ƽṹ��
typedef struct 
{
    dart_motor_t motor[5];     //�ĸ�������3508,һ��װ����2006
    pid_struct_t motor_speed_pid[5];    //�ĸ�����ļ���pid
    pid_struct_t motor_reset_pid[5];    //�ĸ�����ļ���pid

    uint8_t dart_state; //����״̬��־λ
    uint8_t yaw_state; //����yaw��״̬��־λ

    fp32 speed_16;//���ǰ��վ��ת��
    fp32 speed_25;//������ص�ת��
    fp32 speed_reload; //����װ���ٶ�

}dart_control;

dart_control   dart;

#endif
