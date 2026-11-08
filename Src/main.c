#include <stdint.h>
#include <stdio.h>
#include <unistd.h>


#define RCC_AHB1ENR (*(volatile uint32_t*)0x40023830)
#define RCC_APB1ENR (*(volatile uint32_t*)0x40023840)

#define GPIOA_MODER (*(volatile uint32_t*)0x40020000)
#define GPIOA_ODR   (*(volatile uint32_t*)0x40020014)
#define GPIOA_AFRL  (*(volatile uint32_t*)0x40020020)

#define USART2_SR   (*(volatile uint32_t*)0x40004400)
#define USART2_DR   (*(volatile uint32_t*)0x40004404)
#define USART2_BRR  (*(volatile uint32_t*)0x40004408)
#define USART2_CR1  (*(volatile uint32_t*)0x4000440C)

static void uart_init(void) {
    RCC_APB1ENR |= (1U << 17);               // USART2 clock
    GPIOA_MODER &= ~(3U << 4);               // PA2 alternate function
    GPIOA_MODER |=  (2U << 4);
    GPIOA_AFRL  &= ~(0xFU << 8);             // PA2 = AF7 (USART2_TX)
    GPIOA_AFRL  |=  (7U << 8);
    USART2_BRR = 139;
    USART2_CR1 |= (1U << 13) | (1U << 3);    // UE + TE
}

static void uart_putc(char c) {
    while (!(USART2_SR & (1U << 7))) {}      // wait for TXE
    USART2_DR = c;

}

static void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

int _write(int f,char *p, int len)
{
	(void)f;
	for (int i = 0; i < len; i++){

		while (!(USART2_SR & (1U << 7))) {}
		USART2_BRR = p[i];
	}
	return len;
}






int main(void) {
    RCC_AHB1ENR |= (1U << 0);                // GPIOA clock
    GPIOA_MODER &= ~(3U << 10);              // PA5 output (LED)
    GPIOA_MODER |=  (1U << 10);

    uart_init();

    while (1) {
        GPIOA_ODR ^= (1U << 5);
        uart_puts("fornite battle pass\r\n");
        uart_puts("sideways stinks\r\n");


        for (volatile uint32_t i = 0; i < 1000000; i++);
    }
}
