
#include "hal_gpio_driver.h"
#include "stm32f4xx.h"
#include "led.h"




/**
	* @brief  Enable the given USART peripheral 
	* @param  *uartx : base address of the USART or UART peripheral
	* @retval None
	*/
void hal_uart_enable(USART_TypeDef *uartx)
{
	
	uartx->CR1 |= USART_REG_CR1_USART_EN;
}

/**
	* @brief  Disable the given USART peripheral 
	* @param  *uartx : base address of the USART or UART peripheral
	* @retval None
	*/
void hal_uart_disable(USART_TypeDef *uartx)
{
	uartx->CR1 &= ~USART_REG_CR1_USART_EN;
}



/**
	* @brief  Enable/Disable the Receive block of the  given USART peripheral 
	* @param  *uartx : base address of the USART or UART peripheral
  * @param  te    : if re=1, then enable the Receive block.
	* @retval None
	*/
void hal_uart_enable_disable_rx(USART_TypeDef *uartx, uint32_t re)
{
	if(re & USART_REG_CR1_RE)
	{
		uartx->CR1 |= USART_REG_CR1_RE;
	}else
	{
		uartx->CR1 &= ~USART_REG_CR1_RE;
	}
	
}




/**
	* @brief  Enable/Disable the RXNE interrupt  
	* @param  *uartx : base address of the USART or UART peripheral
  * @param  rxne_en     : if rxne_en =1, then enable the RXNE interrupt 
  * @retval None
	*/
void hal_uart_configure_rxne_interrupt(USART_TypeDef *uartx, uint32_t rxne_en)
{
	if(rxne_en)
		uartx->CR1 |= USART_REG_CR1_RXNE_INT_EN;
	else
		uartx->CR1 &= ~USART_REG_CR1_RXNE_INT_EN;
}



/**
  * @brief  Handle the RXNE interrupt 
  * @param  huart: pointer to a uart_handle_t structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
static void hal_uart_handle_RXNE_interrupt(uart_handle_t *huart)
{
 
  uint32_t tmp1 = 0;
  
  tmp1 = huart->rx_state; 
	
  if( tmp1 == HAL_UART_STATE_BUSY_RX )
  {
	
		*(huart->pRxBuffPtr++) = (uint8_t)(huart->Instance->DR);
		
		/* are we done with the reception ?? */
		if(--(huart->RxXferCount) == 0)
		{
			//disable all interrupts
			disable_interrupts();

			/*call the applicaton callback */
			if(huart->rx_cmp_cb)
					huart->rx_cmp_cb(&huart->RxXferSize);
		}
   }
}
  


/**
	* @brief  Clear the error flag 
	* @param  *huart : pointer to a uart_handle_t structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
	*/
void hal_uart_clear_error_flag(uart_handle_t *huart)
{
	uint32_t tmpreg = 0x00;                
	tmpreg = huart->Instance->SR;        
	tmpreg = huart->Instance->DR;        
	
}


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
void hal_uart_init(uart_handle_t *uart_handle)
{
   // initialize and configure all appropriate registers in UART
	 UART_init_reg();
			
}



/**
	* @brief  API to do UART data Reception  
	* @param  *handle : pointer to the handle structure of the UART peripheral  
  * @param  *buffer : holds the pointer to the RX buffer 
  * @param  len : len of the data to be RXed
	* @retval None
	*/
void hal_uart_rx(uart_handle_t *uart_handle, uint8_t *buffer, uint32_t len)
{
	uint32_t val;
/*Populate the application given inforamtions in to the UART handle structure */
	uart_handle->pRxBuffPtr = buffer;
	uart_handle->RxXferCount = len;
	uart_handle->RxXferSize = len;

	/* This handle is busy in doing the RX */
	uart_handle->rx_state = HAL_UART_STATE_BUSY_RX;

	/* Enable the UART Parity Error Interrupt */
	hal_uart_configure_parity_error_interrupt(uart_handle->Instance,1);


	/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	hal_uart_configure_error_interrupt(uart_handle->Instance,1);

	val = uart_handle->Instance->DR;
	/* Enable the UART Data Register not empty Interrupt */
	hal_uart_configure_rxne_interrupt(uart_handle->Instance,1);
	
}


/**
  * @brief  This function handles UART interrupt request.
  * @param  huart: pointer to a uart_handle_t structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void hal_uart_handle_interrupt(uart_handle_t *huart)
{
    uint32_t tmp1 = 0, tmp2 = 0;

	 tmp1 = huart->Instance->SR & USART_REG_SR_RXNE_FLAG;
   tmp2 = huart->Instance->CR1 & USART_REG_CR1_RXNE_INT_EN;
  /* UART in mode Receiver ---------------------------------------------------*/
  if((tmp1 ) && (tmp2))
  { 
    hal_uart_handle_RXNE_interrupt(huart);
  }
  
  

}