#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "clientSocket.h"
#include "PubSubClient.h"
#include "ServiceDispatcher.h"
#include "ServiceDispatcherRouter.h"
#include "ServiceDispatcherQueue.h"
#include "x86.h"
#include "Client.h"

#include "serviceRGBServer.h"
#include "serviceRGBClient.h"

#define LOG_ERROR(...)     \
                        perror(__FUNCTION__);                   \
                        printf("ERROR::%s:%s:", __FUNCTION__, strerror(errno) );    \
                        printf(__VA_ARGS__);                    \
                        printf("\r\n");                         \
                        fflush(stdout);

#define LOG_INFO(...)     \
                        printf("INFO ::%s: ",__FUNCTION__);     \
                        printf(__VA_ARGS__);                    \
                        printf("\r\n");                         \
                        fflush(stdout);

void   cbDataReived(char* topic, uint8_t* payload, unsigned int length);
int8_t cbPublishData(SDHandle_t sdqHandle, uint8_t *topic, uint8_t topicLength, uint8_t *data, uint8_t dataLength);


void cbDataReived(char* topic, uint8_t* payload, unsigned int length)
{
    LOG_INFO("Received %s", topic);
    SDQ_AddToRecvQueue(topic, payload, length, NULL);
}

int8_t cbPublishData(SDHandle_t sdqHandle, uint8_t *topic, uint8_t topicLength, uint8_t *data, uint8_t dataLength)
{
    char *t = (char*)topic;
    topic[topicLength] = 0x0;
    LOG_INFO("Send %s", t);
    if(PubSubClient_publish(t, (uint8_t*)data, dataLength))
    {
        LOG_INFO("    done");
    }
    return 0;
}


int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    clientSocket_init();

    PubSubClient_initHostCallback((Client_t*)&clientSock, &millis, "test.mosquitto.org",   1883, cbDataReived);
//  PubSubClient_initHostCallback((Client_t*)&clientSock, "data.sparkfun.com",    1883, cbDataReived);

    SDR_Init(&PubSubClient_subscribe);
    SDQ_Init(&cbPublishData);

    //Let services subscribe
    S_RGBServer_Init(&SDR_SubscribeService);
    S_RGBClient_Init(&SDR_SubscribeService);

    millis_init();
    while(1)
    {
        LOG_INFO("  Connecting");
        if (PubSubClient_connectId("mqttClient"))
        {
            LOG_INFO("  Subscribing");
            PubSubClient_subscribe("2/+");
            PubSubClient_subscribe("rgbServer/+");
            PubSubClient_subscribe("rgbServer");
            sleep(1);

            uint16_t cnt = 55;
            while(PubSubClient_connected())
            {
                PubSubClient_loop();
                SDQ_Thread();
                sleep(1);

                cnt++;
                if(cnt>60)
                {
                    cnt = 0;
                    PubSubClient_publish("HW", (uint8_t*)"HelloWorld!", 11);
                }
            }
        }
        sleep(2);
    }
    PubSubClient_disconnect();

    return 0;
}
