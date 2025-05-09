# KRAI-ESPNOW-DS2-CONTROLLER-PROJECT
DS2 controller that's been hijacked with ESP32 and ESP-NOW communication Protocol

# documentation:
Google Drive Link: https://drive.google.com/drive/u/0/folders/1bF-S8_3wirRWgxVvsVIZeP3gbsoaI8Ss

# DS2 controller just used for the button contacts and the housing.
so essentially its a DS2 controller but with 'some' mod
we've soldered the switch contact and connecting those to esp32's pin.
we got total of 19 channel that consist of:
- 15 Channel of switch. (Momentary) (SW15 is a toggle switch) 
- "L3", "R3", "L2", "L1", "UP", "LEFT", "DOWN", "RIGHT", "SQUARE", "CROSS", "ROUND", "TRIANGLE", "R2", "R1", SW
- 4 channel of potentiometer on a joystick.
- left Joystick = Lx, Ly  -  Right Joystick = Rx, Ry

# Featuring a 1,3" SH1106 OLED Display with features:
- Battery Voltage Indicator.
- All Button State Monitoring.
- Joystick Position Monitoring. (With Visual Position)
- Connection Status to the Receiver.
- (Error =Rx not active)
- (Standby = Rx Active but no data Transferred)
- (CONN OK = RX is Active and data is Sent)

# More Feature on this remote:
- Periodic sending system, No data is trasferred when no button is pressed or joystick is not moved (Improve Bandwidth and reduce task Overload for the ESP32, Reduce Heat)
- timeout of 1 Second after no button is pressed or no joystick movement. reduce data loss when transmitting.
- All data is sent at the same time, Avoid Data Loss, Multitasking Compatible.
- 3 Dbi external Antenna, increase Transmit Range. (Tested +- 115 meters of range, Not Line Of Sight, Building and Tree Obstacle)
- Integrated 2s LiPo battery.
- NEAT EXECUTION, NO 'paksa N paksa'. All conponents is placed inside. like nothing happened :)

=============================================================================================================================================================================

# Pergerakan base Kontroler DS2 - ESP32 RX - Arduino Mega - Roda Mekanum.
- Hidupkan remote saklar dibelakang sebelah kanan (push button toggle)
- auto connect ke receiver, status remote : Standby = RX hidup, namun belum ada data yang dikirimkan.
											                      conn ok = RX hidup, sedang ada data yang dikirimkan. otomatis stop kirim data setelah 1 detik tombol tidak ditekan.
											                      error   = RX mati / tidak bisa connect.
- Status koneksi remote ada di layar sisi atas kiri.
- Tegangan baterai ada di layar sisi atas kanan.
- Baterai LiPo 2S, tegangan 8.4 - 7.2  USAHAKAN JIKA SUDAH DIBAWAH 7V LANGSUNG CHARGE. -IMAX MODE CHARGE (Arus 0.5A, 2S).
- Monitoring status semu tombol

# github program pergerakan base:
https://github.com/HabibMuhammad05/KRAI-ESPNOW-DS2-CONTROLLER-PROJECT

# last fix update: 
- **Remote** : KRAI_ESPNOW_Remote_V3.4_refreshRate_TX.ino
   > https://github.com/HabibMuhammad05/KRAI-ESPNOW-DS2-CONTROLLER-PROJECT/tree/master/KRAI_ESPNOW_Remote_V3.4_refreshRate_TX
   
- **ESPNOW RX** : KRAI_ESPNOW_Remote_V3.3_SendToMega_RX.ino
   > https://github.com/HabibMuhammad05/KRAI-ESPNOW-DS2-CONTROLLER-PROJECT/tree/master/KRAI_ESPNOW_Remote_V3.3_SendToMega_RX
   
- **Arduino Mega Mecanum Joystick** : KRAI_ArduinoMega_mecanumJoystick_PID_Controller.ino
	 > https://github.com/HabibMuhammad05/KRAI-ESPNOW-DS2-CONTROLLER-PROJECT/tree/master/KRAI_ArduinoMega_mecanumJoystick_PID_Controller
  
- **Arduino Mega Mecanum button Control** : KRAI_ArduinoMega_mecanumButton_PID_Controller.ino
	 > https://github.com/HabibMuhammad05/KRAI-ESPNOW-DS2-CONTROLLER-PROJECT/tree/master/KRAI_ArduinoMega_mecanumButton_PID_Controller
  	
		
# Repository Percobaan motor PG45 dengan internal encoder + PID controller
> https://github.com/HabibMuhammad05/PID-PG45-Motor-Encoder-Feedback

Deskripsi tiap Sketch program:
	- **Motor_With_Encoder.ino**  : Pembacaan pin A & B Internal Encoder motor, Output pulse untuk kontrol Motor.
	   > https://github.com/HabibMuhammad05/PID-PG45-Motor-Encoder-Feedback/tree/master/Motor_With_Encoder.
	- **MotorControl_RPM_Tuning.ino**  : PID 1 motor searah, Hanya CW.
	   > https://github.com/HabibMuhammad05/PID-PG45-Motor-Encoder-Feedback/tree/master/MotorControl_RPM_Tuning
	- **MotorControl_PID_DualDirection.ino**  : Perhitungan & tuning PID 1 motor support pergerakan 2 arah (CW & CCW).
	   > https://github.com/HabibMuhammad05/PID-PG45-Motor-Encoder-Feedback/tree/master/MotorControl_PID_DualDirection
	- **MotorControl_PID_MultiMotor.ino**  : Kontrol 4 motor support pergerakan 2 arah (CW&CCW), Perhitungan sendiri-sendiri per motor.
	   > https://github.com/HabibMuhammad05/PID-PG45-Motor-Encoder-Feedback/tree/master/MotorControl_PID_MultiMotor
