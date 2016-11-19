//
//  BaseParameterSet.cpp
//  UiTest
//
//  Created by Олег on 22.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "BaseParameterSet.h"
#include <assert.h>

const Parameter& BaseParameterSet::value(index_t index) const
{
    assert(index >= 0 && index < size());
    return at(index);
}

index_t BaseParameterSet::parametersNumber() const
{
    return size();
}