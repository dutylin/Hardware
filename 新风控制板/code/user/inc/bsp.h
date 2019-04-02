#ifndef _BSP_H
#define _BSP_H
/*led driver*/
#define TM1629A_DIO_PORT	       GPIOA
#define TM1629A_DIO         	       GPIO_PIN_1
#define TM1629A_CLK_STB_PORT           GPIOD  
#define TM1629A_CLK		       GPIO_PIN_4
#define STB		               GPIO_PIN_7
/*iic driver*/
#define MCU_IIC_PORT		       GPIOA
#define MCU_SCL			       GPIO_PIN_2
#define MCU_SDA			       GPIO_PIN_3

/*iic driver*/
#define SENSOR_IIC_SCK_PORT	       GPIOF
#define SENSOR_IIC_SDA_PORT	       GPIOB	
#define SENSOR_SCL		       GPIO_PIN_4
#define SENSOR_SDA		       GPIO_PIN_7

/*buzzer driver*/
#define BUZZER_PORT		       GPIOB
#define BUZZER			       GPIO_PIN_6

#define PWM_PORT		       GPIOB
#define PWM_10KHZ		       GPIO_PIN_5
#define PWM_10KHZ1		       GPIO_PIN_4
#define PWM_50HZ		       GPIO_PIN_3

#define K37_PORT		       GPIOB
#define K3			       GPIO_PIN_2
#define K7			       GPIO_PIN_1

#define ADC_PORT		       GPIOB
#define AD			       GPIO_PIN_0

#define K6_PORT		       	       GPIOC
#define K4_PORT		               GPIOE
#define K4             		       GPIO_PIN_5
#define K6	                       GPIO_PIN_1

#define RF_PORT		       	       GPIOC
#define MCU_PDN			       GPIO_PIN_2	
#define MCU_IRQ			       GPIO_PIN_3
#define MCU_CS			       GPIO_PIN_4
#define MCU_SCK			       GPIO_PIN_5
#define MCU_MOSI			       GPIO_PIN_6
#define MCU_MISO			       GPIO_PIN_7
#define MCU_RF_GPIO0_PORT	       GPIOD
#define MCU_RF_GPIO0		       GPIO_PIN_0

#define IRED_PORT                      GPIOD
#define IRED			       GPIO_PIN_2

#define IN_PORT                        GPIOD  
#define IN			       GPIO_PIN_3


#define FEED_PORT             	       GPIOD  
#define FEED		               GPIO_PIN_5


void CLK_INIT(void);
#endif 
