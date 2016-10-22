//
//  BaseTypes.h
//  This file holds base parameters of effect.
//
//  Created by Олег on 09.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef BaseTypes_h
#define BaseTypes_h

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
    };
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
    };
};


// Parameter helper.
template <typename T> struct NumberParameter : public Parameter
{
    NumberParameter (const T& value)
    {
        field(this) = value;
    }
    
    static T& field(Parameter* parameter);
};

template <> float& NumberParameter<float>::field(Parameter* parameter)
{
    return parameter->floatNumber;
}
template <> int32_t& NumberParameter<int32_t>::field(Parameter* parameter)
{
    return parameter->intNumber;
}
template <> uint32_t& NumberParameter<uint32_t>::field(Parameter* parameter)
{
    return parameter->uintNumber;
}

// Parameter Info Helper
template <typename T> struct NumberParameterInfo : public ParameterInfo
{
    NumberParameterInfo (const char* title, const T& defaultNumber, const T& minNumber, const T& maxNumber)
    {
        this->title = title;
        NumberParameter<T>::field(&defaultValue) = defaultNumber;
        NumberParameter<T>::field(&range.min) = minNumber;
        NumberParameter<T>::field(&range.max) = maxNumber;
        type = paramType();
    }
    
    BaseParameters paramType();
};

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




// Typedef for each number parameter
typedef NumberParameterInfo<int32_t>  IntParameterInfo;
typedef NumberParameterInfo<uint32_t> UintParameterInfo;
typedef NumberParameterInfo<float>    FloatParameterInfo;



#endif /* BaseTypes_h */
