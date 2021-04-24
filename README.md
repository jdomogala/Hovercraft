# Mbed Bluetooth Hovercraft

## Overview
This project was made as the final design project for ECE 4180 - Embedded Systems Design in Spring 2021. Below is the presentation video.

[![Presentation Video](https://github.com/jdomogala/Hovercraft/blob/main/images/Presentation.JPG?raw=true)](https://www.youtube.com/watch?v=ahHCZ5dAKUU)

The project idea is a fully functional hovercraft. The hovercraft would use two brushless DC motors. One of the motors, the inflator, inflates the air cushion beneath the hovercraft and gives it enough lift so the other motor, the propeller, can propel the craft. A servo is used to rotate the propeller togive the craft steering capabilities. Commands are given to the craft via a bluetooth connection with a device running the BlueFruit App. A LIDAR sensor is also included to provide early warning and collision avoidance by blinking LEDS and emitting noises from the motors. In the event that the craft gets within 10 cm of an object, the inflator and propeller are disabled, which stops the craft, and the LEDS blink a pattern while the motors are used to make angry chirping noises.  

![Hovercraft](https://github.com/jdomogala/Hovercraft/blob/main/images/Hover.JPG?raw=true)  

## Team Members
* Jason Domogala
* Siddhanta Panda

## Materials 
### Electronic Parts
|  Part Name           | Part Quantity |  Product Name                                   |  Product Link                                                                        |
|----------------------|---------------|-------------------------------------------------|--------------------------------------------------------------------------------------|
| MBED Microcontroller |       1       | MBED LPC1768                                    | https://os.mbed.com/platforms/mbed-LPC1768/                                          | 
| Bluetooth UART       |       1       | Adafruit Bluetooth LE UART Friend               | https://www.adafruit.com/product/2479                                                | 
| Servo                |       1       | Hitec HS-422                                    | https://www.sparkfun.com/products/11884                                              |
| LIDAR                |       1       | Adafruit VL53LOX Time of Flight Distance Sensor | https://www.adafruit.com/product/3317                                                |
| Brushless DC Motor   |       2       | Thrust Series 1104-6700Kv                       | https://www.amazon.com/BLH-Q-1104-7600Kv-Racing-Motor/dp/B0772WVHGJ                  |
| ESC                  |       2       | Spedix GS 30A 2-4S/BLHeli_32/Dshot              | https://www.walmart.com/ip/Spedix-GS30-30A-2-4s-BLHeli-32-Dshot-ESC/430749917        |
| 9V 1A Power Supply   |       2       | 9V 1A Power Supply                              | https://www.amazon.com/Arduino-Power-Supply-Adapter-110V/dp/B018OLREG4               |
| 5V 2A Power Supply   |       1       | 5V 2A Power Supply                              | https://www.amazon.com/ALITOVE-100V-240V-Converter-5-5x2-1mm-Security/dp/B078RXZM4C/ |


### Other Materials
* Jumper Wires
* Breadboard
* Barrel Jack Connectors
* Aluminum Foil
* Cardboard
* Packing Tape
* Scissors
* Exacto
* Screw Driver


## Build Instructions
1. The first step of the build is to create a small box with an open bottom face that will serve as the base of the hovercraft. After the box has been made, mount the inflator motor in the center of the box so it pushes air out of the open side of the box. Cut vents in the top of the box for the motor's intake and restrict reverse airflow out of the vents by creating a duct with tape. Test that the motor is secure and working, then wrap the bottom of the box with tinfoil and tape it to all sides to create the cushion. Poke small pin holes in the tinfoil to allow for airflow out of the cushion to create an air layer when the motor is on. The hovercraft should now appear similar to the picture below.  

<img align="right" width="100" height="100" src="https://github.com/jdomogala/Hovercraft/blob/main/images/Inflator.JPG">  

![Base and Inflator Assembly](https://github.com/jdomogala/Hovercraft/blob/main/images/Inflator.JPG?raw=true)  

2. The next step is to make the steering and control mechanism. Fasten the servo to the back of the craft with the output facing upwards with tape. Set the servo to its neutral positon and mount the propeller motor onto the servo so it pushes air out the back of the craft. Make sure the propeller is locked in position using cardboard reinforcement and tape as needed. The hovercraft should now look like the picture below.  

![Hovercraft Assembly](https://github.com/jdomogala/Hovercraft/blob/main/images/Propeller.JPG?raw=true)  

3. The last step is to wire up the hovercraft. Mount the LIDAR facing out the front of the hovercraft using tape. Place the breadboard behind the LIDAR and wire the componenets according to the wiring diagram shown in the picture below.  

![Hovercraft Wiring Diagram](https://github.com/jdomogala/Hovercraft/blob/main/images/Wiring.JPG?raw=true)

## Operation
### Setup
Import, compile, and download the code for the [MBED](https://os.mbed.com/users/spanda38/code/TheHovercraft/)  
Download the Adafruit Bluetooth Connect App for [Android](https://play.google.com/store/apps/details?id=com.adafruit.bluefruit.le.connect&hl=en_US) or [iOS](https://itunes.apple.com/us/app/adafruit-bluefruit-le-connect/id830125974?mt=8)  
Connect the App to the Bluetooth UART 

### Instructions
Below is a picture of the Adafruit App GUI and below that is a table to show the hovercraft response to pressing buttons on the app  
![Adafruit App GUI](https://github.com/jdomogala/Hovercraft/blob/main/images/Adafruit.png?raw=true)

|  Button        |  Action         |
|----------------|-----------------|
| '1 Button'     | Inflate the air cushion by turning on the inflator motor  |
| '2 Button'     | Turn off the craft and reset it                           |
| 'Up Button'    | Increase the speed of the propeller motor                 |
| 'Down Button'  | Decrease the speed of the propeller motor                 |
| 'Left Button'  | Turn the propeller motor to the left using the servo      |
| 'Right Button' | Turn the propeller motor to the right using the servo     |  

## Video
Below is a video demonstration of the hovercraft in operation  
[![Demo Video](http://i3.ytimg.com/vi/8YloxDpKNwA/maxresdefault.jpg)](https://www.youtube.com/watch?v=8YloxDpKNwA)


## Improvements
The hovercraft can be improved in the following ways:
* More powerful motors for more thrust (This will make it faster and more agile)
* Rudder system instead of turning the propeller (This will allow for better thrust vectoring and control)
* Adding another inflator fan (This will allow for more lift and increase the air layer significantly)
* Using battery packs for power (This will increase weight and require a lot more thrust from the motors but allow for remote operation)


## Automation Control Expansion
A hybrid state space integral control system that has active collsion avoidance. The hovercraft is set to move forward until it sees an object, which it manages speed and direction of the propeller to avoid. The inflator motor is only used in case of shutoff when an object appears too close to avoid.  
The automated control is activated by pressing the '3 Button' on the Adafruit App GUI. Below is a video of the automated control.  

[![Automation Video](http://i3.ytimg.com/vi/rwRwWCEp8zU/maxresdefault.jpg)](https://www.youtube.com/watch?v=rwRwWCEp8zU)

