/*
* LAB: 5
* Name: ESP32 PWM LED Control
* Author: Khaled Magdy
* For More Info Visit: www.DeepBlueMbedded.com
*/
 
#define LED_GPIO   5
#define PWM1_Ch    0
#define PWM1_Res   4
#define PWM1_Freq  5000
 
int PWM1_DutyCycle = 0;
 
void setup()
{
  ledcAttachPin(LED_GPIO, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);
}
 
void loop()
{
  while(PWM1_DutyCycle < 16)
  {
    ledcWrite(PWM1_Ch, PWM1_DutyCycle++);
    delay(50);
  }
  while(PWM1_DutyCycle > 0)
  {
    ledcWrite(PWM1_Ch, PWM1_DutyCycle--);
    delay(50);
  }

//  ledcWrite(PWM1_Ch, 0);
//  delay(3000);
//  ledcWrite(PWM1_Ch, 7);
//  delay(3000);
//  ledcWrite(PWM1_Ch, 15);
//  delay(3000);
}
