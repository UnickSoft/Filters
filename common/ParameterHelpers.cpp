//
//  BaseTypes.h
//  This file holds base parameters of effect.
//
//  Created by Олег on 09.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "ParameterHelpers.h"

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
