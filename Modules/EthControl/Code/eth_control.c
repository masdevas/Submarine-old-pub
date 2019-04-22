#include "Headers/eth_control.h"

void initHosts(Host *hosts)
{
	#if defined(THIS_PHOT51) || defined(THIS_PHOT52)

	/* Init server */
	hosts[0].addr.sin_family = AF_INET;
	hosts[0].addr.sin_port = htons(INET_PORT);
	hosts[0].addr.sin_addr.s_addr = inet_addr(ADDR_SERVER);
	hosts[0].ip = (char*)malloc(sizeof(char) * (SIZE_OF_IP_ADDR + 1));
	strcpy(hosts[0].ip, ADDR_SERVER);
	hosts[0].type = TYPE_SERVER;

	/* Init phot */
	hosts[1].addr.sin_family = AF_INET;
	hosts[1].addr.sin_port = htons(INET_PORT);
	hosts[1].addr.sin_addr.s_addr = inet_addr(ADDR_PHOT);
	hosts[1].ip = (char*)malloc(sizeof(char) * (SIZE_OF_IP_ADDR + 1));
	strcpy(hosts[1].ip, ADDR_PHOT);
	hosts[1].type = TYPE_PHOT;

	#elif defined(THIS_SERVER)

	/* Init phot */
	hosts[0].addr.sin_family = AF_INET;
	hosts[0].addr.sin_port = htons(INET_PORT);
	hosts[0].addr.sin_addr.s_addr = inet_addr(ADDR_PHOT1);
	hosts[0].ip = (char*)malloc(sizeof(char) * (SIZE_OF_IP_ADDR + 1));
	strcpy(hosts[0].ip, ADDR_PHOT1);
	hosts[0].type = TYPE_PHOT;

	/* Init phot */
	hosts[1].addr.sin_family = AF_INET;
	hosts[1].addr.sin_port = htons(INET_PORT);
	hosts[1].addr.sin_addr.s_addr = inet_addr(ADDR_PHOT2);
	hosts[1].ip = (char*)malloc(sizeof(char) * (SIZE_OF_IP_ADDR + 1));
	strcpy(hosts[1].ip, ADDR_PHOT2);
	hosts[1].type = TYPE_PHOT;

	#else
	#error "Error with init hosts\n"
	#endif
}

int getPercPacketLoss(char *src, char *targetSymb)
{
	/* Declaration */
	char *posStartPerc, *posEndPerc;
	char perc[3];

	/* Working */
	posStartPerc = posEndPerc = strstr(src, targetSymb);	//Find first include "targetSymb" in "src"
	while((*(posStartPerc - 1)) != ' ')		//Translate to start of number percent
		posStartPerc--;
	strncpy(perc, posStartPerc, posEndPerc - posStartPerc);		//Copy number percent to buffer

	return atoi(perc);
}

void createPipe(int *pipeDescs)
{
	if(pipe(pipeDescs) < 0)
		if(errno == EMFILE)
		{
			LOG_PRINT_ERR("Pipe create : Process using too much file descriptors\n");
			exit(ERROR_PC_TO_MUCH_FILE_DESCS);
		}else if(errno == ENFILE)
		{
			LOG_PRINT_ERR("Pipe create : System table of files is full\n");
			exit(ERROR_PC_FULL_FILES_TABLE);
		}else if(errno == EFAULT)
		{
			LOG_PRINT_ERR("Pipe create : Incorrect file descriptor\n");
			exit(ERROR_PC_INCORRECT_FILE_DESC);
		}else
		{
			LOG_PRINT_ERR("Pipe create : Unknown error\n");
			exit(ERROR_PC_UNKNOWN);
		}
}

