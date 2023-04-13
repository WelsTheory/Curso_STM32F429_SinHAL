/*
 * redirect_printf.c
 *
 *  Created on: Apr 13, 2023
 *      Author: Wels
 */
#include "USART.h"

int _write(int fd, char *ptr, int len);

int _write(int fd, char* ptr, int len) {
  // Fail if trying to write to anything besides standard output (FD = 1).
  if (fd != 1) {
    errno = EBADF;
    return -1;
    // 0 false
    // -1 -> archivo
    // -2 -> otro
  }
  // Send the data out the UART a character at a time.
  int i;
  for (i = 0; i < len; i++)
    USART_Tx( ptr[i]);
	//USART_TX_6(ptr[i]);
  return i;
}

