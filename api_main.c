#include "uart_main.h"
#include "hal_gpio_driver.h"
#include "hal_uart_driver.h"
#include "uart_debug.h"



uart_handle_t uart_handle, debug_handle;




uint8_t message[] = {0x3, 0x5, 0x7, 0x8}
uint8_t rx_buffer[4];
uint8_t i;

//API to set the message id to pass through
void get_message_id(uint8_t id)
{
	i = id;
}


// this function parses the command and takes action 
void 	parse_cmd(uint8_t *rx_buffer)
{
	AF_IDMASKx|= rx_buffer>>4;
	
	AF_IDVALUEx|= rx_buffer 
	
	place_fifo(message(i));
	
}

//API pseudo code for configuring the message
void place_fifo(uint8_t message_id)
{
	
If( (AF_IDMASKx & AF_IDVALUEx) == (AF_IDMASKx & message.id) )
{
Mailbox_put(message);
}

}


//configures the gpio pin and sets it to 
void uart_gpio_init(void)
{
	gpio_configure();

}



/*This callback will be called by the driver when the application receives the command */
void app_rx_cmp_callback(void *size)
{
	//we got a command,  parse it 
	parse_cmd(rx_buffer);
	
}


int main(void)
{
	
	uint32_t val;


	/*configures GPIOs for UART functionality */
	uart_gpio_init();
	
	/*enable the clock for the USART2 Peripheral */
	_HAL_RCC_USART2_CLK_ENABLE();   
	
	uart_handle.Instance          = USART_2;

	uart_handle.Init.BaudRate     = USART_BAUD_9600;
	uart_handle.Init.WordLength   = USART_WL_1S8B;
	uart_handle.Init.StopBits     = UART_STOPBITS_1;
	uart_handle.Init.Parity       = UART_PARITY_NONE;
	uart_handle.Init.Mode         = UART_MODE_TX_RX;
	uart_handle.Init.OverSampling = USART_OVER16_ENABLE;

	uart_handle.rx_cmp_cb = app_rx_cmp_callback;
	
	 hal_uart_init(&uart_handle);

/*enable the IRQ of USART2 peripheral */
	 NVIC_EnableIRQ(USARTx_IRQn);



while(1)
{
  while(uart_handle.rx_state != HAL_UART_STATE_READY );
	/*receive the message */
	hal_uart_rx(&uart_handle,rx_buffer, 5 );
}

	return 0;
}

//This is the ISR for the USARTx interrupt , defined in the hal_uart_driver.h
void USARTx_IRQHandler(void)
{
  hal_uart_handle_interrupt(&uart_handle);
}