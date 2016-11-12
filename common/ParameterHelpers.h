//
//  BaseTypes.h
//  This file holds base parameters of effect.
//
//  Created by Олег on 09.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#pragma once

#include "BaseParameters.h"
#include <cstdlib>


// Parameter helper.
template <typename T> struct NumberParameter : public Parameter
{
    NumberParameter (const T& value)
    {
        field(this) = value;
    }
    
    static T& field(Parameter* parameter);
    static const T& field(const Parameter* parameter);
};

// Parameter Info Helper
template <typename T> struct NumberParameterInfo : public ParameterInfo
{
    NumberParameterInfo (const char* title, const T& defaultNumber, const T& minNumber, const T& maxNumber)
    {
        this->title = title;
        NumberParameter<T>::field(&defaultValue) = defaultNumber;
        NumberParameter<T>::field(&values.range.min) = minNumber;
        NumberParameter<T>::field(&values.range.max) = maxNumber;
        type = paramType();
    }
    
    BaseParameters paramType();
};


// Typedef for each number parameter
typedef NumberParameter<int32_t>  IntParameter;
typedef NumberParameter<uint32_t> UintParameter;
typedef NumberParameter<float>    FloatParameter;
typedef NumberParameter<ROI>    ROIParameter;

typedef NumberParameterInfo<int32_t>  IntParameterInfo;
typedef NumberParameterInfo<uint32_t> UintParameterInfo;
typedef NumberParameterInfo<float>    FloatParameterInfo;
typedef NumberParameterInfo<ROI>    ROIParameterInfo;
