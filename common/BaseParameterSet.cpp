//
//  BaseParameterSet.cpp
//  UiTest
//
//  Created by Олег on 22.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "BaseParameterSet.h"
#include <assert.h>

void BaseParameterSet::add(const Parameter& parameter)
{
    parameters.push_back(parameter);
}

const Parameter& BaseParameterSet::value(index_t index) const
{
    assert(index >= 0 && index < parameters.size());
    return parameters.at(index);
}

index_t BaseParameterSet::parametersNumber() const
{
    return parameters.size();
}