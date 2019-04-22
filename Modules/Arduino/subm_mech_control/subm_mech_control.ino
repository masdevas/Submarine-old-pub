#include <Wire.h>     //I2C
#include <TroykaIMU.h>      //IMU
#include <Servo.h>    //Motors

/* Serial variables (dependence with .h in computers) */
#define SERIAL_SPEED 9600
#define SERIAL_DELIMITER ";"
#define SC_MIN_CODE 1
#define SC_INIT 2
#define SC_READY_RECEIVE 3
#define SC_READY_WORK 4
#define SC_READY_SEND_ORDERS 5
#define SC_OKEY 6
#define SC_MAX_CODE_ERR 7

typedef union
{
 float number;
 byte bytes[4];
} floatbyte_un, bytefloat_un;
typedef union
{
 int number;
 byte bytes[2];
} intbyte_un, byteint_un;
intbyte_un curState;
intbyte_un flagRequest;


/* IMU variables */
#define BETA 0.22f    //Coef of Madgwick's filter
#define STAT_FPS 100    //~100
Madgwick filter;
Accelerometer accel;
Gyroscope gyro;
floatbyte_un yaw, pitch, roll;
float rx, ry, rz, ax, ay, az;
float fps;
unsigned long deltaMillis, startMillis;

/* Motors variables */  //TODO set pins(all pwm)
#define MAX_POW 1960
#define MIN_POW 1040
#define START_POW 1472    //real - 1488
#define UNLOCK_POW 1472
#define UNLOCK_TIME 100
#define MOT_TIME_CALIBRATE_MAX 2000
#define MOT_TIME_CALIBRATE_MIN 6000
#define MOT_VFL 2
#define MOT_VFR 3
#define MOT_VBR 4
#define MOT_VBL 5
#define MOT_HL 6
#define MOT_HR 7
Servo motorVFL, motorVFR, motorVBL, motorVBR, motorHL, motorHR;
int powerMot, powerMot2;
intbyte_un speedHoriz, speedVert;

/* Presssure sensor variables */  //TODO set pin
#define PRESSURE_SENSOR 54      //A0
#define ATM_PRESSURE 0.2
floatbyte_un pressure;
floatbyte_un standPressure;

/* Init sonar */    //TODO set pins
#define SONAR_TRIG 22       //Not a pwm
#define SONAR_ECHO 23       //Not a pwm
#define SONAR_LOW_TIME 2
#define SONAR_HIGH_TIME 10
#define SONAR_TIME_WAIT 100
unsigned impulseTime, distance;
unsigned long lastTimeCount;
floatbyte_un percentFillingBallastTank;
floatbyte_un standPercentFillingBallastTank;

/* Parameters stable */
#define EPSILON_ROLL 10
#define EPSILON_PITCH 10
#define EPSILON_YAW 5
#define COEF_POW_STABLE 6
#define MOTOR_TIME_WORK_STABLE 50
floatbyte_un standYaw, standRoll, standPitch;

/* Functions of direct */
void stableSubmarine();

/* Functions of sensors */
void imuInitAngles();
void countFillingBallastTank();
void countDepth();
void countDistance();

/* Functions of data and flags exchange */ 
void receiveFloat(floatbyte_un* un);
void sendFloat(floatbyte_un* un);
void receiveInt(intbyte_un* un);
void sendInt(intbyte_un* un);
void receiveOrders();
void sendData();
void sendFlag(int flag);
void receiveFlag();
int waitingForServersRequest();

void setup() 
{
  /* Init serial port */
  Serial.begin(SERIAL_SPEED);
  //Try connect (MUST_HAVE)
  while(1)
  {
    sendFlag(SC_READY_WORK);
    receiveFlag();
    if(flagRequest.number == SC_READY_WORK)
      break;
  }
  //sendFlag(SC_READY_WORK);
  //curState.number = SC_OK;

  /* Init IMU */
  accel.begin();
  gyro.begin();
  
  /* Init motors */
  pinMode(MOT_VFL, OUTPUT);
  pinMode(MOT_VFR, OUTPUT);
  pinMode(MOT_VBL, OUTPUT);
  pinMode(MOT_VBR, OUTPUT);
  pinMode(MOT_HL, OUTPUT);
  pinMode(MOT_HR, OUTPUT);
  motorVFL.attach(MOT_VFL);
  motorVFR.attach(MOT_VFR);
  motorVBL.attach(MOT_VBL);
  motorVBR.attach(MOT_VBR);
  motorHL.attach(MOT_HL);
  motorHR.attach(MOT_HR);
  motorVFL.writeMicroseconds(MAX_POW);
  motorVFR.writeMicroseconds(MAX_POW);
  motorVBL.writeMicroseconds(MAX_POW);
  motorVBR.writeMicroseconds(MAX_POW);
  motorHL.writeMicroseconds(MAX_POW);
  motorHR.writeMicroseconds(MAX_POW);
  delay(MOT_TIME_CALIBRATE_MAX);
  motorVFL.writeMicroseconds(MIN_POW);
  motorVFR.writeMicroseconds(MIN_POW);
  motorVBL.writeMicroseconds(MIN_POW);
  motorVBR.writeMicroseconds(MIN_POW);
  motorHL.writeMicroseconds(MIN_POW);
  motorHR.writeMicroseconds(MIN_POW);
  delay(MOT_TIME_CALIBRATE_MIN);

  //Unlock controllers of motors
  motorVFL.writeMicroseconds(UNLOCK_POW);
  motorVFR.writeMicroseconds(UNLOCK_POW);
  motorVBL.writeMicroseconds(UNLOCK_POW);
  motorVBR.writeMicroseconds(UNLOCK_POW);
  motorHL.writeMicroseconds(UNLOCK_POW);
  motorHR.writeMicroseconds(UNLOCK_POW);
  
  /* Init pressure sensor */
  pinMode(PRESSURE_SENSOR, INPUT);

  /* Init sonar */
  pinMode(SONAR_TRIG, OUTPUT);
  pinMode(SONAR_ECHO, INPUT);
  lastTimeCount = 0;

  /* Init standarts */
  receiveOrders();
  sendStands();
}

