#pragma once

#include "../GFX/Activity.h"
#include "../GFX/Layout.h"
#include "../Pedal.h"
#include "../Views/Header.h"
#include "../Views/Label.h"
#include "../Views/ProgressBar.h"
#include "../Views/Spacer.h"

class WiFiConnectionActivity : public Activity
{
public:
    Spacer topSpacer = Spacer(Size(128, 12));
    Label connectingLabel = Label(ContainerSizeMode::MATCH_PARENT, ContainerSizeMode::WRAP_CONTENT, "Connecting to", 1, Color::WHITE);
    Spacer betweenSpacer = Spacer(Size(128, 12));
    Label ssidLabel = Label(ContainerSizeMode::MATCH_PARENT, ContainerSizeMode::WRAP_CONTENT);

    WiFiConnectionActivity(const String & ssid) : Activity()
    {
        pedal.setLED(0);
        connectingLabel.verticalAlignment = VTextAlignment::Center;
        connectingLabel.horizontalAlignment = HTextAlignment::Center;
        ssidLabel.verticalAlignment = VTextAlignment::Center;
        ssidLabel.horizontalAlignment = HTextAlignment::Center;
        ssidLabel.text = ssid;
        add(topSpacer);
        add(connectingLabel);
        add(betweenSpacer);
        add(ssidLabel);
    }

    void onDraw(ViewGFX * gfx)
    {
        Activity::onDraw(gfx);
    }

    void onConnect(const String & ipAddress)
    {
        connectingLabel.text = "Connected!";
        ssidLabel.text = ipAddress;
        pedal.draw();
        for(int i = 0; i < 256; i++)
        {
            pedal.setLED(i);
            delay(4);
        }
        delay(1000);
    }

    void onError()
    {
        connectingLabel.text = "Failed to Connect!";
        ssidLabel.text = "...";
        pedal.draw();
        for (int i = 0; i < 20; i++)
        {
            if (i % 2 == 0)
            {
                pedal.setLED(255);
            }
            else
            {
                pedal.setLED(0);
            }
            delay(100);
        }
    }
};