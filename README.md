This project is based on code written by Alberto Marziali (https://github.com/AlbertoMarziali)
The original repositry resides at https://github.com/AlbertoMarziali/bmw_remote_start

There is a major change from how Alberto managed the pre-start system.
- We no longer need to solder to the key fob PCB to lock/unlock the car.
- Instead we drive the arm/disarm and lock/unlock signals directly

# BMW F3x REMOTE START
BMW F3x Remote Start Project
- Currently testing using an Arduino UNO board, SeeedStudio CANBUS Shield, and mechanical relay board
- Future plans to utilize a custom PCB with a Cortex-M0, MCP2515, and solid state relays

# Features
This project allows us to do the following things:
- If the engine is off, remote start the car with a triple click on the lock button of the OEM keyfob.
- After 15 minutes from remote start, if no one started driving, automatically shut off the car
- If the engine is on, remote stop the car with a triple click on the lock button of the OEM keyfob.
- You have to unlock the car before driving it (why wouldn't you) or it will perform an emergency anti-thief stop

# How it works
The Arduino+CANBUS Shield listen to the BMW K-CAN2 bus looking for a triple lock button click. If this even is seen it starts a 18 seconds long operation, during which it does those steps:
1. Turns on the in-car keyfob
2. Unlocks the car
3. Locks the car 
4. Virtually presses the start button, turning on the ignition
5. Waits some seconds, to let the engine be ready to start.
6. Virtually presses the brake and the start button, turning on the engine
7. Turns off the in-car key.

# Security issues
**What happens if a thief enter the car (by smashing the window) and tries to steal it?**

If someone tries to engage a gear with the car remote started, without unlocking it with a keyfob first, the car automatically shuts off.



**The spare key is always in the car. Wouldn't this be enough to allow a thief to turn on the engine?**

Yes, but actualy no. The Arduino turns on the keyfob only when it needs it to remote start. Normally the key isn't powered so the car doesn't see it and can't be turned on!



**Why do the Arduino open and close the car before starting the engine?**

The lock and unlock thing is needed to be able to turn on the ignition. This is because of the BMW security protocol, which disables the possibility of turning on the car if the car wasn't opened before. This is not a security issues though, because this process happens in 2 seconds and the car remains open for just 1 second.

# Currently used hardware
Here's a list of things you need for this project:
- Arduino UNO or MEGA
- LM2596S voltage regulator board, with the output regulated to 5V
- SeeedStudio CANBUS Shield
- 6x Relay Board with 5V relays
- 2x 560Ω (to replicate the start button internal 560Ω resistor) 
- 1x 1100Ω (to replicate the 12mA current outputted by the hall sensor inside the brake light switch)
- The spare keyfob

You can get those resistance values by putting more resistors in series and sum the resistance value.
For example:
- to get a 550Ω resistance, you can put 220Ω and 330Ω resistors in series! 
- to get a 1100Ω resistance, you can put 1kΩ and 100Ω resistors in series!

# Wiring
You have to do the wiring for:
- K-CAN2 bus
- 12V and GND
- Brake light
- Start button
- Central lock/unlock actuation
- Door locks/anti-theft actuation
- Keyfob battery


## Car Wiring

**KCAN2**

Attention: the OBD2 port doesn't have KCAN2. You can't use it. You have to take KCAN2 from the FEM
- KCAN2_H: FEM A173\*8B - pin 50
- KCAN2_L: FEM A173\*8B - pin 49


**12V and GND**

Attention: you can't use the 12V plug as power source, you need an always-powered 12V source. You can find it at the FEM.
- 12V: ATM Fuse Tap, Main 12V Supply (FEM A173\*3B, pin 33)
- GND: Body Screw near FEM (FEM A173\*3B, pin 6)


**Brake Light Switch and Start-Stop Button**

- BRAKE: FEM A173\*3B - Pin 22
- START/STOP: FEM A173\*7B - Pins 45, 49


**KeyFob Lock and Unlock buttons**

You have to simulate the press of the switch using the relay. To do that, just hook to the key switches like that:
- DOOR LOCKS: FEM A173\*4B - Pin 16
- CENTRAL LOCKS: FEM A173\*2B - Pin 30


## KeyFob Wiring

**KeyFob Power**

You have to interrupt the battery positive connection, split it in 2 wires and feed them inside the relay:



# Plans
I plan to have custom PCBs with enclosures and wiring harnesses available for sale.

# Future
Additional ideas are to add WiFi/4G connectivity to allow for remote starting/setting HVAC from a phone.
