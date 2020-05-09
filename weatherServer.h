#ifndef WEATHER_SERVER_H_
#define WEATHER_SERVER_H_

#include "cc3100_usage.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>

/* JSON Parser include */
#include "jsmn.h"

typedef enum{
    WEATHER_SERVER_SUCCESS_RETURN = 0,
    HTTP_CONNECT_TO_WEATHER_SERVER_FAILED = -1,
    PARSE_JSON_FAILED = HTTP_CONNECT_TO_WEATHER_SERVER_FAILED - 1
} WEATHER_SERVER_ERROR_CODE_t;

#define MAX_WEATHER_TEXT_LEN 16
typedef struct dailyWeatherData{
    uint8_t month;
    uint8_t day;
    float max_temp;
    float min_temp;
    char weather[MAX_WEATHER_TEXT_LEN];
    float rain_depth;
} dailyWeatherData_t;

#define SECONDS_IN_A_DAY       86400
#define HOST_NAME              "api.openweathermap.org"
#define HOST_PORT              80
#define GET_FORECAST_URI        "/data/2.5/onecall?lat=28.141131&lon=-82.315402&exclude=minutely,current,hourly&appid=b8a32c20f2c5d78f18ae6d4d6fcf681a"
#define GET_HISTORICAL_URI_TEMPLATE "/data/2.5/onecall/timemachine?lat=28.141131&lon=-82.315402&dt=%d&appid=b8a32c20f2c5d78f18ae6d4d6fcf681a"


WEATHER_SERVER_ERROR_CODE_t connectToWeatherServer();
char* getRawForecastWeatherJSON();
char* getRawHistoricalWeatherJSON(uint32_t dt);
WEATHER_SERVER_ERROR_CODE_t parseRawWeatherJSON(char* rawJSON, jsmntok_t** outputTokenList);
void generatePrevThreeDaysDT(uint32_t currDT, uint32_t* prevDTs);
void fillInWeekWeatherData();

#endif
