#include <Arduino.h>
#define LED_PIN_Yellow 18
#define LED_PIN_Red 19
#define PWM_CH_Yellow 0
#define PWM_CH_Red 1
#define FREQ 5000
#define RES 8
void setup() {
  ledcSetup(PWM_CH_Yellow, FREQ, RES);
  ledcSetup(PWM_CH_Red, FREQ, RES);
  ledcAttachPin(LED_PIN_Yellow, PWM_CH_Yellow);
  ledcAttachPin(LED_PIN_Red, PWM_CH_Red);
}
void loop() { 
  for (int d=0; d<=255; d++) { 
  ledcWrite(PWM_CH_Yellow, d); 
  ledcWrite(PWM_CH_Red, 255-d); 
  delay(15); 
  } 
  for (int d=255; d>=0; d--) { 
  ledcWrite(PWM_CH_Yellow, d); 
  ledcWrite(PWM_CH_Red, 255-d); 
  delay(15); 
  } 
} 