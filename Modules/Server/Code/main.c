#define THIS_SERVER

#include "Headers/serv_settings.h"
#include "eth_control.h"
#include "gen_cvdata.h"
#include "serial_control.h"

int main(void)
{
	/* Declaration */
	#ifdef PRINT_DEBUG
	coord_t x_deb, y_deb;
	int k_deb, n_obj_deb;
	char buf_deb[SIZE_BUF_DEBUG];
	#endif
	short flag;
	int listener;
	Host *hosts;
	char pingResponse[PING_RESP_LEN];
	byte availServer, availPhot;
	CVData *data51, *data52;
	char phName51[] = "ph51.png", phName52[] = "ph52.png"; 
	
	/* Prepare for working */
	//Prepare logs
	LOG_CREATE();
	LOG_PRINT("Server. Start work");

	hosts = (Host*)malloc(sizeof(Host) * 2);
	initHosts(hosts);
	data51 = initDataForReceive();
	data52 = initDataForReceive();

	LOG_PRINT("Server. Try start listen");
	//listener = startListen();
	LOG_PRINT("Server. Started listen");

	//pingHosts(hosts, pingResponse);
	LOG_PRINT("Server. Hosts was pinged");

	LOG_PRINT("Server. Try connect to Phot51");
	//connectToPhot(hosts, listener);
	LOG_PRINT("Server. Connected to Phot51");

	LOG_PRINT("Server. Try connect to Phot52");
	//connectToPhot(hosts + 1, listener);
	LOG_PRINT("Server. Connected to Phot52");

	LOG_PRINT("Server. Try connect to Arduino");
	initPort();
	LOG_PRINT("Server. Connected to Arduino");

	LOG_PRINT("Server. Try connect to Arduino\n");	//(MUST_HAVE)
	tryConnect();
	LOG_PRINT("Server. Connect to Arduino success\n");

	LOG_PRINT("Server. Send standarts to Arduino");
	/*sendFlag(SC_READY_SEND_ORDERS);
	flag = receiveFlag();
	if(flag == SC_OK)
	{
		LOG_PRINT("Server. Receive SC_OK");
	}else
	{
		LOG_PRINT1("Server. Error code from Arduino : %hi\n", flag);
	}*/
	standYaw = 0;
	standRoll = 0;
	standPitch = 0;
	standPercentFillingBallastTank = 100;
	standPressure = 0.2;	//1 atmosphere - over water
	speedHoriz = START_POW;
	speedVert = START_POW;	
	sendOrders();
	checkOrders();
	LOG_PRINT3("Server. Orders sended %f %f %f\n", standYaw, standRoll, standPitch);

	/* Data excnahge */
	while(1)
	{
		LOG_PRINT("Server. Start send ReadyFlag to Phot51");
		sendReadyFlagToPhot(hosts);
		LOG_PRINT("Server. ReadyFlag was sended");

		LOG_PRINT("Server. Start receive data from Phot51");
		data51 = receiveDataFromPhot(hosts, phName51, data51);
		LOG_PRINT("Server. Data was received from Phot51");

		LOG_PRINT("Server. Start send ReadyFlag to Phot52");
		sendReadyFlagToPhot(hosts + 1);
		LOG_PRINT("Server. ReadyFlag was sended");

		LOG_PRINT("Server. Start receive data from Phot52");
		data52 = receiveDataFromPhot(hosts + 1, phName52, data52);
		LOG_PRINT("Server. Data was received from Phot52");

		#ifdef PRINT_DEBUG
		LOG_PRINT("@@@@@@PH51@@@@@@");
		//Show sx
		LOG_PRINT1("****Sx : %hu****", data51->sx);

		//Show sy
		LOG_PRINT1("****Sy : %hu****", data51->sy);

		//Show map
		/*LOG_PRINT("****Map****");
		for(x_deb = 0; x_deb < data51->sx; x_deb++)
		{
			k_deb = 0;
			for(y_deb = 0; y_deb < data51->sy; y_deb++)
			{
				k_deb += sprintf(buf_deb + k_deb, "%i ", data51->map[x_deb][y_deb].object);
			}
			LOG_PRINT1("%s", buf_deb);
		}
		LOG_PRINT("***********");*/

		//Show objects
		LOG_PRINT("****Objects****");
		for(n_obj_deb = 0; n_obj_deb < data51->countObjects; n_obj_deb++)
		{
			LOG_PRINT1("**Obj : %i**", n_obj_deb);
			LOG_PRINT4("x %hu, y %hu, s %i, perim %i", data51->objects[n_obj_deb].x, data51->objects[n_obj_deb].y, data51->objects[n_obj_deb].size, data51->objects[n_obj_deb].perimeter);
			LOG_PRINT4("maxX %hu, minX %hu, maxY %hu, minY %hu", data51->objects[n_obj_deb].maxX, data51->objects[n_obj_deb].minX, data51->objects[n_obj_deb].maxY, data51->objects[n_obj_deb].minY);
		}
		LOG_PRINT("***************");

		LOG_PRINT("@@@@@@PH52@@@@@@");
		//Show sx
		LOG_PRINT1("****Sx : %hu****", data52->sx);

		//Show sy
		LOG_PRINT1("****Sy : %hu****", data52->sy);

		//Show map
		/*LOG_PRINT("****Map****");
		for(x_deb = 0; x_deb < data52->sx; x_deb++)
		{
			k_deb = 0;
			for(y_deb = 0; y_deb < data52->sy; y_deb++)
			{
				k_deb += sprintf(buf_deb + k_deb, "%i ", data52->map[x_deb][y_deb].object);
			}
			LOG_PRINT1("%s", buf_deb);
		}
		LOG_PRINT("***********");*/

		//Show objects
		LOG_PRINT("****Objects****");
		for(n_obj_deb = 0; n_obj_deb < data52->countObjects; n_obj_deb++)
		{
			LOG_PRINT1("**Obj : %i**", n_obj_deb);
			LOG_PRINT4("x %hu, y %hu, s %i, perim %i", data52->objects[n_obj_deb].x, data52->objects[n_obj_deb].y, data52->objects[n_obj_deb].size, data52->objects[n_obj_deb].perimeter);
			LOG_PRINT4("maxX %hu, minX %hu, maxY %hu, minY %hu", data52->objects[n_obj_deb].maxX, data52->objects[n_obj_deb].minX, data52->objects[n_obj_deb].maxY, data52->objects[n_obj_deb].minY);
		}
		LOG_PRINT("***************");
		#endif
		#if defined(PRINT_DEBUG) || defined(PRINT_LOGS)
		sleep(SLEEP_TIME);
		#endif
		//Work woth received data
	}

	/* End work */
	close(hosts[0].sock);
	close(hosts[1].sock);
	free(hosts);
	close(listener);
	
	freeDataForCV(data51);
	free(data51);
	freeDataForCV(data52);
	free(data52);
	LOG_CLOSE();
	return 0;
}
