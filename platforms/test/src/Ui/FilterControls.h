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
#include "BaseParameterSet.h"
#include "IFilter.h"


class FilterControls : public QWidget
{
    Q_OBJECT
    
public:

    FilterControls ();
    
    void setFilter(const QString& title, const QVector<ParameterInfo>& parameters, index_t firstParamIndex = 0);
    void setFilter(FilterPtr filter, const BaseParameterSet& parameters, index_t firstParamIndex);
    
signals:

    void paramChanged(index_t index, const Parameter& value);
    void removeFilter(FilterPtr filter);


protected:
    void removeOldControls();

    QWidget* createControl(const ParameterInfo& parameterInfo, index_t index);

    QWidget* createUintControl(const ParameterInfo& parameterInfo, index_t index);
    QWidget* createROIControl(const ParameterInfo& parameterInfo, index_t index);
    QWidget* createMaskControl(const ParameterInfo& parameterInfo, index_t index);
    QWidget* createBoolControl(const ParameterInfo& parameterInfo, index_t index);
    QWidget* createColorControl(const ParameterInfo& parameterInfo, index_t index);
    
    FilterControls* controls;
    FilterPtr filter_;
};
