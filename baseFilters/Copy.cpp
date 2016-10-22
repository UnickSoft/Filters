//
//  Copy.cpp
//  test
//
//  Created by Олег on 18.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "Copy.h"


#include <iostream>
#include "BaseParameters.h"


Copy::Copy (const IPrivateFilterList* filterList, IResourceManager* resourceManager)
{}

// Apply filter to frame.
bool Copy::apply(const Frame* inputFrame, Frame* outputFrame, IParameterSet* params)
{
    bool res = false;
    
    if (inputFrame->byteSpan == outputFrame->byteSpan && inputFrame->width == outputFrame->width
        && inputFrame->width == outputFrame->width)
    {
        memcpy(outputFrame->data, inputFrame->data, inputFrame->height * inputFrame->byteSpan);
        res = true;
    }
    
    return res;
}

// @return number of parameters.
index_t Copy::parameterNumber()
{
    return 0;
}

// @return parameter info.
const ParameterInfo& Copy::parameterInfo(index_t)
{
    static ParameterInfo emptyParam;
    return emptyParam;
}

// @return name. Latin only letters.
const char* const Copy::name()
{
    return "Copy";
}
