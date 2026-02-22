
#include "pumpcontrol.h"
#define RELAY_PORT 80

AsyncClient pumpClient;

struct PumpContext {
    bool requestInProgress = false;
    bool responseReceived = false;
    char buffer[256];
    size_t len = 0;
};

PumpContext pumpCtx;

void pumpRequest(const char* command)
{
    if (pumpCtx.requestInProgress)
        return;

    if (WiFi.status() != WL_CONNECTED)
        return;

    pumpCtx.requestInProgress = true;
    pumpCtx.responseReceived = false;
    pumpCtx.len = 0;

    pumpClient.onConnect([](void* arg, AsyncClient* client)
    {
        char request[128];

        snprintf(request, sizeof(request),
            "GET /cm?cmnd=Power%%20%s HTTP/1.1\r\n"
            "Host: %s\r\n"
            "Connection: close\r\n\r\n",
            (const char*)arg,
            PUMP_IP
        );

        client->write(request);

    }, (void*)command);


    pumpClient.onData([](void* arg, AsyncClient* client, void* data, size_t len)
    {
        if (pumpCtx.len + len >= sizeof(pumpCtx.buffer))
            return;

        memcpy(pumpCtx.buffer + pumpCtx.len, data, len);
        pumpCtx.len += len;
        pumpCtx.buffer[pumpCtx.len] = 0;

        char* jsonStart = strchr(pumpCtx.buffer, '{');
        char* jsonEnd = strrchr(pumpCtx.buffer, '}');

        if (jsonStart && jsonEnd)
        {
            StaticJsonDocument<128> doc;

            if (deserializeJson(doc, jsonStart) == DeserializationError::Ok)
            {
                const char* state = doc["POWER"];
                pumpCtx.responseReceived = true;
            }
        }

    }, NULL);


    pumpClient.onDisconnect([](void* arg, AsyncClient* client)
    {
        pumpCtx.requestInProgress = false;
        pumpCtx.len = 0;
    }, NULL);


    pumpClient.onError([](void* arg, AsyncClient* client, int8_t error)
    {
        pumpCtx.requestInProgress = false;
    }, NULL);


    pumpClient.connect(PUMP_IP, RELAY_PORT);
}



void turnOnPumpAsync()
{
    pumpRequest("ON");
}


void turnOffPumpAsync()
{
    pumpRequest("OFF");
}
