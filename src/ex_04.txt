/*
Monte dois conjuntos de 3 leds, um verde, um amarelo e um vermelho, nas
posições de um semáforo (se não tiveres as cores, não importa.).

Desenvolva um programa de controle para este semáforo de dois tempos. 
O tempo de verde/vermelho deve ser de 700 ms e 
o tempo de amarelo aceso é 200ms. 

Faça a sequência 	Vd →	 Vd/Am → 	Vm
					500ms	 200ms		700ms
*/

/* Includes */
#include "stm32f4xx.h"

/* Private macro */
/*typedef struct {
	uint32_t vermelho;
	uint32_t amarelo;
	uint32_t verde;
} semaforo_t;*/
#define VERMELHO1 GPIO_ODR_ODR_0
#define AMARELO1 GPIO_ODR_ODR_1
#define VERDE1 GPIO_ODR_ODR_2
#define VERMELHO2 GPIO_ODR_ODR_3
#define AMARELO2 GPIO_ODR_ODR_4
#define VERDE2 GPIO_ODR_ODR_5

/* Private variables */
uint8_t contador = 0;
/*const semaforo_t semaforo1 = {
	.vermelho = GPIO_ODR_ODR_0,
	.amarelo = GPIO_ODR_ODR_1,
	.verde = GPIO_ODR_ODR_2,
};
const semaforo_t semaforo2 = {
	.vermelho = GPIO_ODR_ODR_3,
	.amarelo = GPIO_ODR_ODR_4,
	.verde = GPIO_ODR_ODR_5,
};*/
/* Private function prototypes */
/* Private functions */

int main(void)
{
	//Ligando clocks dos perifericos
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	//RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; <- Outros timers
	RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;

	// Configurando pinos para saida
	GPIOB->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2
						| GPIO_MODER_MODER3 | GPIO_MODER_MODER4 | GPIO_MODER_MODER5);
	GPIOB->MODER |= (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0
						| GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0);

	// Configurando o timer para 10Hz
	TIM10->PSC = 999;
	TIM10->ARR = 1599;
	TIM10->CR1 = TIM_CR1_CEN;



	while (1)
	{
		if (TIM10->SR & TIM_SR_UIF) {
			TIM10->SR &= ~TIM_SR_UIF;
			contador++;

			if (contador == 5) {
				if (GPIOB->ODR & VERDE1) {
					GPIOB->ODR |= AMARELO1;
				} else {
					GPIOB->ODR |= AMARELO2;
				}
			} else if (contador == 7) {
				if (GPIOB->ODR & VERDE1) {
					GPIOB->ODR |= VERMELHO1;
					GPIOB->ODR &= ~VERDE1;
					GPIOB->ODR &= ~AMARELO1;

					GPIOB->ODR |= VERDE2;
					GPIOB->ODR &= ~VERMELHO2;
				} else {
					GPIOB->ODR |= VERMELHO2;
					GPIOB->ODR &= ~VERDE2;
					GPIOB->ODR &= ~AMARELO2;

					GPIOB->ODR |= VERDE1;
					GPIOB->ODR &= ~VERMELHO1;
				}
				contador = 0;
			}
		}
	}
}
