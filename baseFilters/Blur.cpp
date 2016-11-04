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
#include "FilterTemplates.h"


Blur::Blur (const IPrivateFilterList* filterList, IResourceManager* resourceManager)
{}

// Apply filter to frame.
bool Blur::apply(const Frame* inputFrame, Frame* outputFrame, const IParameterSet* params)
{
    const int kernelSize = params ? params->value(0).value.uintNumber : parameterInfo(0).defaultValue.value.uintNumber;
    
    // Fill kernel
    const int kernelSizeHalf = kernelSize / 2;
    const int kernelRightEdge = (kernelSize % 2 == 1) ? kernelSizeHalf : kernelSizeHalf - 1 ;
    
    unsigned int kernel[kernelSize];
    unsigned int kernelNorm = 0;
    for (int i = 0; i < kernelSizeHalf; i ++)
    {
        kernel[i] = i + 1;
        kernel[kernelSize - 1 - i] = i + 1;
        kernelNorm += 2 * (i + 1);
    }
    if (kernelSize % 2 == 1)
    {
        kernel[kernelSizeHalf] = kernelSizeHalf + 1;
        kernelNorm += kernelSizeHalf + 1;
    }
    
    ROI roi  = {static_cast<uint32_t>(kernelSizeHalf), 0, inputFrame->width - 2 * kernelSizeHalf, inputFrame->height};
 
    FrameEx inputFrameEx  = *inputFrame;
    FrameEx outputFrameEx = *outputFrame;
    
    int pixelDepth = inputFrameEx.pixelDepth();
    
    // Process function.
    if (inputFrame->format == FrameParams::RGB8)
    {
        auto processRGB8 = [=, &kernel](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i, int j)
        {
            unsigned int value[3] = {};
            for (int k = -kernelSizeHalf; k <= kernelRightEdge; k++)
            {
                auto ck = kernel[k + kernelSizeHalf];
                value[0] += ck * inputRow[k * pixelDepth];
                value[1] += ck * inputRow[k * pixelDepth + 1];
                value[2] += ck * inputRow[k * pixelDepth + 2];
            }
            
            outputRow[0] = value[0] / kernelNorm;
            outputRow[1] = value[1] / kernelNorm;
            outputRow[2] = value[2] / kernelNorm;
        };
        
        
        return processFrameToFramePixel(processRGB8, inputFrameEx, outputFrameEx, &roi, &roi);
    }
    else if (inputFrame->format == FrameParams::RGBA8)
    {
        auto processRGBA8 = [=, &kernel](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i, int j)
        {
            unsigned int value[4] = {};
            for (int k = -kernelSizeHalf; k <= kernelRightEdge; k++)
            {
                auto ck = kernel[k + kernelSizeHalf];
                value[0] += ck * inputRow[k * pixelDepth];
                value[1] += ck * inputRow[k * pixelDepth + 1];
                value[2] += ck * inputRow[k * pixelDepth + 2];
                value[3] += ck * inputRow[k * pixelDepth + 3];
            }
            
            outputRow[0] = value[0] / kernelNorm;
            outputRow[1] = value[1] / kernelNorm;
            outputRow[2] = value[2] / kernelNorm;
            outputRow[3] = value[3] / kernelNorm;
        };
        
        
        return processFrameToFramePixel(processRGBA8, inputFrameEx, outputFrameEx, &roi, &roi);
    }
    else if (inputFrame->format == FrameParams::Alpha8)
    {
        auto processAlpha8 = [=, &kernel](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i, int j)
        {
            unsigned int value = 0;
            for (int k = -kernelSizeHalf; k <= kernelRightEdge; k++)
            {
                auto ck = kernel[k + kernelSizeHalf];
                value += ck * inputRow[k * pixelDepth];
            }
            
            outputRow[0] = value / kernelNorm;
        };
        
        
        return processFrameToFramePixel(processAlpha8, inputFrameEx, outputFrameEx, &roi, &roi);
    }
    
    return false;
    
    /*
    auto sourceData = inputFrame->data;
    auto destData   = outputFrame->data;
    for (int i = 0; i < inputFrame->height; i++)
    {
        auto sourceRow = sourceData + inputFrame->byteSpan * i   + kernelSizeHalf * 3;
        auto destRow   = destData   + outputFrame->byteSpan * i  + kernelSizeHalf * 3;
        for (int j = kernelSizeHalf; j < inputFrame->width - kernelSizeHalf - 1; j++)
        {
            unsigned int value[3] = {};
            for (int k = -kernelSizeHalf; k <= kernelRightEdge; k++)
            {
                value[0] += kernel[k + kernelSizeHalf] * sourceRow[k * 3];
                value[1] += kernel[k + kernelSizeHalf] * sourceRow[k * 3 + 1];
                value[2] += kernel[k + kernelSizeHalf] * sourceRow[k * 3 + 2];
            }
            
            destRow[0] = value[0] / kernelNorm;
            destRow[1] = value[1] / kernelNorm;
            destRow[2] = value[2] / kernelNorm;
            
            destRow   += 3;
            sourceRow += 3;
        }
    }
    
    return true;
    */
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
        static UintParameterInfo radius("radius", 16, 2, 32);
        return radius;
    }
    return emptyParam;
}

// @return name. Latin only letters.
const char* const Blur::name()
{
    return "Blur";
}

