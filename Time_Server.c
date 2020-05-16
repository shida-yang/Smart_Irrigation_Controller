#include "Time_Server.h"

HTTPCli_Struct httpClient_time;
HTTPCli_Handle httpClient_time_h = &httpClient_time;

TIME_SERVER_ERROR_CODE_t connectToTimeServer(){
    int32_t retVal = -1;

    /* Connect to HTTP server */
    retVal = ConnectToHTTPServer(httpClient_time_h, TIME_HOST, HOST_PORT);

    if(retVal < 0){
      return HTTP_CONNECT_TO_TIME_SERVER_FAILED;
    }
    return TIME_SERVER_SUCCESS_RETURN;
}

void getCurrentTime(struct tm* ts){
    char* tempPtr1;
    char* tempPtr2;

    connectToTimeServer();
    GetDataFromHTTPServer(httpClient_time_h, TIME_HOST, TIME_URI, &tempPtr2);
    HTTPCli_disconnect(httpClient_time_h);

    //..."unixtime":1589586201,...
    tempPtr1 = strstr(tempPtr2, "\"unixtime\"");
    tempPtr1 += 11;
    tempPtr2 = strstr(tempPtr1, ",");
    *tempPtr2 = '\0';
    __time64_t dt = atoi(tempPtr1);

    dt += TIME_ZONE_OFFSET;

    *ts = *localtime(&dt);
}