floatbyte_un unf;
intbyte_un uni;

void loop() 
{
  if(!waitingForServersRequest())
  {
    sendFlag(flagRequest.number);
  }else
  {
    sendFlag(SC_OKEY);
  }
  
  switch(flagRequest.number)
  {
    case SC_READY_RECEIVE:
      imuInitAngles();
      countDepth();
      countFillingBallastTank();
      sendData();
      break;
    case SC_READY_SEND_ORDERS:
      receiveOrders();
      break;
    default:
      break;
  }
}

/* Functions of direct */
void stableSubmarine()
{
  /* Process front-back incline (Roll) */
  imuInitAngles();
  if((powerMot = abs(roll.number - standRoll.number)) > EPSILON_ROLL)
  {
    /* Count powers of front and back motors */
    powerMot *= COEF_POW_STABLE;
    powerMot2 = -powerMot;

    /* Clamp powers */
    powerMot = ((powerMot < (MAX_POW - START_POW)) ? powerMot : MAX_POW - START_POW) + START_POW;
    powerMot2 = ((powerMot2 > (MIN_POW - START_POW)) ? powerMot2 : MIN_POW - START_POW) + START_POW;
    if((roll.number - standRoll.number) > 0)  // Left incline
    {
      /* On powers */
      motorVFL.writeMicroseconds(powerMot);
      motorVBL.writeMicroseconds(powerMot);
      motorVFR.writeMicroseconds(powerMot2);
      motorVBR.writeMicroseconds(powerMot2);
    }else   // Right incline
    {
      /* On powers */
      motorVFL.writeMicroseconds(powerMot2);
      motorVBL.writeMicroseconds(powerMot2);
      motorVFR.writeMicroseconds(powerMot);
      motorVBR.writeMicroseconds(powerMot);
      
    }
    delay(MOTOR_TIME_WORK_STABLE);
    motorVFL.writeMicroseconds(START_POW);
    motorVBL.writeMicroseconds(START_POW);
    motorVFR.writeMicroseconds(START_POW);
    motorVBR.writeMicroseconds(START_POW);
  }

  /* Process right-left incline (Pitch) */
  imuInitAngles();
  if((powerMot = abs(pitch.number - standPitch.number)) > EPSILON_PITCH)
  {
    /* Count powers of front and back motors */
    powerMot *= COEF_POW_STABLE;
    powerMot2 = -powerMot;

    /* Clamp powers */
    powerMot = ((powerMot < (MAX_POW - START_POW)) ? powerMot : MAX_POW - START_POW) + START_POW;
    powerMot2 = ((powerMot2 > (MIN_POW - START_POW)) ? powerMot2 : MIN_POW - START_POW) + START_POW;
    
    if((pitch.number - standPitch.number) > 0)  // Back incline
    {
      /* On powers */
      motorVBR.writeMicroseconds(powerMot);
      motorVBL.writeMicroseconds(powerMot);
      motorVFR.writeMicroseconds(powerMot2);
      motorVFL.writeMicroseconds(powerMot2);
    }else   // Front incline
    {
      /* On powers */
      motorVBR.writeMicroseconds(powerMot2);
      motorVBL.writeMicroseconds(powerMot2);
      motorVFR.writeMicroseconds(powerMot);
      motorVFL.writeMicroseconds(powerMot);
    }
    delay(MOTOR_TIME_WORK_STABLE);
    motorVBR.writeMicroseconds(START_POW);
    motorVBL.writeMicroseconds(START_POW);
    motorVFR.writeMicroseconds(START_POW);
    motorVFL.writeMicroseconds(START_POW);
  }

  /* Process right-left incline (Yaw) */
  imuInitAngles();
  if((powerMot = abs(yaw.number - standYaw.number)) > EPSILON_YAW)
  {
    /* Count powers of front and back motors */
    powerMot *= COEF_POW_STABLE;
    powerMot2 = -powerMot;

    /* Clamp powers */
    powerMot = ((powerMot < (MAX_POW - START_POW)) ? powerMot : MAX_POW - START_POW) + START_POW;
    powerMot2 = ((powerMot2 > (MIN_POW - START_POW)) ? powerMot2 : MIN_POW - START_POW) + START_POW;
    
    if((yaw.number - standYaw.number) > 0)  // Right incline
    {
      /* On powers */
      motorHL.writeMicroseconds(powerMot2);
      motorHR.writeMicroseconds(powerMot);
      delay(MOTOR_TIME_WORK_STABLE);
      
      /* Off powers */
      motorHL.writeMicroseconds(START_POW);
      motorHR.writeMicroseconds(START_POW);
    }else   // Left incline
    {
      /* On powers */
      motorHL.writeMicroseconds(powerMot);
      motorHR.writeMicroseconds(powerMot2);
      delay(MOTOR_TIME_WORK_STABLE);
      
      /* Off powers */
      motorHL.writeMicroseconds(START_POW);
      motorHR.writeMicroseconds(START_POW);
    }
  }
}

