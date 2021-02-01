/*******************************************************************************
****版本：V1.0.0
****平台：ESP8266
****日期：2021-01-31
****作者：Qitas
****版权：OS-Q
*******************************************************************************/

#include <ESP8266WiFi.h>
#include "ESPAsyncUDP.h"

const char* ssid = "matismart2019";
const char* password = "88888888";

AsyncUDP udp;
/*******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：
**输出参数 ：
*******************************************************************************/
void setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    // if (WiFi.waitForConnectResult() != WL_CONNECTED)
    // {
    //     Serial.println("WiFi Failed");
    //     while(1)
    //     {
    //         delay(1000);
    //     }
    // }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if(udp.connect(IPAddress(192,168,2,22), 1234))
    {
        Serial.println("UDP connected");
        udp.onPacket([](AsyncUDPPacket packet)
        {
            Serial.print("UDP Packet Type: ");
            Serial.print(packet.isBroadcast()?"Broadcast":packet.isMulticast()?"Multicast":"Unicast");
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(packet.remotePort());
            Serial.print(", To: ");
            Serial.print(packet.localIP());
            Serial.print(":");
            Serial.print(packet.localPort());
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
            //reply to the client
            packet.printf("Got %u bytes of data", packet.length());
        });
        //Send unicast
        udp.print("Hello Server!");
    }
}

/*******************************************************************************
**函数信息 ：
**功能描述 ：
**输入参数 ：
**输出参数 ：
*******************************************************************************/

void loop()
{
    delay(1000);
    //Send broadcast on port 1234
    udp.broadcastTo("Anyone here?", 1234);
}

/*---------------------------(C) COPYRIGHT 2021 OS-Q -------------------------*/
