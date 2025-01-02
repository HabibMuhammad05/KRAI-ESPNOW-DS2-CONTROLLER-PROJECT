/*---------------------------------------------------------------------------------------------------------*/
/*----------------------------PINOUT PCB PERGERAKAN AWAL, 6 IDC, ARDUINO MEGA, ESP32-----------------------*/
/*----------------------------------------------TES PWM 6 DRIVER-------------------------------------------*/
/*--------------------------------------Source Code by LEXARGA-24 TEAM-------------------------------------*/
/*-----------------------------------Modified & Adapted by LEXARGA-24 TEAM---------------------------------*/
/*----------------------------------------------------V1.1-------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------*/

uint8_t r_drv1 = 49, l_drv1 = 47, pwm_drv1 = 2; 
uint8_t r_drv2 = 45, l_drv2 = 43, pwm_drv2 = 3; 
uint8_t r_drv3 = 41, l_drv3 = 39, pwm_drv3 = 4; 
uint8_t r_drv4 = 37, l_drv4 = 35, pwm_drv4 = 5; 
uint8_t r_drv5 = 33, l_drv5 = 31, pwm_drv5 = 6; 
uint8_t r_drv6 = 29, l_drv6 = 27, pwm_drv6 = 7; 

void setup() {
  pinMode(r_drv1, OUTPUT); pinMode(l_drv1, OUTPUT); pinMode(pwm_drv1, OUTPUT);
  pinMode(r_drv2, OUTPUT); pinMode(l_drv2, OUTPUT); pinMode(pwm_drv2, OUTPUT);
  pinMode(r_drv3, OUTPUT); pinMode(l_drv3, OUTPUT); pinMode(pwm_drv3, OUTPUT);
  pinMode(r_drv4, OUTPUT); pinMode(l_drv4, OUTPUT); pinMode(pwm_drv4, OUTPUT);
  pinMode(r_drv5, OUTPUT); pinMode(l_drv5, OUTPUT); pinMode(pwm_drv5, OUTPUT);
  pinMode(r_drv6, OUTPUT); pinMode(l_drv6, OUTPUT); pinMode(pwm_drv6, OUTPUT);
  
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  for(int i = 0; i < 256; i++){
  digitalWrite(r_drv1, HIGH); digitalWrite(l_drv1, LOW); analogWrite(pwm_drv1, i);
  digitalWrite(r_drv2, HIGH); digitalWrite(l_drv2, LOW); analogWrite(pwm_drv2, i);
  digitalWrite(r_drv3, HIGH); digitalWrite(l_drv3, LOW); analogWrite(pwm_drv3, i);
  digitalWrite(r_drv4, HIGH); digitalWrite(l_drv4, LOW); analogWrite(pwm_drv4, i);
  digitalWrite(r_drv5, HIGH); digitalWrite(l_drv5, LOW); analogWrite(pwm_drv5, i);
  digitalWrite(r_drv6, HIGH); digitalWrite(l_drv6, LOW); analogWrite(pwm_drv6, i);
  analogWrite(LED_BUILTIN, i);   
  delay(20);
  }
  delay(2000);
  
  analogWrite(LED_BUILTIN, 0);   
  analogWrite(pwm_drv1, 0);
  analogWrite(pwm_drv2, 0);
  analogWrite(pwm_drv3, 0);
  analogWrite(pwm_drv4, 0);
  analogWrite(pwm_drv5, 0);
  analogWrite(pwm_drv6, 0);
  delay(2000);
  
  for(int i = 0; i < 256; i++){
  digitalWrite(r_drv1, LOW); digitalWrite(l_drv1, HIGH); analogWrite(pwm_drv1, i);
  digitalWrite(r_drv2, LOW); digitalWrite(l_drv2, HIGH); analogWrite(pwm_drv2, i);
  digitalWrite(r_drv3, LOW); digitalWrite(l_drv3, HIGH); analogWrite(pwm_drv3, i);
  digitalWrite(r_drv4, LOW); digitalWrite(l_drv4, HIGH); analogWrite(pwm_drv4, i);
  digitalWrite(r_drv5, LOW); digitalWrite(l_drv5, HIGH); analogWrite(pwm_drv5, i);
  digitalWrite(r_drv6, LOW); digitalWrite(l_drv6, HIGH); analogWrite(pwm_drv6, i);
  analogWrite(LED_BUILTIN, i);
  delay(20);
  }
  delay(2000);
  analogWrite(LED_BUILTIN, 0);  
  analogWrite(pwm_drv1, 0);
  analogWrite(pwm_drv2, 0);
  analogWrite(pwm_drv3, 0);
  analogWrite(pwm_drv4, 0);
  analogWrite(pwm_drv5, 0);
  analogWrite(pwm_drv6, 0);
  delay(2000);
}