/* Functions of sensors */
void imuInitAngles()
{
  //startMillis = millis();
  /* Read radian speeds and linear acc*/
  accel.readGXYZ(&ax, &ay, &az);
  gyro.readRadPerSecXYZ(&rx, &ry, &rz);

  /* Update Madgwick's filter */
  filter.setKoeff(STAT_FPS, BETA);
  filter.update(rx, ry, rz, ax, ay, az);

  /* Get Euler's angles */
  yaw.number =  filter.getYawDeg();
  pitch.number = filter.getPitchDeg();
  roll.number = filter.getRollDeg();
  //deltaMillis = millis() - startMillis;
  //fps = 1000 / deltaMillis;
}

void countFillingBallastTank()
{
  //TODO call countDistance
}

void countDepth()
{
  pressure.number = analogRead(PRESSURE_SENSOR) / 1024.0 - 0.1;
}

void countDistance()
{
  while(millis() - lastTimeCount < SONAR_TIME_WAIT);   //What happened with this condition if millis flush after 50 days?
  digitalWrite(SONAR_TRIG, LOW);
  delayMicroseconds(SONAR_LOW_TIME);
  digitalWrite(SONAR_TRIG, HIGH); 
  delayMicroseconds(SONAR_HIGH_TIME);
  digitalWrite(SONAR_TRIG, LOW); 
  impulseTime = pulseIn(SONAR_ECHO, HIGH);
  distance = impulseTime / 58.2;
  lastTimeCount = millis();
}

/* Functions of data and flags exchange */

void sendData()
{
  sendFloat(&yaw);   //change to yaw, without "stand"
  sendFloat(&roll); 
  sendFloat(&pitch);
  sendFloat(&percentFillingBallastTank);
  sendFloat(&pressure);
}

void receiveFloat(floatbyte_un* un)
{
  while(Serial.available() <= 0);
  un->bytes[0] = Serial.read();
  while(Serial.available() <= 0);
  un->bytes[1] = Serial.read();
  while(Serial.available() <= 0);
  un->bytes[2] = Serial.read();
  while(Serial.available() <= 0);
  un->bytes[3] = Serial.read();  
}

void sendFloat(floatbyte_un* un)
{
  Serial.write(un->bytes[0]);
  Serial.write(un->bytes[1]);
  Serial.write(un->bytes[2]);
  Serial.write(un->bytes[3]);
}

void receiveInt(intbyte_un* un)
{
  while(Serial.available() <= 0);
  un->bytes[0] = Serial.read();
  while(Serial.available() <= 0);
  un->bytes[1] = Serial.read();
}

void sendInt(intbyte_un* un)
{
  Serial.write(un->bytes[0]);
  Serial.write(un->bytes[1]);
}

void receiveOrders()
{
  receiveFloat(&standYaw);
  receiveFloat(&standRoll);
  receiveFloat(&standPitch);
  receiveFloat(&standPercentFillingBallastTank);
  receiveFloat(&standPressure);
  receiveInt(&speedHoriz);
  receiveInt(&speedVert);
}

void sendStands()
{
  sendFloat(&standYaw);
  sendFloat(&standRoll);
  sendFloat(&standPitch);
  sendFloat(&standPercentFillingBallastTank);
  sendFloat(&standPressure);
  sendInt(&speedHoriz);
  sendInt(&speedVert);
}

void sendFlag(int flag)
{
  intbyte_un un_flag;
  un_flag.number = flag;
  sendInt(&un_flag);
}

void receiveFlag()
{
  receiveInt(&flagRequest);
}

int waitingForServersRequest()
{
  do
    stableSubmarine();
  while(Serial.available() <= 0);
  receiveFlag();
  if(flagRequest.number > SC_MIN_CODE && flagRequest.number < SC_MAX_CODE_ERR)
  {
    return 1;
  }
  return 0;
}
