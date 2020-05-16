#ifndef TIME_SERVER_H
#define TIME_SERVER_H

#include "cc3100_usage.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

typedef enum{
    TIME_SERVER_SUCCESS_RETURN = 0,
    HTTP_CONNECT_TO_TIME_SERVER_FAILED = -1
} TIME_SERVER_ERROR_CODE_t;

#define HOST_PORT              80
#define TIME_ZONE_OFFSET       (-4*3600)
#define TIME_HOST              "worldtimeapi.org"
#define TIME_URI               "/api/timezone/America/New_York"


TIME_SERVER_ERROR_CODE_t connectToTimeServer();
void getCurrentTime(struct tm* ts);

#endif
