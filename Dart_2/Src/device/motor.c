#include "motor.h"
#include "PID.h"
#include "struct_typedef.h"
#include "CAN_receive.h"

//pid ����
#define MOTOR_SPEED_PID_KP  20.0f
#define MOTOR_SPEED_PID_KI  0.1f
#define MOTOR_SPEED_PID_KD  0.0f

//�޸����ϲ������ɽ���pid����


// ���ڷ�����ת������Ϊ0
void dart_shoot_reset(dart_control *dart)
{   
    dart->motor[0].give_voltage = 0;
    dart->motor[1].give_voltage = 0;
    dart->motor[2].give_voltage = 0;
    dart->motor[3].give_voltage = 0;
}

// ����װ����ת������Ϊ0
void dart_reload_reset(dart_control *dart)
{
     dart->motor[4].give_voltage = 0;
}