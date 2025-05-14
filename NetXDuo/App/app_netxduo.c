/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_netxduo.c
  * @author  MCD Application Team
  * @brief   NetXDuo applicative file
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
#include "app_netxduo.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "nx_api.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// Memory sizes
#define NX_PACKET_POOL_SIZE      ((1536 + sizeof(NX_PACKET)) * 10)
#define DEFAULT_IP_THREAD_STACK_SIZE 2048
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
// IP address and mask are placeholders (DHCP will override)
#define IP_ADDRESS( a, b, c, d )  ((ULONG)((a << 24) | (b << 16) | (c << 8) | d))
#define IP_MASK( a, b, c, d )     ((ULONG)((a << 24) | (b << 16) | (c << 8) | d))
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
// NetX memory buffers
static ULONG ip_stack[DEFAULT_IP_THREAD_STACK_SIZE / sizeof(ULONG)];
static ULONG packet_pool_area[NX_PACKET_POOL_SIZE / sizeof(ULONG)];

// Exported variables
NX_IP ip;
NX_PACKET_POOL packet_pool;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
extern VOID _nx_driver_stm32h7(NX_IP_DRIVER *driver_req_ptr);
/* USER CODE END PFP */

/**
  * @brief  Application NetXDuo Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  * Initializes NetX Duo stack.
  * Called from MX_NetXDuo_Init in generated code.
  */
UINT MX_NetXDuo_Init(VOID *memory_ptr)
{
  UINT ret = NX_SUCCESS;
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;

  /* USER CODE BEGIN MX_NetXDuo_MEM_POOL */
  (void)byte_pool;
  // Initialize NetX system
  nx_system_initialize();

  // Create packet pool
  ret = nx_packet_pool_create(&packet_pool, "NetX Packet Pool",
                                  1536, (UCHAR*)packet_pool_area,
                                  NX_PACKET_POOL_SIZE);
  if (ret != NX_SUCCESS) return ret;

  // Create IP instance with dummy address (DHCP will override)
  ret = nx_ip_create(&ip, "NetX IP Instance",
                        IP_ADDRESS(0, 0, 0, 0),
                        IP_MASK(0, 0, 0, 0),
                        &packet_pool, _nx_driver_stm32h7,
                        (UCHAR*)ip_stack, sizeof(ip_stack), 1);
  if (ret != NX_SUCCESS) return ret;

  // Enable required protocols
  ret = nx_ip_fragment_enable(&ip);
  if (ret != NX_SUCCESS) return ret;

  ret = nx_ip_forwarding_disable(&ip);
  if (ret != NX_SUCCESS) return ret;

  ret = nx_ip_udp_enable(&ip);
  if (ret != NX_SUCCESS) return ret;

  ret = nx_ip_tcp_enable(&ip);
  if (ret != NX_SUCCESS) return ret;

  // Optional: enable ARP
  ret = nx_arp_enable(&ip, (UCHAR *)byte_pool, 1024);
  if (ret != NX_SUCCESS) return ret;

  // Optional: enable ICMP (e.g. ping)
  ret = nx_icmp_enable(&ip);

  /* USER CODE END MX_NetXDuo_MEM_POOL */

  /* USER CODE BEGIN 0 */

  /* USER CODE END 0 */

  /* USER CODE BEGIN MX_NetXDuo_Init */

  /* USER CODE END MX_NetXDuo_Init */

  return ret;
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
