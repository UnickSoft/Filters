//
//  BaseTypes.h
//  This file holds base parameters of effect.
//
//  Created by Олег on 09.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "ParameterHelpers.h"
/*
template <> float& NumberParameter<float>::field(Parameter* parameter)
{
    return parameter->value.floatNumber;
}
template <> int32_t& NumberParameter<int32_t>::field(Parameter* parameter)
{
    return parameter->value.intNumber;
}
template <> uint32_t& NumberParameter<uint32_t>::field(Parameter* parameter)
{
    return parameter->value.uintNumber;
}
template <> ROI& NumberParameter<ROI>::field(Parameter* parameter)
{
    return parameter->value.roi;
}
template <> MaskBitmap& NumberParameter<MaskBitmap>::field(Parameter* parameter)
{
    return parameter->value.mask;
}
template <> bool& NumberParameter<bool>::field(Parameter* parameter)
{
    return parameter->value.boolean;
}
*/
/*
template <typename T> T& NumberParameter<T>::field(Parameter* parameter)
{
    return const_cast<T&>(NumberParameter<T>::field(static_cast<const Parameter*>(parameter)));
}
*/


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
