#ifndef __HAL_UART_DRIVER_H
#define __HAL_UART_DRIVER_H

#include "stm32f407xx.h"

	
/** 
  * @brief HAL UART State structures definition  
  */ 
typedef enum
{
  HAL_UART_STATE_RESET             = 0x00,    /*!< Peripheral is not yet Initialized                  */
  HAL_UART_STATE_READY             = 0x01,    /*!< Peripheral Initialized and ready for use           */
  HAL_UART_STATE_BUSY              = 0x02,    /*!< an internal process is ongoing                     */   
  HAL_UART_STATE_BUSY_TX           = 0x12,    /*!< Data Transmission process is ongoing               */ 
  HAL_UART_STATE_BUSY_RX           = 0x22,    /*!< Data Reception process is ongoing                  */
  HAL_UART_STATE_BUSY_TX_RX        = 0x32,    /*!< Data Transmission and Reception process is ongoing */        
}hal_uart_state_t;


/*Different USART and UART peripheral base addresses */
#define USART_1 USART1
#define USART_2 USART2
#define USART_3 USART3
#define USART_4 UART4
#define USART_5 USART5
#define USART_6 USART6



#define UART_MODE_TX_RX                     			((uint32_t)(USART_REG_CR1_TE |USART_REG_CR1_RE))


#define UNUSED(x) ((void)(x))


/******************************************************************************/
/*                                                                            */
/*                      Data Structures used by UART Driver                    */
/*                                                                            */
/******************************************************************************/



	
/** 
  * @brief UART Init Structure definition  
  */ 
typedef struct
{
	uint32_t BaudRate;                  /* This member configures the UART communication baud rate */
																 
	uint32_t WordLength;                /* Specifies the number of data bits transmitted or received in a frame */
																 
	uint32_t StopBits;                  /* Specifies the number of stop bits transmitted */
																
	uint32_t Parity;                    /* Specifies the parity mode. */
											

	uint32_t Mode;                      /*  Specifies whether the Receive or Transmit mode is enabled or disabled */

	uint32_t OverSampling;              /*  Specifies whether the Over sampling 8 is enabled or disabled */

}uart_init_t;

/*Application callbacks typedef */
typedef void (	TX_COMP_CB_t) (void *ptr);
typedef void (	RX_COMP_CB_t) (void *ptr);

/** 
  * @brief  UART handle Structure definition  
  */  
typedef struct
{
	USART_TypeDef                 *Instance;        /* UART registers base address        */

	uart_init_t                    Init;            /* UART communication parameters      */

	uint8_t                       *pRxBuffPtr;      /* Pointer to UART Rx transfer Buffer */

	uint16_t                      RxXferSize;       /* UART Rx Transfer size              */

	uint16_t                      RxXferCount;      /* UART Rx Transfer Counter           */  

	hal_uart_state_t          		rx_state;         /* UART communication state           */
	
	RX_COMP_CB_t            			*rx_cmp_cb ;      /* Application callback when RX Completed */
	
}uart_handle_t;





/******************************************************************************/
/*                                                                            */
/*                      Driver exposed APIs                                   */
/*                                                                            */
/******************************************************************************/

/**
	* @brief  API to do UART Peripheral initialization   
	* @param  *handle : pointer to the handle structure of the UART peripheral  
	* @retval None
	*/	
void hal_uart_init(uart_handle_t *handle);

/**
	* @brief  API to do UART data Transmission
	* @param  *uart_handle : pointer to the handle structure of the UART Peripheral 
  * @param  *buffer : holds the pointer to the TX buffer 
  * @param  len : len of the data to be TXed
	* @retval None
	*/

void hal_uart_rx( uart_handle_t *handle,uint8_t *buffer, uint32_t len);


/**
  * @brief  This API handles UART interrupt request.
  * @param  huart: pointer to a uart_handle_t structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void hal_uart_handle_interrupt(uart_handle_t *huart);

#endif 