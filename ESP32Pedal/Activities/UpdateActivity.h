#pragma once

#include "../GFX/Activity.h"
#include "../Views/ProgressBar.h"
#include "../Views/Label.h"
#include "../Views/Header.h"
#include "../Views/Spacer.h"
#include "../GFX/Layout.h"

class UpdateActivity : public Activity
{
   
public:
    Spacer topSpacer = Spacer(Size(128, 12));
    ProgressBar pbar = ProgressBar(ContainerSizeMode::MATCH_PARENT, ContainerSizeMode::WRAP_CONTENT);
    Label updateLabel = Label(ContainerSizeMode::MATCH_PARENT, ContainerSizeMode::WRAP_CONTENT, "Updating...", 1, Color::WHITE);
    Label progressLabel = Label(ContainerSizeMode::MATCH_PARENT, ContainerSizeMode::WRAP_CONTENT);
    UpdateActivity() : Activity()
    {
        updateLabel.verticalAlignment = VTextAlignment::Center;
        updateLabel.horizontalAlignment = HTextAlignment::Center;
        progressLabel.verticalAlignment = VTextAlignment::Center;
        progressLabel.horizontalAlignment = HTextAlignment::Center;
        pbar.setHeight(15);
        
        pbar.setValue(0);
        add(topSpacer);
        add(updateLabel);
        add(progressLabel);
        add(pbar);
    }

    void onDraw()
    {
        Activity::onDraw();
        progressLabel.text = String(pbar.getValue()) + "%";
    }

    void onError(String mes)
    {
        updateLabel.text = "Error!";
        progressLabel.text = mes;
    }
};