void pingHost(byte *resAvail, Host *host, char *pingResponse)
{
	/* Declaration */
	int pid, pipeDescs[2];
	//int backupStdout;
	char targetSymb[] = "\045";		//%

	/* Create pipe for ping server */
	createPipe(pipeDescs);

	/* Ping server */
	if( !(pid = fork()) )		// Work in child
	{
		/* Redirect stdout */
		//backupStdout = dup(STDOUT_FILENO);		//For nexts printf in child(no using now)
		close(STDOUT_FILENO);
		dup2(pipeDescs[1], STDOUT_FILENO);

		/* Close pipes descriptors */
		close(pipeDescs[1]);
		close(pipeDescs[0]);

		execl("/bin/ping", "/bin/ping",  host->ip, "-c", "1", "-w", "1", NULL);
	}else if(pid > 0)		//Work in parent
	{
		wait(NULL);

		read(pipeDescs[0], pingResponse, PING_RESP_LEN);

		/* Close pipes descriptors */
		close(pipeDescs[0]);
		close(pipeDescs[1]);

		/* Check percent of loss packages */
		if(getPercPacketLoss(pingResponse, targetSymb) < PERC_LOSS)
			*resAvail = HOST_AVAIL;
		else
			*resAvail = HOST_NOT_AVAIL;
	}else
	{
		LOG_PRINT_ERR("Process start : Error with start ping\n");
		exit(ERROR_SP_WITH_PING);
	}
}

int pingHosts(Host *hosts, char *pingResponse)
{
	byte avails[2];

	while(avails[0] != HOST_AVAIL && avails[1] != HOST_AVAIL)
	{
		pingHost(avails, hosts, pingResponse);
		pingHost(avails + 1, hosts + 1, pingResponse);
	}

	return 0;
}

#if defined(THIS_PHOT51) || defined(THIS_PHOT52)

