/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       can_receive.c/h
  * @brief      there is CAN interrupt function  to receive motor data,
  *             and CAN send function to send motor current to control motor.
  *          
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */

#include "CAN_receive.h"
#include "main.h"
#include "motor.h"
extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

#define MOTOR_3508_NUM 4   
#define MOTOR_2006_NUM 1
#define FEEDBACK_ID_BASE      0x201
#define CAN_CONTROL_ID_BASE   0x200
#define CAN_CONTROL_ID_EXTEND 0x1ff

/*
motor data read
*/

static CAN_TxHeaderTypeDef  motor_tx_message;
static uint8_t              motor_can_send_data[8];

//½ÓÊÜÒ£¿ØÆ÷Ö¸Õë
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  if (hcan == &hcan1)
  {
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];
    if (hcan->Instance == CAN1)
    {
      HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &rx_header, rx_data);
    }
    if ((rx_header.StdId >= FEEDBACK_ID_BASE) && (rx_header.StdId < FEEDBACK_ID_BASE + MOTOR_3508_NUM + MOTOR_2006_NUM))
    {
      uint8_t index = rx_header.StdId - FEEDBACK_ID_BASE;
      (dart.motor[index].motor_measure).ecd = ((rx_data[0] << 8) | rx_data[1]);
      (dart.motor[index].motor_measure).speed_rpm = ((rx_data[2] << 8) | rx_data[3]);
      (dart.motor[index].motor_measure).given_current = ((rx_data[4] << 8) | rx_data[5]);
      (dart.motor[index].motor_measure).temperate = rx_data[6];
    }
  }
}


void CAN_cmd_motor(uint8_t id_range,int16_t m1, int16_t m2, int16_t m3, int16_t m4)
{

    uint32_t send_mail_box;
    motor_tx_message.StdId = (id_range == 0)?(0x200):(0x1ff);
    motor_tx_message.IDE = CAN_ID_STD;
    motor_tx_message.RTR = CAN_RTR_DATA;  
    motor_tx_message.DLC = 0x08;  
    motor_can_send_data[0] = (m1 >> 8);
    motor_can_send_data[1] = m1;
    motor_can_send_data[2] = (m2 >> 8);
    motor_can_send_data[3] = m2;
    motor_can_send_data[4] = (m3 >> 8);  
    motor_can_send_data[5] = m3;
    motor_can_send_data[6] = (m4 >> 8);
    motor_can_send_data[7] = m4;
    HAL_CAN_AddTxMessage(&hcan1, &motor_tx_message, motor_can_send_data, &send_mail_box);
}

// void CAN_cmd_motor_2(uint8_t id_range,int16_t m1)
// {
//     uint32_t send_mail_box;
//     motor_tx_message.StdId = 0x1FF;
//     motor_tx_message.IDE = CAN_ID_STD;
//     motor_tx_message.RTR = CAN_RTR_DATA;  
//     motor_tx_message.DLC = 0x08;  
//     motor_can_send_data[0] = (m1 >> 8);
//     motor_can_send_data[1] = m1;
//     motor_can_send_data[2] = (m1 >> 8);
//     motor_can_send_data[3] = m1;
//     motor_can_send_data[4] = (m1 >> 8);  
//     motor_can_send_data[5] = m1;
//     motor_can_send_data[6] = (m1 >> 8);
//     motor_can_send_data[7] = m1;
//     HAL_CAN_AddTxMessage(&hcan1, &motor_tx_message, motor_can_send_data, &send_mail_box);
// }

void CAN_cmd_motor_reset_ID(void)
{
    uint32_t send_mail_box;
    motor_tx_message.StdId = 0x200; 
    motor_tx_message.IDE = CAN_ID_STD;
    motor_tx_message.RTR = CAN_RTR_DATA;  
    motor_tx_message.DLC = 0x08;
    motor_can_send_data[0] = 0;
    motor_can_send_data[1] = 0;
    motor_can_send_data[2] = 0;
    motor_can_send_data[3] = 0;
    motor_can_send_data[4] = 0; 
    motor_can_send_data[5] = 0;
    motor_can_send_data[6] = 0;
    motor_can_send_data[7] = 0;
    HAL_CAN_AddTxMessage(&hcan1,  &motor_tx_message, motor_can_send_data, &send_mail_box);
}

//-------------------------------------------------------------------------------

