#pragma once

#include <WiFi.h>
#include <nvs_flash.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "../Activities/UpdateActivity.h"
#include "../Activities/WiFiConnectionActivity.h"
#include "../Pedal.h"

//class Pedal;
//Pedal pedal;

class PedalOTA
{
private:
    UpdateActivity * updateActivity = new UpdateActivity();
    WiFiConnectionActivity * wifiConnectionActivity = nullptr;
public:
    PedalOTA(const char * ssid, const char * pass)
    {
        
        nvs_flash_init();

        wifiConnectionActivity = new WiFiConnectionActivity(String(ssid));
        pedal.setActivity(wifiConnectionActivity);
        pedal.draw();
        WiFi.disconnect();
        
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, pass);
        while (WiFi.waitForConnectResult() != WL_CONNECTED) {
            Serial.println("Connection Failed! Rebooting...");
            wifiConnectionActivity->onError();
            pedal.draw();
            //delay(5000);
            //ESP.restart();
        }
        


        //pedal.setActivity(nullptr);
        // Port defaults to 3232
        // ArduinoOTA.setPort(3232);

        // Hostname defaults to esp3232-[MAC]
        // ArduinoOTA.setHostname("myesp32");

        // No authentication by default
        // ArduinoOTA.setPassword("admin");

        // Password can be set with it's md5 value as well
        // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
        // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
        

        ArduinoOTA
            .onStart([this]() {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
            else // U_SPIFFS
                type = "filesystem";

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            Serial.println("Start updating " + type);
            pedal.setActivity(updateActivity);
            })
            .onEnd([this]() {
            Serial.println("\nEnd");
            delete updateActivity;
            })
            .onProgress([this](unsigned int progress, unsigned int total) {
                
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
            updateActivity->pbar.setValue(((float)progress) / (((float)total) / 100.0));
            pedal.draw();
            })
            .onError([this](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            String mes;
            if (error == OTA_AUTH_ERROR) mes = "Auth Failed";
            else if (error == OTA_BEGIN_ERROR) mes = "Begin Failed";
            else if (error == OTA_CONNECT_ERROR) mes = "Connect Failed";
            else if (error == OTA_RECEIVE_ERROR) mes = "Receive Failed";
            else if (error == OTA_END_ERROR) mes = "End Failed";
            updateActivity->onError(mes);
            pedal.draw();
            });

        ArduinoOTA.begin();

        wifiConnectionActivity->onConnect(WiFi.localIP().toString());
        delete wifiConnectionActivity;
        Serial.println("Ready");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        
    }

    void handle()
    {
        ArduinoOTA.handle();
    }
};