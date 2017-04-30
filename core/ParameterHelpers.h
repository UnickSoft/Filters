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
#include "ParameterHelpers.h"
#include <string>
#include "json.hpp"

using json = nlohmann::json;

// Parameter helper.
template <typename T> struct NumberParameter : public Parameter
{
    NumberParameter (const T& value)
    {
        field(this) = value;
    }
    
    NumberParameter (const Parameter* parameter) : NumberParameter(field(parameter)) {}
    
    static T& field(Parameter* parameter)
    {
        return const_cast<T&>(NumberParameter<T>::field(static_cast<const Parameter*>(parameter)));
    }
    
    static const T& field(const Parameter* parameter);
    
    static json save(const Parameter* parameter);
    
    static Parameter load(const json& data);
    
    static void remove(Parameter& data);
};


// @return json string of parameter.
json saveParameter(BaseParameters type, const Parameter* parameter);
Parameter loadParameter(BaseParameters type, const json& data);
void removeParameter(BaseParameters type, Parameter& parameter);


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
    
    static BaseParameters paramType();
};


// Typedef for each number parameter
typedef NumberParameter<int32_t>  IntParameter;
typedef NumberParameter<uint32_t> UintParameter;
typedef NumberParameter<float>    FloatParameter;
typedef NumberParameter<ROI>      ROIParameter;
typedef NumberParameter<MaskBitmap>    MaskParameter;
typedef NumberParameter<bool>    BoolParameter;
typedef NumberParameter<Color>    ColorParameter;

typedef NumberParameterInfo<int32_t>  IntParameterInfo;
typedef NumberParameterInfo<uint32_t> UintParameterInfo;
typedef NumberParameterInfo<float>    FloatParameterInfo;
typedef NumberParameterInfo<ROI>      ROIParameterInfo;
typedef NumberParameterInfo<MaskBitmap>      MaskParameterInfo;
typedef NumberParameterInfo<bool>      BoolParameterInfo;
typedef NumberParameterInfo<Color>     ColorParameterInfo;
