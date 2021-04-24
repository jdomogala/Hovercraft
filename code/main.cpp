#include "mbed.h"
#include "Servo.h"
#include "ultrasonic.h"
#include "XNucleo53L0A1.h"
#include "math.h"

//Variable Declaration Centre
Servo propeller(p21);
Servo inflator(p22);
Servo rudder1(p23); //Left Rudder
Servo rudder2(p24); //Right Rudder
void goAutonomous(); //Function Prototype
void command(); // Another Function Prototype
float maxPropellerVal = 0.2;

Serial blue(p13, p14);
DigitalOut shdn(p26);
//I2C sensor pins
#define VL53L0_I2C_SDA   p28
#define VL53L0_I2C_SCL   p27


DigitalOut led1(LED1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);
DigitalOut led4(LED4);

int deploy = 0; // Disable the System
int flag = 0;

//Low Level Actuation Centre
void speedUp()
{
    if (deploy) {
     if (propeller >= maxPropellerVal)
      { 
       propeller = maxPropellerVal; 
      }  else {
          propeller = propeller + 0.01;
      }
    } else {
        propeller = 0;
    }
}

void speedDown()
{
    if (deploy) {
      if (propeller == 0) 
      { 
       propeller = 0; 
      } else { 
       propeller = propeller - 0.01;
      }
    } else {
        propeller = 0;
     }
}

void ventFull()
{
    for (float i = 0; i < 0.17; i += 0.01)
    {
     inflator = i;
     deploy = 1;
     wait(0.5);
    }
}

void ventCutOff()
{
    inflator = 0;
    propeller = 0;
    rudder1 = 0.5;
    rudder2 = 0.5;
    deploy = 0;
    led1 = 0;
    led2 = 0;
    led3 = 0;
    led4 = 0;
}

void rightTurn()
{
    rudder1 = rudder2;
    if (rudder1 == 1)
     {
         rudder1 = 1;
         rudder2 = 1;
      } else {
       rudder1 = rudder1 + 0.05;
       rudder2 = rudder2 + 0.05;
    }
}

void leftTurn()
{
    rudder1 = rudder2;
    if (rudder1 == 0)
     {
         rudder1 = 0;
         rudder2 = 0;
      } else {
       rudder1 = rudder1 - 0.05;
       rudder2 = rudder2 - 0.05;
    }
}

void ledFlashers()
{
    led1 = !led1;
    wait(0.2);
    led2 = !led2;
    wait(0.1);
    led3 = !led3;
    wait(0.1);
    led4 = !led4;
    wait(0.2);
}

//Contingency Trigger...

static XNucleo53L0A1 *board=NULL;
DevI2C *device_i2c = new DevI2C(VL53L0_I2C_SDA, VL53L0_I2C_SCL);
int status;
uint32_t dist;
int old_dist = 0;
int warning = 0;

void distance()
{
    status = board->sensor_centre->get_distance(&dist);
     // If less than 10 cm cut off power sound the alarm flash the LEDS!
     if (dist < 100 && dist != 0 && old_dist < 100 && old_dist != 0) { //No eroneous Warnings
         warning = 1;
     } else {
         warning = 0;
         led1 = 0;
         led2 = 0;
         led3 = 0;
         led4 = 0;
     }
     if (warning == 1)
    {
        
        propeller = 1;
        inflator = 1;
        flag = 0; // Cut out the Power Plant...
        ventCutOff();
        ledFlashers();
    }
    old_dist = dist;
}


    


