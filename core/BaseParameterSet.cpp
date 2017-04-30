//
//  BaseParameterSet.cpp
//  UiTest
//
//  Created by Олег on 22.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "BaseParameterSet.h"
#include <assert.h>

BaseParameterSet::BaseParameterSet(bool manageData) : manageData(manageData)
{}

BaseParameterSet::BaseParameterSet(BaseParameterSet&& other )
{
    data.swap(other.data);
    manageData = other.manageData;
}
BaseParameterSet& BaseParameterSet::operator=( BaseParameterSet&& other )
{
    data.swap(other.data);
    manageData = other.manageData;
    return *this;
}

BaseParameterSet::~BaseParameterSet()
{
    clear();
}

const Parameter& BaseParameterSet::value(index_t index) const
{
    assert(index >= 0 && index < size());
    return at(index);
}

index_t BaseParameterSet::parametersNumber() const
{
    return size();
}

void BaseParameterSet::push_back( BaseParameters type,  const Parameter& value )
{
    data.push_back({type, value});
}

void BaseParameterSet::push_back( BaseParameters type,  Parameter&& value )
{
    push_back(type,  (const Parameter&) value);
}

const Parameter& BaseParameterSet::front() const
{
    return data.front().second;
}

void BaseParameterSet::clear()
{
    std::for_each(data.begin(), data.end(), [this](ParamAndType& value)
    {
        preParameterRemove(value);
    });
    
    data.clear();
}

size_t BaseParameterSet::size() const
{
    return data.size();
}

const Parameter& BaseParameterSet::at( size_t pos ) const
{
    return data.at(pos).second;
}

void BaseParameterSet::replace(size_t pos, const Parameter& newValue)
{
    preParameterRemove(data[pos]);
    data[pos].second = newValue;
}


void BaseParameterSet::preParameterRemove(ParamAndType& parameter)
{
    if (manageData)
    {
        removeParameter(parameter.first, parameter.second);
    }
    //NumberParameter<>
}

