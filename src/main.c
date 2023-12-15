#include "stm32f4xx.h"

#define botao1 GPIO_MODER_MODER0
#define botao2 GPIO_MODER_MODER1

#define LEDA GPIO_ODR_ODR_0
#define LEDB GPIO_ODR_ODR_1
#define LEDC GPIO_ODR_ODR_2
#define LEDD GPIO_ODR_ODR_3
#define LEDE GPIO_ODR_ODR_4
#define LEDF GPIO_ODR_ODR_5
#define LEDG GPIO_ODR_ODR_6

#define PONTO GPIO_ODR_ODR_7

#define TAMANHO_VETOR 10
#define TOTAL (TAMANHO_VETOR + 1)

const uint8_t cronometro[TAMANHO_VETOR] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

const uint32_t leds[10] = {
		LEDA | LEDB | LEDC | LEDD | LEDE | LEDF,						   // 0000 | 0 | led = 0
		LEDB | LEDC,													   // 0001 | 1 | led = 1
		LEDA | LEDB | LEDD | LEDE | LEDG,			   					   // 0010 | 2 | led = 2
		LEDA | LEDB | LEDC | LEDD | LEDG,						   		   // 0011 | 3 | led = 3
		LEDB | LEDC | LEDF | LEDG,							      		   // 0100 | 4 | led = 4
		LEDA | LEDC | LEDD | LEDF | LEDG,			   					   // 0101 | 5 | led = 5
		LEDA | LEDC | LEDD | LEDE | LEDF | LEDG,						   // 0110 | 6 | led = 6
		LEDA | LEDB | LEDC,												   // 0111 | 7 | led = 7
		LEDA | LEDB | LEDC | LEDD | LEDE | LEDF | LEDG,					   // 1000 | 8 | led = 8
		LEDA | LEDB | LEDC | LEDD | LEDF | LEDG,						   // 1001 | 9 | led = 9
};

#define TODOS (LEDA | LEDB | LEDC | LEDD | LEDE | LEDF | LEDG)
#define anterior 0
#define atual 1

#define TEMPO_DEBOUNCE 20000

uint8_t estados_botao1[2] = {0, 0};
uint8_t estados_botao2[2] = {0, 0};

uint16_t contador = 0;

int main()
{
	// clock pinos GPIO c e b
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN;

	// clock timer
	RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;

	// entradas dos bot�es
	GPIOC->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1);

	GPIOC->MODER &= ~GPIO_MODER_MODER7;
	GPIOC->MODER |= GPIO_MODER_MODER7;

	// Configurando pinos saidas segmentos display
	GPIOB->MODER &= ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1 |GPIO_MODER_MODER2 | GPIO_MODER_MODER3 | GPIO_MODER_MODER4 |GPIO_MODER_MODER5 | GPIO_MODER_MODER6);
	GPIOB->MODER |= (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 |GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 |GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0);

	TIM10->PSC = 999;
	TIM10->ARR = 15999;
	TIM10->CR1 = TIM_CR1_CEN;

	GPIOB->ODR |= TODOS;

	//GPIOB->ODR |= leds[cronometro];

	while (1)
	{
		if (TIM10->SR & TIM_SR_UIF)
		{
			TIM10->SR &= ~TIM_SR_UIF;
			GPIOC->ODR ^= PONTO;

			// atualizando estado do botao 1
			estados_botao1[anterior] = estados_botao1[atual];
			if (GPIOC->IDR & botao1)
			{
				estados_botao1[atual] = 1;
				if (estados_botao1[atual] == 0)
				{
					for (int i = 0; i < TEMPO_DEBOUNCE; i++)
						;
				}
				if (GPIOC->IDR & botao1)
				{
					estados_botao1[atual] = 1;
				}
				else
				{
					estados_botao1[atual] = 0;
				}
			}
			else
			{
				estados_botao1[atual] = 0;
			}

			// atualizando estado do botao 2
			estados_botao2[anterior] = estados_botao2[atual];
			if (GPIOC->IDR & botao2)
			{
				estados_botao2[atual] = 1;
				if (estados_botao2[atual] == 0)
				{
					for (int i = 0; i < TEMPO_DEBOUNCE; i++);
				}
				if (GPIOC->IDR & botao2)
				{
					estados_botao2[atual] = 1;
				}
				else
				{
					estados_botao2[atual] = 0;
				}
			}
			else
			{
				estados_botao2[atual] = 0;
			}

			// Caso queira rodar algo quando o botao come�ar a ser pressionado
			// util para fazer algo apenas uma vez quando o botao for pressionado
			if (estados_botao1[anterior] == 0 && estados_botao1[atual] == 1)
			{
				//GPIOB->ODR &= ~TODOS;
				//GPIOB->ODR |= leds[2];

				contador++;

				if (contador == 0) {

					GPIOB->ODR |= TODOS;
					GPIOB -> ODR &=~ leds[0];
					TIM10->ARR = 15999; // 1HZ
				}

				if (contador == 1) {

					GPIOB->ODR |= TODOS;
					GPIOB -> ODR &=~ leds[1];
					TIM10->ARR = 1599; // 5HZ
				}
				if (contador == 2) {

					GPIOB->ODR |= TODOS;
					GPIOB -> ODR &=~ leds[2];
					TIM10->ARR = 1142; // 7HZ
				}
				if (contador == 3) {

					GPIOB->ODR |= TODOS;
					GPIOB -> ODR &=~ leds[3];
					TIM10->ARR = 888; // 9HZ
				}
				if (contador == 4) {

					GPIOB->ODR |= TODOS;
					GPIOB -> ODR &=~ leds[4];
					//GPIOC->ODR ^= PINO;
					TIM10->ARR = 726; // 11HZ
				}
				if (contador == 5) {

					GPIOB->ODR |= TODOS;
					GPIOB -> ODR &=~ leds[5];
					//GPIOC->ODR ^= PINO;
					TIM10->ARR = 614; // 13HZ
				}
				if (contador == 6) {

					GPIOB->ODR |= TODOS;
					GPIOB -> ODR &=~ leds[6];
					//GPIOC->ODR ^= PINO;
					TIM10->ARR = 532; // 15HZ
				}
				if (contador == 7) {

					GPIOB->ODR |= TODOS;
					GPIOB -> ODR &=~ leds[7];
					//GPIOC->ODR ^= PINO;
					TIM10->ARR = 470; // 17HZ
				}
				if (contador == 8) {

					GPIOB->ODR |= TODOS;
					GPIOB -> ODR &=~ leds[8];
					//GPIOC->ODR ^= PINO;
					TIM10->ARR = 420; // 19HZ
				}
				if (contador == 9) {

					GPIOB->ODR |= TODOS;
					GPIOB -> ODR &=~ leds[9];
					//GPIOC->ODR ^= PINO;
					TIM10->ARR = 380; // 21HZ
				}
			}

			if (estados_botao2[anterior] == 0 && estados_botao2[atual] == 1)
						{
							//GPIOB->ODR &= ~TODOS;
							//GPIOB->ODR |= leds[2];
					contador--;
					if (contador < 0 ) {
						contador = 0;
					}
					if (contador > 9) {
						contador = 9;
					}
						}
		}
	}
}
