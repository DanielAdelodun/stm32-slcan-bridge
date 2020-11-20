#ifndef _LED_H
#define _LED_H

#ifndef LD1_Pin 			// Green LED
#define LD1_Pin GPIO_PIN_0
#define LD1_GPIO_Port GPIOB
#endif

#ifndef LD2_Pin				// Blue LED
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB
#endif

#ifndef LD3_Pin 			// Red LED
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#endif

#define LED_OFF_DURATION 50
#define LED_RED_DURATION 200
#define LED_BLUE_DURATION 200
#define LED_GREEN_DURATION 200

void LED_RedForceOn(void);
void LED_RedOn(void);
void LED_BlueOn(void);
void LED_GreenOn(void);
void LED_Process(void);

#endif
