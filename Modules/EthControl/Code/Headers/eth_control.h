#ifndef ETH_CONTROL
#define ETH_CONTROL

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>

#include "overall_settings.h"
#include "gen_cvdata.h"

/* Define network parameters of computers */
//ATTENTION : Next line using by scripts for compile. You mustn't edit this.
//place_for_define

#if defined(THIS_PHOT51)
#define ADDR_PHOT "192.168.0.52"
#define ADDR_SERVER "192.168.0.100"
#elif defined(THIS_PHOT52)
#define ADDR_PHOT "192.168.0.51"
#define ADDR_SERVER "192.168.0.100"
#elif defined(THIS_SERVER)
#define ADDR_PHOT1 "192.168.0.51"
#define ADDR_PHOT2 "192.168.0.52"
#else
#error "Number of photographer is not defined\n"
#endif

#define TYPE_SERVER 5
#define TYPE_PHOT 6

#define INET_PORT 3425

/* Define flags of state computers */
#define HOST_UNDEF 0
#define HOST_AVAIL 1
#define HOST_NOT_AVAIL 2

#define HOST_READY_RECEIVE_DATA 3
#define HOST_STOP_RECEIVE_DATA 4


/* Define errors */
/* 1) Errors with start processes (SP) */
#define ERROR_SP_WITH_PING -2

/* 2) Errors with create pipe (PC) */
#define ERROR_PC_TO_MUCH_FILE_DESCS -3	
#define ERROR_PC_FULL_FILES_TABLE -4
#define ERROR_PC_INCORRECT_FILE_DESC -5
#define ERROR_PC_UNKNOWN -6

/* 3) Errors with ethernet communication (EC) */
#define ERROR_EC_CREATE_SOCKET -7
#define ERROR_EC_SETUP_CONNECT -8
#define ERROR_EC_RECEIVE_MESSAGE -9
#define ERROR_EC_SX -10
#define ERROR_EC_SY -11
#define ERROR_EC_COUNT_OBJ -12
#define ERROR_EC_IMG -13
#define ERROR_EC_MAP -14
#define ERROR_EC_OBJECTS -15
#define ERROR_EC_RECEIVE_READYFLAG -16
#define ERROR_EC_SIZE_OF_IMG -17
#define ERROR_EC_SEND_READYFLAG -18
#define ERROR_EC_CREATE_LISTENER -19
#define ERROR_EC_BIND -20
#define ERROR_EC_SEND_MESSAGE -21
#define ERROR_FPRINTF_LOG -22

/* Define other */
#define PING_RESP_LEN 1024
#define PERC_LOSS 25
#define SIZE_OF_IP_ADDR 15
#define SIZE_REC_PACKAGE 1024
#define SIZE_QUEUE_LISTEN 10

/* Define aliases of types */
typedef struct sockaddr_in sockaddr_in;

/* Define structures ant functions */
typedef struct Host
{
	byte type;
	sockaddr_in addr;
	byte state;
	char *ip;
	int sock;
} Host;

void initHosts(Host *hosts);
int getPercPacketLoss(char *src, char *targetSymb);
void createPipe(int *pipeDescs);
void pingHost(byte *resAvail, Host *comp, char *pingResponse);
int pingHosts(Host *hosts, char *pingResponse);
#if defined(THIS_PHOT51) || defined(THIS_PHOT52)
void connectToHost(Host *host);
void connectServer(Host *hosts);
void waitingForServersRequest(Host *host);
void sendData(Host *host, CVData *data, char *phName);
#elif defined(THIS_SERVER)
int startListen();
int connectToPhot(Host *host, int listener);
void sendReadyFlagToPhot(Host *host);
CVData* initDataForReceive();
CVData* receiveDataFromPhot(Host *host, char *phName, CVData *data);
#else
#error "Error with define functions\n"
#endif
int recvall(int s, byte *buf, int len, int flags);
int sendall(int s, byte *buf, int len, int flags);

#endif
