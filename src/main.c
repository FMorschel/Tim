/*
Altere o exercício 1 para que a frequência da piscada seja
função de uma entrada E. 
O led é ativo em HIGH e conectado ao PB0.
Quando E estiver em nivel alto, o led pisca a 2 Hz, caso contrário pisca a 7 Hz.  
A entrada E será feita através do pino PC1. 
Utilize o TIM10.
*/

/* Includes */
#include "stm32f4xx.h"

/* Private macro */
#define F14HZ 1142
#define F4HZ 4000
/* Private variables */
/* Private function prototypes */
/* Private functions */

int main(void)
{

	//Ligando clocks dos perifericos
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN;
	//RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; <- Outros timers
	RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;

	// Definindo pino como chave
	GPIOC->MODER &= ~GPIO_MODER_MODER1;

	// Configurando pino para saida
	GPIOB->MODER &= ~GPIO_MODER_MODER0;
	GPIOB->MODER |= GPIO_MODER_MODER0_0;

	// Configurando o timer para 14Hz
	TIM10->PSC = 999;
	TIM10->ARR = F14HZ;
	TIM10->CR1 = TIM_CR1_CEN;

	while (1)
	{

		if ((GPIOB->IDR & GPIO_IDR_IDR0) == 0) {
			TIM10->ARR = F14HZ;
		} else {
			TIM10->ARR = F4HZ;
		}

		if (TIM10->CNT >= TIM10->ARR) {
			TIM10->CNT = 0;
		}

		if (TIM10->SR & TIM_SR_UIF) {
			TIM10->SR &= ~TIM_SR_UIF;

			if (GPIOC->ODR & GPIO_ODR_ODR0) {
				GPIOC->ODR &= ~GPIO_ODR_ODR0; //0001000X & 11111110 = 00010000
			} else {
				GPIOC->ODR |= GPIO_ODR_ODR0;  //0001000X | 00000001 = 00010001
			}
		}
	}
}
