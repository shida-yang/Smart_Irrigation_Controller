#include "weatherServer.h"

HTTPCli_Struct httpClient;
HTTPCli_Handle httpClient_h = &httpClient;

WEATHER_SERVER_ERROR_CODE_t connectToWeatherServer(){
    int32_t retVal = -1;

    /* Connect to HTTP server */
    retVal = ConnectToHTTPServer(httpClient_h, HOST_NAME, HOST_PORT);
    if(retVal < 0){
      return HTTP_CONNECT_TO_WEATHER_SERVER_FAILED;
    }
    return WEATHER_SERVER_SUCCESS_RETURN;
}

char* getRawForecastWeatherJSON(){
    char* ptr=0;
    GetDataFromHTTPServer(httpClient_h, HOST_NAME, GET_FORECAST_URI, &ptr);
    return ptr;
}

WEATHER_SERVER_ERROR_CODE_t parseRawWeatherJSON(char* rawJSON){
    int32_t retVal = -1;

    /* Parse JSON data */
    retVal = ParseJSONData(rawJSON);

    if(retVal < 0)
    {
        return PARSE_JSON_FAILED;
    }

    return WEATHER_SERVER_SUCCESS_RETURN;
}

void generatePrevThreeDaysDT(uint32_t currDT, uint32_t* prevDTs){

}

