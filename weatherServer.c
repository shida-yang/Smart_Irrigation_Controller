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

char* getRawHistoricalWeatherJSON(uint32_t dt){
    char* ptr=0;
    char strBuf[256];
    snprintf(strBuf, 256, GET_HISTORICAL_URI_TEMPLATE, dt);
    GetDataFromHTTPServer(httpClient_h, HOST_NAME, strBuf, &ptr);
    return ptr;
}

WEATHER_SERVER_ERROR_CODE_t parseRawWeatherJSON(char* rawJSON, jsmntok_t** outputTokenList){
    _i32            retVal = 0;
    _i32            noOfToken;
    jsmn_parser     parser;
    jsmntok_t       *tokenList;
    _i8             printBuffer[4];

    char* tempPtr=0;
    tempPtr=strstr(rawJSON, ",\"hourly\"");
    *tempPtr='\0';
    rawJSON=strstr(rawJSON, "\"current\"");

    /* Initialize JSON PArser */
    jsmn_init(&parser);

    /* Get number of JSON token in stream as we we dont know how many tokens need to pass */
    noOfToken = jsmn_parse(&parser, (const char *)rawJSON, strlen((const char *)rawJSON), NULL, 10);
    if(noOfToken <= 0)
    {
        //CLI_Write(" Failed to initialize JSON parser\n\r");
        return PARSE_JSON_FAILED;
    }

    /* Allocate memory to store token */
    tokenList = (jsmntok_t *) malloc(noOfToken*sizeof(jsmntok_t));
    if(tokenList == NULL)
    {
        //CLI_Write(" Failed to allocate memory\n\r");
        return PARSE_JSON_FAILED;
    }

    /* Initialize JSON Parser again */
    jsmn_init(&parser);
    noOfToken = jsmn_parse(&parser, (const char *)rawJSON, strlen((const char *)rawJSON), tokenList, noOfToken);
    if(noOfToken < 0)
    {
        return PARSE_JSON_FAILED;
    }
    else
    {
        *outputTokenList = tokenList;
        return noOfToken;
    }
}

void generatePrevThreeDaysDT(uint32_t currDT, uint32_t* prevDTs){
    prevDTs[0]=currDT-SECONDS_IN_A_DAY;
    prevDTs[1]=prevDTs[0]-SECONDS_IN_A_DAY;
    prevDTs[2]=prevDTs[1]-SECONDS_IN_A_DAY;
}

