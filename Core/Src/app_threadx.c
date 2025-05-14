/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2020-2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "main.h"
#include "tx_api.h"
#include "nx_api.h"
#include "nxd_dhcp_client.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
// LED control macros (assumes LD1 = PB0, LD3 = PB14)
#define LED_GREEN_ON()    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)
#define LED_GREEN_OFF()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)
#define LED_RED_ON()      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET)
#define LED_RED_OFF()     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET)

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TX_THREAD tx_app_thread;
TX_SEMAPHORE tx_app_semaphore;
TX_MUTEX tx_app_mutex;
/* USER CODE BEGIN PV */
// External NetX objects declared in app_netxduo.c
extern NX_IP NetXDuoEthIpInstance;
extern NX_PACKET_POOL packet_pool;
NX_DHCP dhcp_client;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  /* USER CODE BEGIN App_ThreadX_MEM_POOL */

  /* USER CODE END App_ThreadX_MEM_POOL */
  CHAR *pointer;

  /* Allocate the stack for tx app thread  */
  if (tx_byte_allocate(byte_pool, (VOID**) &pointer,
                       TX_APP_STACK_SIZE, TX_NO_WAIT) != TX_SUCCESS)
  {
    return TX_POOL_ERROR;
  }
  /* Create tx app thread.  */
  if (tx_thread_create(&tx_app_thread, "tx app thread", tx_app_thread_entry, 0, pointer,
                       TX_APP_STACK_SIZE, TX_APP_THREAD_PRIO, TX_APP_THREAD_PREEMPTION_THRESHOLD,
                       TX_APP_THREAD_TIME_SLICE, TX_APP_THREAD_AUTO_START) != TX_SUCCESS)
  {
    return TX_THREAD_ERROR;
  }

  /* Create tx app semaphore.  */
  if (tx_semaphore_create(&tx_app_semaphore, "tx app semaphore", 0) != TX_SUCCESS)
  {
    return TX_SEMAPHORE_ERROR;
  }

  /* Create tx app mutex.  */
  if (tx_mutex_create(&tx_app_mutex, "tx app mutex", TX_NO_INHERIT) != TX_SUCCESS)
  {
    return TX_MUTEX_ERROR;
  }

  /* USER CODE BEGIN App_ThreadX_Init */

  /* USER CODE END App_ThreadX_Init */

  return ret;
}
/**
  * @brief  Function implementing the tx_app_thread_entry thread.
  * @param  thread_input: Hardcoded to 0.
  * @retval None
  */
void tx_app_thread_entry(ULONG thread_input)
{
  /* USER CODE BEGIN tx_app_thread_entry */
  UINT status;
  ULONG actual_status;
  ULONG ip_address;
  ULONG netmask;

  // not used
  (void)thread_input;

  // Initial state: red on, green off
  LED_GREEN_OFF();
  LED_RED_ON();

  while (1)
  {
      // Wait for physical Ethernet link
      status = nx_ip_interface_status_check(&NetXDuoEthIpInstance, 0, NX_IP_LINK_ENABLED, &actual_status, 100);
      if (status == NX_SUCCESS && actual_status == NX_IP_LINK_ENABLED)
      {
          // Link is up, create and start DHCP client
          nx_dhcp_create(&dhcp_client, &NetXDuoEthIpInstance, "DHCP Client");
          nx_dhcp_start(&dhcp_client);

          // Wait until an IP address is assigned
          while (1)
          {
              nx_ip_address_get(&NetXDuoEthIpInstance, &ip_address, &netmask);
              if (ip_address != 0)
              {
                  LED_GREEN_ON();
                  LED_RED_OFF();
                  break;
              }
              tx_thread_sleep(50);
          }

          // Remain here while link is up
          while (1)
          {
              status = nx_ip_interface_status_check(&NetXDuoEthIpInstance, 0, NX_IP_LINK_ENABLED, &actual_status, 100);
              if (status != NX_SUCCESS || actual_status != NX_IP_LINK_ENABLED)
              {
                  break;  // Link down
              }
              tx_thread_sleep(50);
          }

          // Link down — revert to red LED
          LED_GREEN_OFF();
          LED_RED_ON();

          nx_dhcp_stop(&dhcp_client);
          nx_dhcp_delete(&dhcp_client);
      }

      tx_thread_sleep(100); // Polling delay
  }
  /* USER CODE END tx_app_thread_entry */
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN  Before_Kernel_Start */

  /* USER CODE END  Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN  Kernel_Start_Error */

  /* USER CODE END  Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
