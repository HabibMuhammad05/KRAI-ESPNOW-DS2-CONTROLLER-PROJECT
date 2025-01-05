# KRAI-ESPNOW-DS2-CONTROLLER-PROJECT
DS2 controller that's been hijacked with ESP32 and ESP-NOW communication Protocol

# documentation:
Google Drive https://drive.google.com/drive/u/0/folders/1bF-S8_3wirRWgxVvsVIZeP3gbsoaI8Ss

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
