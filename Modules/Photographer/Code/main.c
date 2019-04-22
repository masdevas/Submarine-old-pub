//ATTENTION : Next line using by script for compile. You mustn't edit this.
//place_for_define

#include "Headers/phot_settings.h"
#include "eth_control.h"
#include "gen_cvdata.h"

void makePhoto(char *phName, char *fswebcamResponse, char *phSize);

int main(void)
{
	/* Declaration */
	#ifdef PRINT_DEBUG
	coord_t x_deb, y_deb;
	int k_deb, n_obj_deb;
	char buf_deb[SIZE_BUF_DEBUG];
	#endif
	Host *hosts;
	char pingResponse[PING_RESP_LEN], fswebcamResponse[FSWEB_RESP_LEN];
	byte availServer, availPhot, tst;
	CVData *data;
	char phName[] = "ph.png", photoSizeBuf[10];
	
	/* Prepare for working */
	//Prepare logs
	LOG_CREATE();
	LOG_PRINT("Phot. Start work");
	
	//Memory allocate
	hosts = (Host*)malloc(sizeof(Host) * 2);
	data = (CVData*)malloc(sizeof(CVData));

	//Make string with sizes of photo (SXxSY)
	sprintf(photoSizeBuf, "%hux%hu", sx, sy); 
	
	/* Prepare network configure */
	initHosts(hosts);
	LOG_PRINT("Phot. Hosts was initialized");

	pingHosts(hosts, pingResponse);
	LOG_PRINT("Phot. Hosts was pinged");

	connectServer(hosts);
	LOG_PRINT("Phot. Server was connected");

	/* Data exchange */
	while(1)
	{
		LOG_PRINT("Phot. Waiting for server\'s request");
		waitingForServersRequest(hosts);
		LOG_PRINT("Phot. Server\'s request received");
		
		LOG_PRINT("Phot. Start make photo");
		//makePhoto(phName, fswebcamResponse, photoSizeBuf);
		LOG_PRINT("Phot. Photo was made");

		LOG_PRINT("Phot. Start generate CVData");
		data = genCVData(phName, data);
		LOG_PRINT2("Phot. Data was generated sx : %hu, sy : %hu", data->sx, data->sy);
		
		LOG_PRINT("Phot. Start send data");
		sendData(hosts, data, phName);
		LOG_PRINT("Phot. Data was sended");

		#ifdef PRINT_DEBUG
		//Show sx
		LOG_PRINT1("****Sx : %hu****", data->sx);

		//Show sy
		LOG_PRINT1("****Sy : %hu****", data->sy);

		//Show map
		LOG_PRINT("****Map****");
		/*for(x_deb = 0; x_deb < data->sx; x_deb++)
		{
			k_deb = 0;
			for(y_deb = 0; y_deb < data->sy; y_deb++)
			{
				k_deb += sprintf(buf_deb + k_deb, "%i ", data->map[x_deb][y_deb].object);
			}
			LOG_PRINT1("%s", buf_deb);
		}*/
		LOG_PRINT("***********");

		//Show objects
		LOG_PRINT("****Objects****");
		for(n_obj_deb = 0; n_obj_deb < data->countObjects; n_obj_deb++)
		{
			LOG_PRINT1("**Obj : %i**", n_obj_deb);
			LOG_PRINT4("x %hu, y %hu, s %i, perim %i", data->objects[n_obj_deb].x, data->objects[n_obj_deb].y, data->objects[n_obj_deb].size, data->objects[n_obj_deb].perimeter);
			LOG_PRINT4("maxX %hu, minX %hu, maxY %hu, minY %hu", data->objects[n_obj_deb].maxX, data->objects[n_obj_deb].minX, data->objects[n_obj_deb].maxY, data->objects[n_obj_deb].minY);
		}
		#endif
		#if defined(PRINT_DEBUG) || defined(PRINT_LOGS)
		sleep(SLEEP_TIME);
		#endif
	}

	/*End work */
	freeDataForCV(data);
	free(data);
	close(hosts[0].sock);
	close(hosts[1].sock);
	free(hosts);
	LOG_CLOSE();
	return 0;
}

void makePhoto(char *phName, char *fswebcamResponse, char *phSize)
{
	/* Declaration */
	int pid, pipeDescs[2];
	//int backupStdout;

	/* Create pipe for ping server */
	//createPipe(pipeDescs);

	/* Ping server */
	if( !(pid = fork()) )		// Work in child
	{
		/* Redirect stdout */
		//backupStdout = dup(STDOUT_FILENO);		//For nexts printf in child(no using now)
		//close(STDOUT_FILENO);
		//dup2(pipeDescs[1], STDOUT_FILENO);

		/* Close pipes descriptors */
		//close(pipeDescs[1]);
		//close(pipeDescs[0]);

		execl("/usr/bin/fswebcam", "/usr/bin/fswebcam", "--png", "0", "-F", "3", "-S", "2", "-r", phSize, "--no-banner", "--save", phName, NULL);
	}else if(pid > 0)		//Work in parent
	{
		wait(NULL);

		//read(pipeDescs[0], fswebcamResponse, FSWEB_RESP_LEN);

		/* Close pipes descriptors */
		//close(pipeDescs[0]);
		//close(pipeDescs[1]);
	}else
	{
		LOG_PRINT_ERR("Process start : Error with start fswebcam\n");
		exit(ERROR_SP_WITH_FSWEBCAM);
	}
}
