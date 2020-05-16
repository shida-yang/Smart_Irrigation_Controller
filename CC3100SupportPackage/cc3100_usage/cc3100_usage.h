/*
 * cc3100_usage.h
 *
 *  Created on: Feb 16, 2017
 *      Author: Danny
 */

#ifndef CC3100_USAGE_H_
#define CC3100_USAGE_H_

/********************** Includes *********************/
#include "simplelink.h"
#include "sl_common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* HTTP Client lib include */
#include "httpcli.h"
/********************** Includes *********************/

/*
 * Description of CC3100 Usage:
 *
 * TODO: fill this in
 *
 *
 */

/**************************** Defines ******************************/
/*
 * Determines whether player is the host or client
 */
typedef enum
{
    Client = 0,
    Host = 1
}playerType;

/* IP addressed of server side socket.
 * Should be in long format, E.g: 0xc0a8010a == 192.168.1.10
 */
//192.168.254.122
#define HOST_IP_ADDR           0xC0A8FE15               // IP address of server to connect to
#define PORT_NUM               5001                     // Port number to be used
#define NO_OF_PACKETS          1                        // Number of packets to send out

/*
 * Static IP address for host
 */
#define CONFIG_IP       SL_IPV4_VAL(192,168,254,123)       /* Static IP to be configured */
#define AP_MASK         SL_IPV4_VAL(255,255,255,0)      /* Subnet Mask for the station */
#define AP_GATEWAY      SL_IPV4_VAL(192,168,254,254)        /* Default Gateway address */
#define AP_DNS          SL_IPV4_VAL(192,168,254,254)            /* DNS Server Address */
#define SL_STOP_TIMEOUT        0xFF

/* Application specific status/error codes */
typedef enum{
    DEVICE_NOT_IN_STATION_MODE = -0x7D0,        /* Choosing this number to avoid overlap w/ host-driver's error codes */
    BSD_UDP_CLIENT_FAILED = DEVICE_NOT_IN_STATION_MODE - 1,

    STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;

#define NOTHING_RECEIVED -1

/* HTTP */
typedef enum{
    JSON=0,
    OTHERS
} HTTPCli_CONTENT_TYPE_t;

#define MAX_BUFF_SIZE   4096

/*
https://www.adamspest.biz/about/our-blog/2-florida-irrigation-how-much-should-i-water-my-lawn
Spring: 1-2 times a week for a total of 1.0 to 1.5 inches per week
Summer: 2-3 times a week for a total of 2.0 to 2.5 inches per week
Fall: 1-2 times a week for a total of 1.0 to 1.5 inches per week
Winter: 1 time a week for a total of 0.5 to 1.0 inches per week
*/
#define TARGET_WATER_LEVEL_SPRING_IN    1.5
#define TARGET_WATER_LEVEL_SUMMER_IN    2.5
#define TARGET_WATER_LEVEL_FALL_IN      1.5
#define TARGET_WATER_LEVEL_WINTER_IN    1
#define SINGLE_TIME_WATER_LEVEL_IN      0.75
/**************************** Defines ******************************/



/*********************** User Functions ************************/
void initCC3100();
_u32 getLocalIP();
_i32 ConnectToHTTPServer(HTTPCli_Handle httpClient_h, char* hostName, uint16_t hostPort);
_i32 GetDataFromHTTPServer(HTTPCli_Handle httpClient_h, char* hostName, char* URI, char** dataPtr);
/*********************** User Functions ************************/


/*********************** HTTP Functions ************************/
static _i32 initializeAppVariables();
static _i32 readResponse(HTTPCli_Handle httpClient_h, char** dataPtr, HTTPCli_CONTENT_TYPE_t* contentType);
static void FlushHTTPResponse(HTTPCli_Handle httpClient_h);
/*********************** HTTP Functions ************************/


/*********************** Event Handlers ************************/
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock);
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent);
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent,
                                  SlHttpServerResponse_t *pHttpResponse);
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent);
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent);
/*********************** Event Handlers ************************/



#endif /* CC3100_USAGE_H_ */
