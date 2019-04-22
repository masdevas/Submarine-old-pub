#include <stdio.h>
#include "../Headers/serial_control.h"

//WORK

int main()
{
	short a, b, flag;
	int i;

	printf("Try init\n");
	initPort();
	printf("Init success\n");

	printf("Try connect\n");	//(MUST_HAVE)
	tryConnect();
	printf("Connect success\n");
	//while(i++ < 1000000);
	//i = 0;
	
	//Init stands
	

	standYaw = 1.1;
	standRoll = 2.3;
	standPitch = 3.9;
	standPercentFillingBallastTank = 100;
	standPressure = 0.2;	//1 atmosphere - over water
	speedHoriz = START_POW;
	speedVert = START_POW;	
	sendOrders();
	printf("Orders sended %f %f %f\n", standYaw, standRoll, standPitch);

	checkOrders();
	//while(i++ < 1000000);
	//i = 0;

	//Check communication
	/*sendFlag(SC_READY_RECEIVE);
	flag = receiveFlag();
	if(flag == SC_OK)
	{
		printf("Receive SC_OK\n");
	}else
	{
		printf("Else, %hi\n", flag);
	}

	receiveData();*/
	
	//printf("%f\n", receiveFloat());
	//printf("%f\n", receiveFloat());
	//printf("%f\n", receiveFloat());
	//printf("%hi\n", receiveShort());
	
	printf("Data Received : %f %f %f\n", standYaw, standRoll, standPitch);

	//while(i++ < 1000000);
	//i = 0;
	//printf("%f\n", receiveFloat());
	
	/*
	short a, b, yes = SC_READY_WORK;
	printf("Try init\n");
	initPort();
	printf("Init success\n");

	printf("Try connect\n");
	while(1)
	{
		sp_nonblocking_read(port, &b, sizeof(short));
		if(b == SC_READY_WORK)
		{
			sp_blocking_write(port, &yes, sizeof(short), 0);
			break;
		}
	}
	printf("Connect success\n");

	while(1)
	{
		a = 200;
		sendFlag(a);
		b = receiveFlag();
		printf("Read %hi\n", b);
	}*/
	return 0;
}	
	


	

