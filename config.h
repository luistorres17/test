#include <stm32f10x.h>

/*--- Registros del Controlador de Reloj (RCC) ---*/
#define RCC_CR      (*(volatile uint32_t*)0x40021000)
#define RCC_CFGR    (*(volatile uint32_t*)0x40021004)
#define RCC_APB2ENR (*(volatile uint32_t*)0x40021018)

/*--- Registros del Puerto GPIOA ---*/
#define GPIOA_CRL   (*(volatile uint32_t*)0x40010800)
#define GPIOA_CRH   (*(volatile uint32_t*)0x40010804)
#define GPIOA_ODR   (*(volatile uint32_t*)0x4001080C)

/*--- Registros del Periférico SPI1 ---*/
#define SPI1_CR1    (*(volatile uint32_t*)0x40013000)
#define SPI1_SR     (*(volatile uint32_t*)0x40013008)
#define SPI1_DR     (*(volatile uint32_t*)0x4001300C)

/*--- Registros del Timer 1 (TIM1) ---*/
#define TIM1_CR1    (*(volatile uint32_t*)0x40012C00) 
#define TIM1_SMCR   (*(volatile uint32_t*)0x40012C08) 
#define TIM1_CCMR1  (*(volatile uint32_t*)0x40012C18) 
#define TIM1_CCER   (*(volatile uint32_t*)0x40012C20) 
#define TIM1_CNT    (*(volatile uint32_t*)0x40012C24) 
#define TIM1_ARR    (*(volatile uint32_t*)0x40012C2C)
#define TIM1_PSC    (*(volatile uint32_t*)0x40012C28) //se adiciona psc
#define TIM1_SR     (*(volatile uint32_t*)0x40012C10)

//REGISTRO GPIO C
#define GPIOC_CRH     (*(volatile uint32_t*)0x40011004)
#define GPIOC_BSRR    (*(volatile uint32_t*)0x40011010)
	
//REGISTRO GPIO B
#define GPIOB_CRL     (*(volatile uint32_t*)0x40010C00)
#define GPIOB_ODR     (*(volatile uint32_t*)0x4001080C)
#define GPIOB_IDR     (*(volatile uint32_t*)0x40010808)




// --- Prototipos de Funciones ---
void Clock_Init(void);
void SPI1_Init(void);
void SPI1_Init_nokia(void);
void TIM1_Counter_Init(void);
void TIM1_Timer_Init(void);
void p_pullup(void);


