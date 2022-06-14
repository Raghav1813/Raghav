/*Arduino Self Balancing Robot*/

#include "I2Cdev.h"
#include <PID_v1.h> //From https://github.com/br3ttb/Arduino-PID-Library/blob/master/PID_v1.h
#include "MPU6050_6Axis_MotionApps20.h" //https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050

MPU6050 mpu;

// MPU control/status vars
bool dmpReady = false;  
uint8_t mpuIntStatus;   
uint8_t devStatus;     
uint16_t packetSize;   
uint16_t fifoCount;    
uint8_t fifoBuffer[64]; 

// orientation/motion vars
Quaternion q;          
VectorFloat gravity;   
float ypr[3];           

/*********Tune these 4 values for your BOT*********/
double setpoint= 176; 
double Kp = 21; //Set this first
double Kd = 0.8; //Set this secound
double Ki = 140; //Finally set this 
/******End of values setting*********/

double input, output;
PID pid(&input, &output, &setpoint, Kp, Ki, Kd, DIRECT);

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady()
{
    mpuInterrupt = true;
}

void setup() {
  Serial.begin(115200);

    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    devStatus = mpu.dmpInitialize();

    
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1688); 

    if (devStatus == 0)
    {
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        packetSize = mpu.dmpGetFIFOPacketSize();
        

        pid.SetMode(AUTOMATIC);
        pid.SetSampleTime(10);
        pid.SetOutputLimits(-255, 255);  
    }
    else
    {
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
    pinMode (6, OUTPUT);
    pinMode (9, OUTPUT);
    pinMode (10, OUTPUT);
    pinMode (11, OUTPUT);

    analogWrite(6,LOW);
    analogWrite(9,LOW);
    analogWrite(10,LOW);
    analogWrite(11,LOW);
}

void loop() {
 
    if (!dmpReady) return;

    while (!mpuInterrupt && fifoCount < packetSize)
    {
        pid.Compute();   
        
        Serial.print(input); Serial.print(" =>"); Serial.println(output);
               
        if (input>150 && input<200){//If the Bot is falling 
          
        if (output>0) 
        Forward(); 
        else if (output<0)
        Reverse();
        }
        else 
        Stop();
        
    }

    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    fifoCount = mpu.getFIFOCount();

    if ((mpuIntStatus & 0x10) || fifoCount == 1024)
    {
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    }
    else if (mpuIntStatus & 0x02)
    {
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        fifoCount -= packetSize;

        mpu.dmpGetQuaternion(&q, fifoBuffer); //get value for q
        mpu.dmpGetGravity(&gravity, &q); //get value for gravity
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity); //get value for ypr

        input = ypr[1] * 180/M_PI + 180;

   }
}

void Forward() //Code to rotate the wheel forward 
{
    analogWrite(6,output);
    analogWrite(9,0);
    analogWrite(10,output);
    analogWrite(11,0);
    Serial.print("F"); 
}

void Reverse() //Code to rotate the wheel Backward  
{
    analogWrite(6,0);
    analogWrite(9,output*-1);
    analogWrite(10,0);
    analogWrite(11,output*-1); 
    Serial.print("R");
}

void Stop() //Code to stop both the wheels
{
    analogWrite(6,0);
    analogWrite(9,0);
    analogWrite(10,0);
    analogWrite(11,0); 
    Serial.print("S");
}
