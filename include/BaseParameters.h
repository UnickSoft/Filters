//
//  BaseTypes.h
//  This file holds base parameters of effect.
//
//  Created by Олег on 09.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef BaseTypes_h
#define BaseTypes_h

#include "Base.h"

enum BaseParameters {
    BS_UNKNOWN = 0, // Unknown parameter.
    BS_FLOAT,       // Float parameter. It is float32 type.
    BS_INT,         // Int parameter. It is int32 type.
    BS_UINT,        // Int parameter. It is uint32 type.
    BS_MAX = 32};


// Struct to hold any type of parameter.
// We will use helper to write/read parameter from this struct.
struct Parameter
{
    union
    {
        float    floatNumber;
        int32_t  intNumber;
        uint32_t uintNumber;
    } value;
};

// Range of parameters.
struct ParamRange
{
    Parameter min;
    Parameter max;
};

// List of parameters.
struct ParamList
{
    Parameter* list;
    index_t number;
};

// This struct hold parameter info.
struct ParameterInfo
{
    uint32_t     type;
    const char*  title;
    Parameter    defaultValue;
    union
    {
        ParamRange range;
        ParamList  list;
    } values;
};


#endif /* BaseTypes_h */
