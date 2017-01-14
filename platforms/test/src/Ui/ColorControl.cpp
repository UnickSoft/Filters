//
//  ColorControl.cpp
//  UiTest
//
//  Created by Олег on 11.12.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "ColorControl.h"
#include <QtGui/QColor.h>
#include <QtWidgets/QColorDialog.h>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include "ParameterHelpers.h"



ColorControl::ColorControl (QWidget* parent, const ParameterInfo& parameterInfo)
{
    auto layout = new QVBoxLayout();
    setLayout(layout);
    
    auto openButton   = new QPushButton(parameterInfo.title, this);

    layout->addWidget(openButton);
    
    auto displayColor = [=](const Color& color)
    {
        currentColor = QColor(color.red, color.green, color.blue, color.alpha);
        QString qss  = QString("background-color: %1").arg(currentColor.name());
        
        openButton->setStyleSheet(qss);
    };

    auto openColorDialog = [=]()
    {
        auto color = QColorDialog::getColor(currentColor, this, openButton->text(), QColorDialog::ShowAlphaChannel);
        if (color.isValid())
        {
            Color newColor = {(uint8_t)color.red(), (uint8_t)color.green(), (uint8_t)color.blue(), (uint8_t)color.alpha()};
            displayColor(newColor);
            emit paramChanged(ColorParameter(newColor));
        }
    };

    auto color = ColorParameter::field(&parameterInfo.defaultValue);
    displayColor(color);
    
    connect(openButton, &QPushButton::clicked, openColorDialog);
}