void connectToHost(Host *host)
{
	/* Create socket for connect to host */
	LOG_PRINT1("Connect To Host. Create socket for %s\n", host->ip);
	if((host->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		LOG_PRINT_ERR("Ethernet communication : Error with create socket\n");
		exit(ERROR_EC_CREATE_SOCKET);
	}
	
	/* Connect to host */
	LOG_PRINT1("Connect To Host. Start connect to %s\n", host->ip);
	if(connect(host->sock, (struct sockaddr *)(&(host->addr)), sizeof(host->addr)) < 0)
    {
    	LOG_PRINT_ERR("Ethernet communication : Error with setup connect\n");
       	exit(ERROR_EC_SETUP_CONNECT);
    }
    LOG_PRINT1("Connect To Host. Connect to %s install successfull\n", host->ip);
    return;
}

void connectServer(Host *host)
{
	connectToHost(host);
}

void waitingForServersRequest(Host *host)
{
	byte recMsg;

	/* Receive data */
	while(1)
	{
		LOG_PRINT("Receive flag : Try receive ReadyFlag\n");
		if(recv(host->sock, &recMsg, sizeof(byte), 0) < 0)
		{
			LOG_PRINT_ERR("Ethernet communication : Error with receive ReadyToReceiveCode\n");
			exit(ERROR_EC_RECEIVE_MESSAGE);
		}else
		{
			LOG_PRINT("Receive flag : Something was received\n");
			if(recMsg == HOST_READY_RECEIVE_DATA)
			{
				LOG_PRINT("Receive flag : Succefful flag was received\n");
				if(sendall(host->sock, &recMsg, sizeof(byte), 0) < 0)
				{	
					LOG_PRINT_ERR("Ethernet communication : Error with send ReadyToReceiveCode\n");
					exit(ERROR_EC_SEND_MESSAGE);
				}
				return;
			}			
			else
			{
				LOG_PRINT_ERR("Ethernet communication : Received wrong ReadyToReceiveCode\n");
				exit(ERROR_EC_RECEIVE_MESSAGE);
			}
		}
	}
}

void sendData(Host *host, CVData *data, char *phName)
{
	/* Declaration */
	int fileDesc, i, nbytes;

	/* Process data */
	LOG_PRINT1("SendData. %s\n", phName);
	fileDesc = open(phName, O_RDONLY);	//FIXME process errors
	
	/* Send sx */
	if((nbytes = sendall(host->sock, (byte*)(&(data->sx)), sizeof(coord_t), 0)) < 0)
	{
		LOG_PRINT_ERR("Ethernet communication : Error with send sx\n");
		exit(ERROR_EC_SX);
	}
	LOG_PRINT2("SendData. sx:%i, bytes : %i\n", data->sx, nbytes);

	/* Send sy */
	if((nbytes = sendall(host->sock, (byte*)(&(data->sy)), sizeof(coord_t), 0)) < 0)
	{
		LOG_PRINT_ERR("Ethernet communication : Error with send sx\n");
		exit(ERROR_EC_SY);
	}
	LOG_PRINT2("SendData. sy:%i, bytes : %i\n", data->sy, nbytes);

	/* Send countObjects */
	if((nbytes = sendall(host->sock, (byte*)(&(data->countObjects)), sizeof(int), 0)) < 0)
	{
		LOG_PRINT_ERR("Ethernet communication : Error with send sx\n");
		exit(ERROR_EC_COUNT_OBJ);
	}
	LOG_PRINT2("SendData. countObj:%i, bytes : %i\n", data->countObjects, nbytes);

	/* Send size of img */
	if((nbytes = sendall(host->sock, (byte*)(&(data->sizeImg)), sizeof(off_t), 0)) < 0)
	{
		LOG_PRINT_ERR("Ethernet communication : Error with send size of img\n");
		exit(ERROR_EC_SIZE_OF_IMG);
	}
	LOG_PRINT2("SendData. sizeImg:%li, bytes : %i\n", data->sizeImg, nbytes);

	/* Send img */
	if((nbytes = sendfile(host->sock, fileDesc, NULL, data->sizeImg)) < 0)
   	{
		LOG_PRINT_ERR("Ethernet communication : Error with send img\n");
		exit(ERROR_EC_IMG);
    }
    LOG_PRINT1("SendData. Send img bytes : %i\n", nbytes);

    /* Send map */
    for(i = 0; i < data->sx; i++)
	{
    	if((nbytes = sendall(host->sock, (byte*)(data->map[i]), sizeof(PixelEth) * data->sy, 0)) < 0)
   		{
			perror("Ethernet communication : Error with send map\n");
			exit(ERROR_EC_MAP);
  		}
  		//LOG_PRINT2("SendData. X : %i, Send img bytes : %i\n", i, nbytes);
	}
	LOG_PRINT("SendData. Map sended\n");

	
	LOG_PRINT("SendData. Send objects\n");
  	/* Send objects */
  	if((nbytes = sendall(host->sock, (byte*)(data->objects), sizeof(ObjParams) * data->countObjects, 0)) < 0)
   	{
		LOG_PRINT_ERR("Ethernet communication : Error with send objects\n");
		exit(ERROR_EC_OBJECTS);
  	}
  	LOG_PRINT1("SendData. Send objects\'s bytes' : %i\n", nbytes);
  	LOG_PRINT("SendData. Objects sended\n");

    /* End working */
    close(fileDesc);
}

#elif defined(THIS_SERVER)

int startListen()
{
	int listener;
	struct sockaddr_in addr;

	if((listener = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    	{
        	LOG_PRINT_ERR("Ethernet communication : Error with create listener\n");
        	exit(ERROR_EC_CREATE_LISTENER);
    	}

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        LOG_PRINT_ERR("Ethernet communication : Error with bind\n");
        exit(ERROR_EC_BIND);
    }

    listen(listener, SIZE_QUEUE_LISTEN);

    return listener;
}

int connectToPhot(Host *host, int listener)
{
	int structLength;

	while((host->sock = accept(listener, (struct sockaddr*)&host->addr, &structLength)) < 0);	//TODO think about addr_client

	return structLength;
}

void sendReadyFlagToPhot(Host *host)
{
	byte readyFlag = HOST_READY_RECEIVE_DATA, recvReadyFlag;
	
	do
	{
		LOG_PRINT("Send flag. Try send flag\n");
		if(sendall(host->sock, &readyFlag, sizeof(byte), 0) < 0)
   		{
			LOG_PRINT_ERR("Ethernet communication : Error with send ReadyFlag\n");
			exit(ERROR_EC_SEND_READYFLAG);
  		}
  		LOG_PRINT1("Send flag. Sended was successfull to %s\n", host->ip);

		if(recvall(host->sock, &recvReadyFlag, sizeof(byte), 0) < 0)
		{
			LOG_PRINT_ERR("Ethernet communication : Error with receive ReadyFlag\n");
			exit(ERROR_EC_RECEIVE_READYFLAG);
		}
		LOG_PRINT1("Send flag. Response : %i\n", recvReadyFlag);

	}while(recvReadyFlag != readyFlag);
}

CVData* initDataForReceive()
{
	int i;
	CVData *data;
	data = (CVData*)malloc(sizeof(CVData));
	data->map = (PixelEth**)malloc(sizeof(PixelEth*) * sx);
	for(i = 0; i < sx; i++)
		data->map[i] = (PixelEth*)malloc(sizeof(PixelEth) * sy);
	data->objects = (ObjParams*)malloc(sx * sy * sizeof(ObjParams));
	data->eqTree = NULL;
	data->queuePix = NULL;
	data->mapPixels = NULL;
	return data;
}

CVData* receiveDataFromPhot(Host *host, char *phName, CVData *data)
{
	int i, j, nbytes;
	off_t k = 0;
	byte buf[SIZE_REC_PACKAGE];
	FILE *img = fopen(phName, "wb");
	
	/* Receive sx */
	if((nbytes = recvall(host->sock, (byte*)(&(data->sx)), sizeof(coord_t), 0)) < 0)
	{
		LOG_PRINT_ERR("Ethernet communication : Error with sx\n");
		exit(ERROR_EC_SX);
	}
	LOG_PRINT2("ReceiveData. sx:%i, bytes : %i\n", data->sx, nbytes);

	/* Receive sy */
	if((nbytes = recvall(host->sock, (byte*)(&(data->sy)), sizeof(coord_t), 0)) < 0)
	{
		LOG_PRINT_ERR("Ethernet communication : Error with sx\n");
		exit(ERROR_EC_SY);
	}
	LOG_PRINT2("ReceiveData. sy:%i, bytes : %i\n", data->sy, nbytes);

	/* Receive countObjects */
	if((nbytes = recvall(host->sock, (byte*)(&(data->countObjects)), sizeof(int), 0)) < 0)
	{
		LOG_PRINT_ERR("Ethernet communication : Error with sx\n");
		exit(ERROR_EC_COUNT_OBJ);
	}
	LOG_PRINT2("ReceiveData. countObj:%i, bytes : %i\n", data->countObjects, nbytes);

	/* Receive size of img */
	if((nbytes = recvall(host->sock, (byte*)(&(data->sizeImg)), sizeof(off_t), 0)) < 0)
	{
		LOG_PRINT_ERR("Ethernet communication : Error with size of img\n");
		exit(ERROR_EC_SIZE_OF_IMG);
	}
	LOG_PRINT2("ReceiveData. sizeImg:%li, bytes : %i\n", data->sizeImg, nbytes);

	/* Receive img */
	while(k < data->sizeImg)
	    if((nbytes = recvall(host->sock, buf, k < data->sizeImg - SIZE_REC_PACKAGE ? SIZE_REC_PACKAGE : data->sizeImg - k, 0)) >= 0)
	    {
	        fwrite(buf, sizeof(char), nbytes, img);
	        k += nbytes;
	    }
	    else
	    {
	     	LOG_PRINT_ERR("Ethernet communication : Error with img\n");
			exit(ERROR_EC_IMG);
	    }
	    LOG_PRINT2("ReceiveData : Image received k = %li, size = %li\n", k, data->sizeImg);

    /* Receive map */    
    for(i = 0; i < data->sx; i++)
    {
    	if(recvall(host->sock, (byte*)(data->map[i]), sizeof(PixelEth) * data->sy, 0) < 0)
    	{
			LOG_PRINT_ERR("Ethernet communication : Error with map\n");
			exit(ERROR_EC_MAP);
  		}
    }
    LOG_PRINT("ReceiveData : Map received\n");

  	/* Receive objects */
  	if(recvall(host->sock, (byte*)(data->objects), sizeof(ObjParams) * data->countObjects, 0) < 0)
   	{
		LOG_PRINT_ERR("Ethernet communication : Error with objects\n");
		exit(ERROR_EC_OBJECTS);
  	}
  	LOG_PRINT("ReceiveData : Objects received\n");
  	
	fclose(img);
	return data;
}


#else
#error "Error with define functions\n"
#endif

int recvall(int s, byte *buf, int len, int flags)
{
	int total = 0;
    int n;

    while(total < len)
    {
        n = recv(s, buf+total, len-total, flags);
        if(n < 0) { break; }
        total += n;
    }

    return (n < 0 ? n : total);
}

int sendall(int s, byte *buf, int len, int flags)
{
    int total = 0;
    int n;

    while(total < len)
    {
        n = send(s, buf+total, len-total, flags);
        if(n < 0) { break; }
        total += n;
    }

    return (n < 0 ? n : total);
}
