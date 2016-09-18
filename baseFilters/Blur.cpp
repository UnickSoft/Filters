//
//  Blur.cpp
//  test
//
//  Created by Олег on 11.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include <iostream>
#include "Blur.h"


Blur::Blur (const IPrivateFilterList* filterList, IResourceManager* resourceManager)
{}

// Apply filter to frame.
bool Blur::apply(const Frame* inputFrame, Frame* outputFrame, IParameterSet* params)
{
    std::cout << "Blur is working" << std::endl;
    return false;
}

// @return number of parameters.
index_t Blur::parameterNumber()
{
    return 0;
}

// @return parameter info.
ParameterInfo Blur::parameterInfo(index_t)
{
    return ParameterInfo();
}

// @return name. Latin only letters.
const char* const Blur::name()
{
    return "Blur";
}

