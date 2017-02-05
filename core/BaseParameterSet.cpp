//
//  BaseParameterSet.cpp
//  UiTest
//
//  Created by Олег on 22.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "BaseParameterSet.h"
#include <assert.h>

BaseParameterSet::BaseParameterSet(const IParameterSet& paramSet)
{
        for (int i = 0; i < paramSet.parametersNumber(); i++)
        {
            push_back(paramSet.value(i));
        }
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
