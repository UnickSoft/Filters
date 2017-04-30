//
//  BaseTypes.h
//  This file holds base parameters of effect.
//
//  Created by Олег on 09.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "ParameterHelpers.h"



template <typename T> json NumberParameter<T>::save(const Parameter* parameter)
{
    json res;
    res["value"] =  field(parameter);
    res["type"]  =  NumberParameterInfo<T>::paramType();
    
    return res;
}

template <typename T> Parameter NumberParameter<T>::load(const json& data)
{
    return NumberParameter<T>((T)data["value"]);
}

template <typename T> void NumberParameter<T>::remove(Parameter& parameter)
{}

template <> const float& NumberParameter<float>::field(const Parameter* parameter)
{
    return parameter->value.floatNumber;
}
template <> const int32_t& NumberParameter<int32_t>::field(const Parameter* parameter)
{
    return parameter->value.intNumber;
}
template <> const uint32_t& NumberParameter<uint32_t>::field(const Parameter* parameter)
{
    return parameter->value.uintNumber;
}
template <> const ROI& NumberParameter<ROI>::field(const Parameter* parameter)
{
    return parameter->value.roi;
}
template <> const MaskBitmap& NumberParameter<MaskBitmap>::field(const Parameter* parameter)
{
    return parameter->value.mask;
}
template <> const bool& NumberParameter<bool>::field(const Parameter* parameter)
{
    return parameter->value.boolean;
}
template <> const Color& NumberParameter<Color>::field(const Parameter* parameter)
{
    return parameter->value.color;
}




template <> BaseParameters NumberParameterInfo<float>::paramType()
{
    return BS_FLOAT;
}
template <> BaseParameters NumberParameterInfo<int32_t>::paramType()
{
    return BS_INT;
}
template <> BaseParameters NumberParameterInfo<uint32_t>::paramType()
{
    return BS_UINT;
}
template <> BaseParameters NumberParameterInfo<ROI>::paramType()
{
    return BS_ROI;
}
template <> BaseParameters NumberParameterInfo<MaskBitmap>::paramType()
{
    return BS_MASK;
}
template <> BaseParameters NumberParameterInfo<bool>::paramType()
{
    return BS_BOOL;
}
template <> BaseParameters NumberParameterInfo<Color>::paramType()
{
    return BS_COLOR;
}

template <> json NumberParameter<ROI>::save(const Parameter* parameter)
{
    json res;
    res["x"] =  field(parameter).x;
    res["y"] =  field(parameter).y;
    res["width"] =  field(parameter).width;
    res["height"] =  field(parameter).height;
    res["type"]  =  NumberParameterInfo<ROI>::paramType();
    
    return res;
}

template <> json NumberParameter<MaskBitmap>::save(const Parameter* parameter)
{
    json res;
    res["width"] =  field(parameter).width;
    res["height"] =  field(parameter).height;
    res["byteSpan"] =  field(parameter).byteSpan;
    
    json data;
    for (int i = 0; i < field(parameter).byteSpan * field(parameter).height; i++)
    {
        data.push_back(field(parameter).data[i]);
    }

    res["data"] = data;
    
    res["type"]  =  NumberParameterInfo<MaskBitmap>::paramType();
    
    return res;
}

template <> json NumberParameter<Color>::save(const Parameter* parameter)
{
    json res;
    res["red"] =  field(parameter).red;
    res["green"] =  field(parameter).green;
    res["blue"] =  field(parameter).blue;
    res["alpha"] =  field(parameter).alpha;
    
    res["type"]  =  NumberParameterInfo<Color>::paramType();
    
    return res;
}

template <> Parameter NumberParameter<ROI>::load(const json& data)
{
    Parameter param;
    field(&param).x = data["x"];
    field(&param).y = data["y"];
    field(&param).width  = data["width"];
    field(&param).height = data["height"];

    return param;
}

template <> Parameter NumberParameter<MaskBitmap>::load(const json& data)
{
    Parameter res;
    field(&res).width  = data["width"];
    field(&res).height = data["height"];
    field(&res).byteSpan  = data["byteSpan"];
    
    auto size = field(&res).byteSpan * field(&res).height;
    field(&res).data = new uint8_t[size];
    auto pixels = data["data"];
    for (int i = 0; i < size; i++)
    {
        field(&res).data[i] = (uint8_t)pixels[i];
    }
    field(&res).dynamic = true;
    
    return res;
}

template <> Parameter NumberParameter<Color>::load(const json& data)
{
    Parameter res;
    field(&res).red    = data["red"];
    field(&res).green  = data["green"];
    field(&res).blue   = data["blue"];
    field(&res).alpha  = data["alpha"];
    
    return res;
}

template <> void NumberParameter<MaskBitmap>::remove(Parameter& parameter)
{
    MaskBitmap bitmap =  MaskParameter::field(&parameter);
    if (bitmap.dynamic)
    {
        delete[] bitmap.data;
    }
}

json saveParameter(BaseParameters type, const Parameter* parameter)
{
    switch (type)
    {
        case BS_FLOAT:  return FloatParameter::save(parameter);
        case BS_INT:    return IntParameter::save(parameter);
        case BS_UINT:   return UintParameter::save(parameter);
        case BS_ROI:    return ROIParameter::save(parameter);
        case BS_MASK:   return MaskParameter::save(parameter);
        case BS_BOOL:   return BoolParameter::save(parameter);
        case BS_COLOR:  return ColorParameter::save(parameter);
        default : assert ("Error"); return "";
    }
}

Parameter loadParameter(BaseParameters type, const json& data)
{
    switch (type)
    {
        case BS_FLOAT:  return FloatParameter::load(data);
        case BS_INT:    return IntParameter::load(data);
        case BS_UINT:   return UintParameter::load(data);
        case BS_ROI:    return ROIParameter::load(data);
        case BS_MASK:   return MaskParameter::load(data);
        case BS_BOOL:   return BoolParameter::load(data);
        case BS_COLOR:  return ColorParameter::load(data);
        default : assert ("Error"); return Parameter();
    }
}

void removeParameter(BaseParameters type, Parameter& parameter)
{
    switch (type)
    {
        case BS_FLOAT:  FloatParameter::remove(parameter); break;
        case BS_INT:    IntParameter::remove(parameter); break;
        case BS_UINT:   UintParameter::remove(parameter); break;
        case BS_ROI:    ROIParameter::remove(parameter); break;
        case BS_MASK:   MaskParameter::remove(parameter); break;
        case BS_BOOL:   BoolParameter::remove(parameter); break;
        case BS_COLOR:  ColorParameter::remove(parameter); break;
        default : assert ("Error");
    }
}
