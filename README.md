---
author: Jason Domogala, Siddhanta Panda
date: April 19, 2021
geometry: margin=1in
title: Hovercraft
header-includes:
  - \hypersetup{colorlinks=true}
---

# Mbed Bluetooth Hovercraft

## Overview
This project was made as the final design project for ECE 4180 - Embedded Systems Design in Spring 2021.

The project idea is a fully functional hovercraft. The hovercraft would use two brushless DC motors. One of the motors, the inflator, inflates the air cushion beneath the hovercraft and gives it enough lift so the other motor, the propeller, can propel the craft. A servo is used to rotate the propeller togive the craft steering capabilities. Commands are given to the craft via a bluetooth connection with a device running the BlueFruit App. A LIDAR sensor is also included to provide early warning and collision avoidance by blinking LEDS and emitting noises from the motors. In the event that the craft gets within 10 cm of an object, the inflator and propeller are disabled, which stops the craft, and the LEDS blink a pattern while the motors are used to make angry chirping noises.


## Team Members
* Jason Domogala
* Siddhanta Panda

## Materials 
### Electronic Parts
|  Part Name           |  Part Quantity  |  Product Name                                   |  Product Link                               |
|----------------------|-----------------|-------------------------------------------------|---------------------------------------------|
| MBED Microcontroller |        1        | MBED LPC1768                                    | https://os.mbed.com/platforms/mbed-LPC1768/ | 
| Bluetooth UART       |        1        | Adafruit Bluetooth LE UART Friend               | https://www.adafruit.com/product/2479       | 
| Servo                |        1        | Hitec HS-422                                    | https://www.sparkfun.com/products/11884     |
| LIDAR                |        1        | Adafruit VL53LOX Time of Flight Distance Sensor | https://www.adafruit.com/product/3317       |
| Brushless DC Motor   |        2        | ???????                                         |                                             |
| 9V 1A Power Supply   |        2        |                                                 |                                             |
| 5V 2A Power Supply   |        1        |                                                 |                                             |


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

![Base and Inflator Assembly](https://github.com/jdomogala/Hovercraft/blob/main/Inflator.JPG?raw=true)  

2. The next step is to make the steering and control mechanism. Fasten the servo to the back of the craft with the output facing upwards with tape. Set the servo to its neutral positon and mount the propeller motor onto the servo so it pushes air out the back of the craft. Make sure the propeller is locked in position using cardboard reinforcement and tape as needed. The hovercraft should now look like the picture below.  

![Hovercraft Assembly](https://github.com/jdomogala/Hovercraft/blob/main/Propeller.JPG?raw=true)  

3. The last step is to wire up the hovercraft. Mount the LIDAR facing out the front of the hovercraft using tape. Place the breadboard behind the LIDAR and wire the componenets according to the wiring diagram shown in the picture below.  

![Hovercraft Wiring Diagram](https://github.com/jdomogala/Hovercraft/blob/main/Wiring.JPG?raw=true)

## Operation
### Setup
Download the bin file to the MBED  
Download the Adafruit Bluetooth Connect App for [Android](https://play.google.com/store/apps/details?id=com.adafruit.bluefruit.le.connect&hl=en_US) or [iOS](https://itunes.apple.com/us/app/adafruit-bluefruit-le-connect/id830125974?mt=8)  
Connect the App to the Bluetooth UART 

### Instructions
Below is a picture of the Adafruit App GUI and below that is a table to show the hovercraft response to pressing buttons on the app  
![Adafruit App GUI](https://github.com/jdomogala/Hovercraft/blob/main/Adafruit.png?raw=true)

|  Button        |  Action         |
|----------------|-----------------|
| '1 Button'     | Inflate the air cushion by turning on the inflator motor  |
| '2 Button'     | Turn off the craft and reset it                           |
| 'Up Button'    | Increase the speed of the propeller motor                 |
| 'Down Button'  | Decrease the speed of the propellor motor                 |
| 'Left Button'  | Turn the propeller motor to the left using the servo      |
| 'Right Button' | Turn the propeller motor to the right using the servo     |  

## Video
Below is a video demonstration of the hovercraft in operation  
[![Watch the video](https://img.youtube.com/vi/8YloxDpKNwA/hqdefault.jpg)](https://www.youtube.com/watch?v=8YloxDpKNwA)


## Improvements
The hovercraft can be improved in the following ways:
* More powerful motors for more thrust (This will make it faster and more agile)
* Rudder system instead of turning the propeller (This will allow for better thrust vectoring and control)
* Adding another inflator fan (This will allow for more lift and increase the air layer significantly)
* Using battery packs for power (This will increase weight and require a lot more thrust from the motors but allow for remote operation)
