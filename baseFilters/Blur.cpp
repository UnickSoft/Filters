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
    const unsigned int kernelSize = 5;
    const unsigned int kernelSizeHalf = kernelSize / 2;
    
    unsigned int kernel[kernelSize];
    unsigned int kernelNorm = 0;
    for (int i = 0; i < kernelSizeHalf; i ++)
    {
        kernel[i] = i;
        kernel[kernelSize - 1 - i] = i;
        kernelNorm += 2 * i;
    }
    kernel[kernelSizeHalf] = kernelSizeHalf;
    kernelNorm += kernelSizeHalf;
    
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
                value[0] += kernel[k + kernelSizeHalf] * sourceRow[k];
                value[1] += kernel[k + kernelSizeHalf] * sourceRow[k + 1];
                value[2] += kernel[k + kernelSizeHalf] * sourceRow[k + 2];
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

