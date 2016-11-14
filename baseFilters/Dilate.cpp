//
//  Dilate.cpp
//  UiTest
//
//  Created by Олег on 12.11.16.
//  Copyright © 2016 Oleg. All rights reserved.
//


#include <iostream>
#include "Dilate.h"
#include "ParameterHelpers.h"
#include "FilterTemplates.h"
#include <memory>
#include "BaseParameterSet.h"


Dilate::Dilate (const IPrivateFilterList* filterList, IResourceManager* resourceManager) : resourceManager(*resourceManager), filterList(*filterList)
{}

// Apply filter to frame.
bool Dilate::apply(const Frame* inputFrame, Frame* outputFrame, const IParameterSet* params)
{
/*
    const int kernelSizeHalf = UintParameter::field(params ? &params->value(0) : &parameterInfo(0).defaultValue);
    
    const int kernelSize = kernelSizeHalf * 2 + 1;
    
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
    
    FrameEx inputFrameEx  = *inputFrame;
    
    int pixelDepth = inputFrameEx.pixelDepth();
    
    // Process function.
    if (inputFrame->format == FrameParams::RGB8)
    {
        auto processRGB8H = [=, &kernel](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i, int j)
        {
            unsigned int value[3] = {};
            for (int k = -kernelSizeHalf; k <= kernelSizeHalf; k++)
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
        
        auto processRGB8V = [=, &kernel](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i, int j)
        {
            unsigned int value[3] = {};
            int32_t byteSpan = inputFrame.byteSpan;
            
            for (int k = -kernelSizeHalf; k <= kernelSizeHalf; k++)
            {
                auto ck = kernel[k + kernelSizeHalf];
                value[0] += ck * inputRow[k * byteSpan];
                value[1] += ck * inputRow[k * byteSpan + 1];
                value[2] += ck * inputRow[k * byteSpan + 2];
            }
            
            outputRow[0] = value[0] / kernelNorm;
            outputRow[1] = value[1] / kernelNorm;
            outputRow[2] = value[2] / kernelNorm;
        };
        
        
        return process(processRGB8H, processRGB8V, *inputFrame, *outputFrame, kernelSizeHalf);
    }
    else if (inputFrame->format == FrameParams::RGBA8)
    {
        auto processRGBA8H = [=, &kernel](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i, int j)
        {
            unsigned int value[4] = {};
            for (int k = -kernelSizeHalf; k <= kernelSizeHalf; k++)
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
        
        auto processRGBA8V = [=, &kernel](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i, int j)
        {
            unsigned int value[4] = {};
            int32_t byteSpan = inputFrame.byteSpan;
            
            for (int k = -kernelSizeHalf; k <= kernelSizeHalf; k++)
            {
                auto ck = kernel[k + kernelSizeHalf];
                value[0] += ck * inputRow[k * byteSpan];
                value[1] += ck * inputRow[k * byteSpan + 1];
                value[2] += ck * inputRow[k * byteSpan + 2];
                value[3] += ck * inputRow[k * byteSpan + 3];
            }
            
            outputRow[0] = value[0] / kernelNorm;
            outputRow[1] = value[1] / kernelNorm;
            outputRow[2] = value[2] / kernelNorm;
            outputRow[3] = value[3] / kernelNorm;
        };
        
        
        return process(processRGBA8H, processRGBA8V, *inputFrame, *outputFrame, kernelSizeHalf);
    }
    else if (inputFrame->format == FrameParams::Alpha8)
    {
        auto processAlpha8H = [=, &kernel](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i, int j)
        {
            unsigned int value = 0;
            for (int k = -kernelSizeHalf; k <= kernelSizeHalf; k++)
            {
                auto ck = kernel[k + kernelSizeHalf];
                value += ck * inputRow[k * pixelDepth];
            }
            
            outputRow[0] = value / kernelNorm;
        };
        
        auto processAlpha8V = [=, &kernel](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i, int j)
        {
            unsigned int value = 0;
            int32_t byteSpan = inputFrame.byteSpan;
            
            for (int k = -kernelSizeHalf; k <= kernelSizeHalf; k++)
            {
                auto ck = kernel[k + kernelSizeHalf];
                value += ck * inputRow[k * byteSpan];
            }
            
            outputRow[0] = value / kernelNorm;
        };
        
        
        return process(processAlpha8H, processAlpha8V, *inputFrame, *outputFrame, kernelSizeHalf);
    }
*/
    return false;
}

// @return number of parameters.
index_t Dilate::parameterNumber()
{
    return 1;
}

// @return parameter info.
const ParameterInfo& Dilate::parameterInfo(index_t index)
{
    static ParameterInfo emptyParam;
    if (index == 0)
    {
        static uint8_t data[] = {0, 0, 1, 0, 0,
                                 0, 1, 1, 1, 0,
                                 1, 1, 1, 1, 1,
                                 0, 1, 1, 1, 0,
                                 0, 0, 1, 0, 0};
        
        MaskBitmap bitmap = {data, 5, 5, 5};
        MaskBitmap minBitmap = {nullptr, 3, 3,   0};
        MaskBitmap maxBitmap = {nullptr, 32, 32, 0};
        
        static MaskParameterInfo mask("mask", bitmap, minBitmap, maxBitmap);
        return mask;
    }
    return emptyParam;
}

// @return name. Latin only letters.
const char* const Dilate::name()
{
    return "Dilate";
}
