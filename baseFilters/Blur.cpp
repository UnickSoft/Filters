//
//  Blur.cpp
//  test
//
//  Created by Олег on 11.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include <iostream>
#include "Blur.h"
#include "ParameterHelpers.h"


Blur::Blur (const IPrivateFilterList* filterList, IResourceManager* resourceManager)
{}

// Apply filter to frame.
bool Blur::apply(const Frame* inputFrame, Frame* outputFrame, const IParameterSet* params)
{
    const int kernelSize = params ? params->value(0).value.uintNumber : parameterInfo(0).defaultValue.value.uintNumber;
    
    const int kernelSizeHalf = kernelSize / 2;
    
    unsigned int kernel[kernelSize];
    unsigned int kernelNorm = 0;
    for (int i = 0; i < kernelSizeHalf; i ++)
    {
        kernel[i] = i + 1;
        kernel[kernelSize - 1 - i] = i + 1;
        kernelNorm += 2 * (i + 1);
    }
    kernel[kernelSizeHalf] = kernelSizeHalf + 1;
    kernelNorm += kernelSizeHalf + 1;
    
    auto sourceData = inputFrame->data;
    auto destData   = outputFrame->data;
    for (int i = kernelSize; i < inputFrame->height; i++)
    {
        auto sourceRow = sourceData + inputFrame->byteSpan * i;
        auto destRow   = destData   + outputFrame->byteSpan * i;
        for (int j = 0; j < inputFrame->width; j++)
        {
            unsigned int value[3] = {};
            for (int k = -kernelSizeHalf; k <= kernelSizeHalf; k++)
            {
                value[0] += kernel[k + kernelSizeHalf] * sourceRow[k * 3];
                value[1] += kernel[k + kernelSizeHalf] * sourceRow[k * 3 + 1];
                value[2] += kernel[k + kernelSizeHalf] * sourceRow[k * 3 + 2];
            }
            
            destRow[0] = value[0] / kernelNorm;
            destRow[1] = value[1] / kernelNorm;
            destRow[2] = value[2] / kernelNorm;
            
            destRow += 3;
            sourceRow += 3;
        }
    }
    
    return true;
}

// @return number of parameters.
index_t Blur::parameterNumber()
{
    return 1;
}

// @return parameter info.
const ParameterInfo& Blur::parameterInfo(index_t index)
{
    static ParameterInfo emptyParam;
    if (index == 0)
    {
        static UintParameterInfo radius("radius", 31, 0, 32);
        return radius;
    }
    return emptyParam;
}

// @return name. Latin only letters.
const char* const Blur::name()
{
    return "Blur";
}