//Boss Loop
int main()
{
    propeller.calibrate();
    inflator.calibrate();
    rudder1.calibrate();
    rudder2.calibrate();
    
    rudder1 = 0.5;
    rudder2 = 0.5;
    
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
    
    //End Sensor Set up
    
    while(1)
    {
        distance(); //Check this as much as you want :-)
        if (!flag)
        {
            flag = 1;
            propeller = 1;
            inflator = 1;
            wait(0.5);
            propeller = 0;
            inflator = 0;
        }
        if (blue.readable() && blue.getc() == '!')
        {
            if (blue.getc() == 'B') {
                char bnum = blue.getc();
                if (blue.getc() == '1') 
                {
                 switch(bnum)
                 {
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




float x[4] = {0,0,0,0}; //State Variables.
float u[2][2] = {{0,0}, {0,0}}; //Inputs that translate to rudder and propeller speeds...
float y[4][2] = {{0,0},{0,0},{0,0},{0,0}};  // Log this
int i = 0;
int j = 0;
float I = 1; //You can change this 
//You can modify this as part of a Design Review
//Force parameters
float param1 = 7600; // 7600 RPM/Volt
float param2 = 8659.0135; // Aerodynamic Force
float param3 = 3.3; //Voltage

//Torque Parameters
float param4 = 0.0044;
float param5 = 90; //Range of the servo ... 

void command(float torque, float force)
{
    //The actual place where the force and torque and converted to forms the low level controllers can understand
    float prop_control = force / (param1 * param2 * param3);                   //(propeller * 3.3) * 7600 * 0.00086590135
    float rudder_control = torque / (param4 * param5);                                  //(rudder1.position() * 0.0644); //Torque Value 
    printf("%f \n", prop_control);
    
    //Do a saturation check...
    if (rudder_control > 1)
    {
        rudder1 = rudder2 = 1;
        wait(0.2);
    } else {
        rudder1 = rudder2 = fabs(rudder_control);
        wait(0.6);
    }
    
     wait(0.2);
    
    if (prop_control > maxPropellerVal)
    {
        propeller = 0;
        propeller = maxPropellerVal - 0.04;
    }
    else if (prop_control <= 0) {

        propeller = 0.14;
    } else {
        propeller = prop_control;
    }

} // Done! Enforce the Safety Envelope!
        

void goAutonomous() 
{
    //Sensor Set Up
    while(1)
    {
        distance();
        if (blue.readable() && blue.getc() == '!')
        {
            if (blue.getc() == 'B') {
                char bnum = blue.getc();
                if (blue.getc() == '1' && bnum == '4') 
                {
                    break; //Go back if 4 is pressed!
                }
            }
        }
        
        //Implement the controls aspect here...
        // Get me the output variables...
        //dist is y 
        float T = 0.1;
        float F = (propeller * param3) * param1 * param2; //This is the force
        float tau = (rudder1.read() * param4 * param5); //Torque Value
        float alpha = dist * T + 0.5 * (tau / I) * T * T; //Thats y and alpha ...
 
        int k = (i + 1) % 4;
        y[k][0] = y[i][0];
        y[k][1] = y[i][1];
        y[i][0] =  dist;
        y[i][1] = alpha; //Log in the data
        i = k;
        
        int w = (j + 1) % 2;
        u[w][0] = u[j][0];
        u[w][1] = u[j][1];
        u[j][0] =  dist;
        u[j][1] = alpha; //Log in the data
        j = w;

        
        //Hybrid Control Algorithms ...
        
        if(warning == 1)
        {
            ventCutOff();
        } //Dont move if there is a contingency
        
        if (warning == 0) {
            //There is no obstacle
            //Plow ahead at optimal speed
            //propeller = 0.15; //Fill up the cushions
            if (inflator == 0) {
              ventFull();
            }
            rudder1 = 0.5; //Keep Rudder Positions Straight
            deploy = 1;
            //Move now
            if (propeller != 0.15) {
               if (propeller == 0) {
               propeller = 1;
               propeller = 0;
               }
               wait(0.5);
               propeller = 0.15;
            } 
        } if (dist > 100 && dist < 300 && old_dist > 100 && old_dist < 300) {
            // Now you are in a situation where there is something ahead but not a danger to stop
            // Rover should make some predictive driving and try to turn
            float ref_y = dist + 50;
            float ref_alpha = alpha; //You can change these!
            
            float ref_F;
            float ref_tau; 
            //Need to compute these and set these as the required angles and rotation RPMs to the controllers...
            // You can change the parameters on top if needed! No problems with that...
            // Get the x from there
            
            x[0] = -8.5899 * ref_F + 8.5899 * y[2][0] - 8.5899 * y[3][0];
            x[1] = 0.0307 * ref_F - 0.0234 * y[1][0] - 0.0140 * y[2][0];
            x[2] = -8.5899 * ref_tau + 8.5899 * y[2][1] - 8.5899 * y[3][1];
            x[3] = 0.0307 * ref_tau - 0.0234 * y[1][1] - 0.0140 * y[2][1]; // Least Sq Approx Solution BEST LINEAR UNBIASED ESTIMATOR
   /*      
   -8.5899         0         0         0    8.5899         0   -8.5899         0
    0.0307         0   -0.0234         0   -0.0140         0         0         0   = Observability Matrix Least Sq Approx Matrix
         0   -8.5899         0         0         0    8.5899         0   -8.5899
         0    0.0307         0   -0.0234         0   -0.0140         0         0

    2.8091    0.0127         0         0
         0         0    2.8091    0.0127
   -9.1215    0.0602         0         0
         0         0   -9.1215    0.0602     = Controllability Matrix Least Norm Matrix
   -0.1843    0.0065         0         0
         0         0   -0.1843    0.0065
    6.4967   -0.0132         0         0
         0         0    6.4967   -0.0132
*/
             ref_tau = 0.4967 * x[2] - 0.0132 * x[3];
             ref_F = 6.4967 * x[0] - 0.0132 * x[1];
             command(ref_tau, ref_F); //Computes the values and compensates...
             
             ref_tau = 0 * x[0] + 0 * x[1] + (-0.01843) * x[2] + 0.0065 * x[3];
             ref_F =-0.1843 * x[0] + 0.0065 * x[1] + 0 * x[2] + 0 * x[3];
             command(ref_tau, ref_F);
             
             ref_F = -9.1215 * x[0] + 0.0602  * x[1] + 0 * x[2] + 0 * x[3];
             ref_tau = 0 * x[0] + 0 * x[1] + (-0.1215) * x[2] + 0.0602 * x[3];
             command(ref_tau, ref_F);
             
             ref_tau = 0 * x[0] + 0 * x[1] + 0.8091 * x[2] + 0.0127 * x[3];
             ref_F = 2.8091 * x[0] + 0.0127 * x[1] + 0 * x[2] + 0 * x[3];
             command(ref_tau, ref_F);
        }       
    }
}
