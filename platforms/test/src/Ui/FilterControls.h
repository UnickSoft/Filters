//
//  FilterControls.hpp
//  Controls for one filter.
//
//  Created by Олег on 28.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <QtWidgets/QWidget>
#include "BaseParameters.h"

class FilterControls : public QWidget
{
    Q_OBJECT
    
public:

    FilterControls ();
    
    void setFilter(const QString& title, const QVector<ParameterInfo>& parameters);
    
signals:

    void paramChanged(index_t index, const Parameter& value);


protected:
    void removeOldControls();

    QWidget* createControl(const ParameterInfo& parameterInfo);

    QWidget* createUintControl(const ParameterInfo& parameterInfo);
    
    FilterControls* controls;
};