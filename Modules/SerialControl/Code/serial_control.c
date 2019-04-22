#include "Headers/serial_control.h"

float standYaw;
float standRoll;
float standPitch;
float standPercentFillingBallastTank;
float standPressure;
FLAG_TYPE speedHoriz;
FLAG_TYPE speedVert;

struct sp_port * port;

float yaw;
float roll;
float pitch;
float percentFillingBallastTank;
float pressure;

void initPort()
{
	LOG_CREATE();
	sp_get_port_by_name("/dev/ttyUSB0", &port);
	while (SP_OK != sp_open(port, SP_MODE_READ | SP_MODE_WRITE));
	LOG_PRINT1("SerCont. Port %s opened successfully\n", sp_get_port_name(port));

	sp_set_baudrate(port, SERIAL_SPEED);
	sp_set_bits(port, 8);
	sp_set_flowcontrol(port, SP_FLOWCONTROL_NONE);
	sp_set_parity(port, SP_PARITY_NONE);
	sp_set_stopbits(port, 1);
	LOG_PRINT1("SerCont. Port %s configurated successfully\n", sp_get_port_name(port));
}

void tryConnect()
{
	short flag, yes = SC_READY_WORK;
	while(1)
	{
		sp_nonblocking_read(port, &flag, sizeof(short));
		if(flag == SC_READY_WORK)
		{
			sp_blocking_write(port, &yes, sizeof(short), 0);
			break;
		}
	}
	LOG_PRINT("SerCont. Connect setup successfull");
}

FLAG_TYPE receiveShort()
{
	FLAG_TYPE res;
	sp_blocking_read(port, &res, sizeof(FLAG_TYPE), 0);
	return res;
}

void sendShort(FLAG_TYPE number)
{
	sp_blocking_write(port, &number, sizeof(FLAG_TYPE), 0);
}

float receiveFloat()
{
	
	float res;
	sp_blocking_read(port, &res, sizeof(float), 0);
	return res;
}

void sendFloat(float number)
{
	sp_blocking_write(port, &number, sizeof(float), 0);
}

FLAG_TYPE receiveFlag()
{
	return receiveShort();
}

void sendFlag(FLAG_TYPE flag)
{
	sendShort(flag);
}

void sendOrders()
{
	sendFloat(standYaw);
	sendFloat(standRoll);
	sendFloat(standPitch);
	sendFloat(standPercentFillingBallastTank);
	sendFloat(standPressure);
	sendShort(speedHoriz);
	sendShort(speedVert);
}

void checkOrders()
{
	float tmpf;
	short tmph;

	if(standYaw != (tmpf = receiveFloat()))
	{
		LOG_PRINT1("Error yaw %f\n", tmpf);
	}
	if(standRoll != (tmpf = receiveFloat()))
	{
		LOG_PRINT1("Error roll %f\n", tmpf);
	}
	if(standPitch != (tmpf = receiveFloat()))
	{
		LOG_PRINT1("Error pitch %f\n", tmpf);
	}
	if(standPercentFillingBallastTank != (tmpf = receiveFloat()))
	{
		LOG_PRINT1("Error standPercentFillingBallastTank %f\n", tmpf);
	}
	if(standPressure != (tmpf = receiveFloat()))
	{
		LOG_PRINT1("Error standPressure %f\n", tmpf);
	}

	if(speedHoriz != (tmph = receiveShort()))
	{
		LOG_PRINT1("Error speedHoriz %hi\n", tmph);
	}
	if(speedVert != (tmph = receiveShort()))
	{
		LOG_PRINT1("Error speedVert %hi\n", tmph);
	}

}

void receiveData()	//Not a debug version
{
	yaw = receiveFloat();
	roll = receiveFloat();
	pitch = receiveFloat();
	percentFillingBallastTank = receiveFloat();
	pressure = receiveFloat();
}
