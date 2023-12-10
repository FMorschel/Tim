/*
Este é o clássico dos clássicos. Elabore um programa capaz de piscar o Led
com um ciclo de trabalho de 50% (metade do tempo ligado, metade
desligado) e com período de 200ms (5 Hz ou 5 piscadas/segundo). 
O led é ativo em LOW e conectado ao PC0. 
Utilize o TIM11.
*/

/* Includes */
#include "stm32f4xx.h"

/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */

int main(void)
{

	//Ligando clocks dos perifericos
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN;
	RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;

	// Definindo pino como saida
	GPIOC->MODER &= ~GPIO_MODER_MODER0;
	GPIOC->MODER |= GPIO_MODER_MODER0_0;

	// Configurando pino para chave
	GPIOB->MODER &= ~GPIO_MODER_MODER0;

	// Configurando o timer para 10Hz
	TIM11->PSC = 999;
	TIM11->ARR = 1599;
	TIM11->CR1 = TIM_CR1_CEN;

	while (1)
	{

		if ((GPIOB->IDR & GPIO_IDR_IDR0) == 0) {
			if (TIM11->SR & TIM_SR_UIF) {
				TIM11->SR &= ~TIM_SR_UIF;

				if (GPIOC->ODR & GPIO_ODR_ODR0) {
					GPIOC->ODR &= ~GPIO_ODR_ODR0; //0001000X & 11111110 = 00010000
				} else {
					GPIOC->ODR |= GPIO_ODR_ODR0;  //0001000X | 00000001 = 00010001
				}
			}
		}

	}
}
