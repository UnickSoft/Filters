//
//  ColorControl.hpp
//  UiTest
//
//  Created by Олег on 11.12.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#pragma once

#include <QtWidgets/QWidget>
#include "BaseParameters.h"

class ColorControl : public QWidget
{
    Q_OBJECT
    
public:

    ColorControl (QWidget* parent, const ParameterInfo& parameterInfo);
    
signals:

    void paramChanged(const Parameter& value);
    
private:

    QColor currentColor;
};
