#include "usart.h"
#include "stm32wbxx_hal_uart.h"

#define UART_TIMEOUT 1000 // ms

int __io_putchar(int ch) {
	// TODO: transmit 'ch' byte through ST-LINK UART
	// apps/sample-app/Core/Src/syscalls.c:  HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, , 0xFFFF);
	// UartHandle: apps/sample-app/Core/Src/usart.c UART_HandleTypeDef huart1;
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, sizeof((uint8_t )ch), 0xFFFF);
	return ch;
}

int _write(int file, char *ptr, int len) {
	int i;
	for (i = 0; i < len; i++) {
		__io_putchar(*ptr++);
	}
	return len;
}