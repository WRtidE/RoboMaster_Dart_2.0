#include "bsp_can.h"
#include "main.h"


extern CAN_HandleTypeDef hcan1;


void can_filter_init(void)
{

    CAN_FilterTypeDef can_filter_st;
    can_filter_st.FilterActivation = ENABLE;
    can_filter_st.FilterMode = CAN_FILTERMODE_IDMASK;
    can_filter_st.FilterScale = CAN_FILTERSCALE_32BIT;
    can_filter_st.FilterIdHigh = 0x0000;
    can_filter_st.FilterIdLow = 0x0000;
    can_filter_st.FilterMaskIdHigh = 0x0000;
    can_filter_st.FilterMaskIdLow = 0x0000;
    can_filter_st.FilterBank = 0;
    can_filter_st.FilterFIFOAssignment = CAN_RX_FIFO0;
    HAL_CAN_ConfigFilter(&hcan1, &can_filter_st);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);


    can_filter_st.SlaveStartFilterBank = 14;
    can_filter_st.FilterBank = 14;
}

void CAN_AlternateConfig()
{

  // 本函数用于配置can必需的配置：过滤器、使能、开启回调
  CAN_FilterTypeDef canfilterconfig;

  canfilterconfig.FilterActivation = CAN_FILTER_ENABLE;
  canfilterconfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  canfilterconfig.FilterIdHigh = 0x000;
  canfilterconfig.FilterIdLow = 0;
  canfilterconfig.FilterMaskIdHigh = 0x000;
  canfilterconfig.FilterMaskIdLow = 0x000;
  canfilterconfig.FilterMode = CAN_FILTERMODE_IDMASK;
  canfilterconfig.FilterScale = CAN_FILTERSCALE_32BIT;
  canfilterconfig.FilterBank = 0;            // which filter bank to use from the assigned ones
  canfilterconfig.SlaveStartFilterBank = 14; // how many filters to assign to the CAN1 (master can)

  if (HAL_CAN_ConfigFilter(&hcan1, &canfilterconfig) != HAL_OK)
    Error_Handler();
  if (HAL_CAN_Start(&hcan1) != HAL_OK)
    Error_Handler();
  if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
    Error_Handler();

  canfilterconfig.FilterBank = 14;            // which filter bank to use from the assigned ones
  canfilterconfig.SlaveStartFilterBank = 14; // how many filters to assign to the CAN1 (master can)

//   if (HAL_CAN_ConfigFilter(&hcan2, &canfilterconfig) != HAL_OK)
//     Error_Handler();
//   if (HAL_CAN_Start(&hcan2) != HAL_OK)
//     Error_Handler();
//   if (HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
//     Error_Handler();
}