/*Arduino Self Balancing Robot
 */
#include "I2Cdev.h"
#include "MPU6050.h"
#include "math.h"

#define leftMotorPin1   6
#define leftMotorPin2   9
#define rightMotorPin1  10
#define rightMotorPin2  11

#define TRIGGER_PIN 9
#define ECHO_PIN 8
#define MAX_DISTANCE 75

#define Kp  17
#define Kd  0.5
#define Ki  150
#define sampleTime  0.005
#define targetAngle -2.5

MPU6050 mpu;

int16_t accX, accZ, gyroY;
volatile int motorPower, gyroRate;
volatile float accAngle, gyroAngle, prevAngle=0, currentAngle, currentGyroAngle, error, prevError=0, errorSum=0;
volatile byte count=0;
unsigned long currTime, prevTime=0, loopTime;

void setup() {
  //setup motors
  pinMode (leftMotorPin1, OUTPUT);
  pinMode (leftMotorPin2, OUTPUT);
  pinMode (rightMotorPin1, OUTPUT);
  pinMode (rightMotorPin2, OUTPUT);
  //By default turn off both the motors
  analogWrite(leftMotorPin1,LOW);
  analogWrite(leftMotorPin2,LOW);
  analogWrite(rightMotorPin1,LOW);
  analogWrite(rightMotorPin2,LOW);
  
  //setup MPU sensor
  mpu.initialize();
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1688); 
  Serial.begin(9600);

  // initialize PID sampling loop
  init_PID();
}

void loop() {
  //readings from MPU
  accZ = mpu.getAccelerationZ();
  accX = mpu.getAccelerationX();
  gyroY = mpu.getRotationY();

  //set motor power after constraining it
  if (motorPower>0) //Falling towards front 
    Forward(motorPower); //Rotate the wheels forward 
  else if (motorPower<0) //Falling towards back
    Reverse(motorPower); //Rotate the wheels backward   
}

// The ISR will be called every 5 milliseconds
ISR(TIMER1_COMPA_vect){
  //measure angle using acceleration along X & Z axises
  accAngle = atan2(accX, accZ)*RAD_TO_DEG;
  //Serial.print("accAngle: ");
  //Serial.println(accAngle);

  //measure angle using rotation velocity along Y axis
  gyroRate = map(gyroY, -32768, 32767, -250, 250);
  gyroAngle = (float)gyroRate * sampleTime;
  //currentGyroAngle = prevAngle - gyroAngle;
  //prevAngle = currentGyroAngle;
  //Serial.print("currentGyroAngle: ");
  //Serial.println(currentGyroAngle);

  //calculate angle using accelerometer & gyro results above
  currentAngle = 0.9934*(prevAngle - gyroAngle) + 0.0066*(accAngle); 
  //Serial.print("currentAngle: ");
  //Serial.println(currentAngle);

  //calculate error
  error = currentAngle - targetAngle;
  errorSum = errorSum + error;
  errorSum = constrain(errorSum, -300, 300);

  //calculate output from P, I and D values
  motorPower = Kp*(error) + Ki*(errorSum)*sampleTime - Kd*(currentAngle-prevAngle)/sampleTime;
  prevAngle = currentAngle;
}

void Forward(volatile int motorSpeed) //Code to rotate the wheel forward 
{
    analogWrite(leftMotorPin1, motorSpeed);
    analogWrite(leftMotorPin2, 0);
    analogWrite(rightMotorPin1, motorSpeed);
    analogWrite(rightMotorPin2, 0);
    Serial.print("F"); //Debugging information 
}
void Reverse(volatile int motorSpeed) //Code to rotate the wheel Backward  
{
    analogWrite(leftMotorPin1, 0);
    analogWrite(leftMotorPin2, motorSpeed*-1);
    analogWrite(rightMotorPin1 ,0);
    analogWrite(rightMotorPin2 ,motorSpeed*-1); 
    Serial.print("R");
}

void init_PID() {  
  // initialize Timer1
  cli();          // disable global interrupts
  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B    
  // set compare match register to set sample time 5ms
  OCR1A = 9999;    
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS11 bit for prescaling by 8
  TCCR1B |= (1 << CS11);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();          // enable global interrupts
}
