#ifndef SERIAL_CONTROL
#define SERIAL_CONTROL

#define SERIAL_SPEED 115200
#define FLAG_TYPE short
#define SC_READY_RECEIVE 3
#define SC_READY_WORK 4
#define SC_READY_SEND_ORDERS 5
#define SC_OK 6
#define SC_MAX_CODE_ERR 7

#define START_POW 1472

#define ERROR_SC_HAVE_NOT_PORTS -1
#define ERROR_SC_CAN_NOT_OPEN -2

#include <stdio.h>
#include <stdlib.h>
#include <libserialport.h>
#include "overall_settings.h"

extern float standYaw;
extern float standRoll;
extern float standPitch;
extern float standPercentFillingBallastTank;
extern float standPressure;
extern FLAG_TYPE speedHoriz;
extern FLAG_TYPE speedVert;

extern struct sp_port * port;

extern float yaw;
extern float roll;
extern float pitch;
extern float percentFillingBallastTank;
extern float pressure;

void initPort();
void tryConnect();
FLAG_TYPE receiveShort();
void sendShort(FLAG_TYPE number);
float receiveFloat();
void sendFloat(float number);
FLAG_TYPE receiveFlag();
void sendFlag(FLAG_TYPE flag);
void sendOrders();
void checkOrders();
void receiveData();

#endif