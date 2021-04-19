#include "mbed.h"
#include "Servo.h"
#include "ultrasonic.h"
#include "XNucleo53L0A1.h"
#include "math.h"

//Pin Assignments
Servo propeller(p21);
Servo inflator(p22);
Servo rudder(p23);
Serial blue(p13, p14);
DigitalOut shdn(p26);
#define VL53L0_I2C_SDA   p28
#define VL53L0_I2C_SCL   p27
DevI2C *device_i2c = new DevI2C(VL53L0_I2C_SDA, VL53L0_I2C_SCL);
DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

//Variables
float maxPropellerVal = 0.2;
int deploy = 0; // Disable the System
int flag = 0;
int status;
uint32_t dist;
int old_dist = 0;
int warning = 0;
static XNucleo53L0A1 *board = NULL;

/* Speed Up Command
 * Increases speed of rear motor
 */
void speedUp() {
    if (deploy) {
        if (propeller >= maxPropellerVal) { 
            propeller = maxPropellerVal; 
        }  else {
            propeller = propeller + 0.01;
        }
    } else {
        propeller = 0;
    }
}

/* Speed Down Command
 * Decreases speed of rear motor
 */
void speedDown() {
    if (deploy) {
        if (propeller == 0) { 
            propeller = 0; 
        } else { 
            propeller = propeller - 0.01;
        }
    } else {
        propeller = 0;
    }
}

/* Inflate Cushion Command
 * Incremently inflates air cushion
 */
void ventFull() {
    for (float i = 0; i < 0.17; i += 0.01) {
        inflator = i;
        deploy = 1;
        wait(0.5);
    }
}

/* Deflate Cushion Command
 * Sets everything to off values
 */
void ventCutOff() {
    inflator = 0;
    propeller = 0;
    rudder = 0.5;
    deploy = 0;
    led1 = 0;
    led2 = 0;
    led3 = 0;
    led4 = 0;
}

/* Rudder Right Command
 * Turn rudder right
 */
void rightTurn() {
    if (rudder == 1) {
       rudder = 1;
    } else {
       rudder = rudder + 0.05;
    }
}

/* Rudder Left Command
 * Turn rudder left
 */
void leftTurn() {
    if (rudder == 0) {
       rudder = 0;
    } else {
       rudder = rudder - 0.05;
    }
}

/* Flash LEDS
 * Flash all 4 LEDS
 */
void ledFlashers() {
    led1 = !led1;
    wait(0.2);
    led2 = !led2;
    wait(0.1);
    led3 = !led3;
    wait(0.1);
    led4 = !led4;
    wait(0.2);
}

/* LIDAR Check Command
 * Stop if something within 10 cm
 */
void distance() {
    status = board->sensor_centre->get_distance(&dist);
    // If less than 10 cm cut off power sound the alarm flash the LEDS
    if (dist < 100 && dist != 0 && old_dist < 100 && old_dist != 0) {
        warning = 1;
    } else {
        warning = 0;
        led1 = 0;
        led2 = 0;
        led3 = 0;
        led4 = 0;
    }
    if (warning == 1) {
        propeller = 1;
        inflator = 1;
        flag = 0;
        ventCutOff();
        ledFlashers();
    }
    old_dist = dist;
}

/* Main Command
 * Initialize values and continuously execute bluetooth commands
 */
int main() {
    propeller.calibrate();
    inflator.calibrate();
    rudder.calibrate();
    rudder = 0.5;
    
    //Sensor Set Up
    board = XNucleo53L0A1::instance(device_i2c, A2, D8, D2);
    shdn = 0; 
    wait(0.1);
    shdn = 1;
    wait(0.1);
    status = board->init_board();
    while (status) {
        status = board->init_board();
    }
    
    while (1) {
        distance(); //Check this as much as you want :-)
        if (!flag) {
            flag = 1;
            propeller = 1;
            inflator = 1;
            wait(0.5);
            propeller = 0;
            inflator = 0;
        }
        if (blue.readable() && blue.getc() == '!') {
            if (blue.getc() == 'B') {
                char bnum = blue.getc();
                if (blue.getc() == '1') {
                    switch(bnum) {
                        case '5' :  speedUp(); break;
                        case '6' :  speedDown(); break;
                        case '1' :  ventFull(); break; //Cushion Deployed
                        case '2' :  ventCutOff();  break; //Cushion Cutoff
                        case '3' :  ventCutOff(); goAutonomous(); break;
                        case '7' :  rightTurn(); break;
                        case '8' :  leftTurn(); break;
                    }
                    blue.getc();
                }
            }          
        }
    }
    return 0;
}
