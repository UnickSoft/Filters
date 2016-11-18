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
    const MaskBitmap mask = MaskParameter::field(params ? &params->value(0) : &parameterInfo(0).defaultValue);
    const bool bResultOnly = BoolParameter::field(params ? &params->value(1) : &parameterInfo(1).defaultValue);
    
    FrameEx inputFrameEx  = *inputFrame;
    
    //int pixelDepth = inputFrameEx.pixelDepth();
    
    if (inputFrame->format == FrameParams::Alpha8)
    {
        int kernelSizeHalfH = mask.width  / 2;
        int kernelSizeHalfV = mask.height / 2;
        int kernelSizeHalfHEnd = kernelSizeHalfH - ((mask.width % 2 == 0) ? 1 : 0);
        int kernelSizeHalfVEnd = kernelSizeHalfV - ((mask.height % 2 == 0) ? 1 : 0);
        
        auto processAlpha8 = [=, &mask](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i, int j)
        {
            uint8_t value = 0;
            auto maskCenterData = mask.data + (int32_t)mask.byteSpan * kernelSizeHalfV;
            
            for (int l = -kernelSizeHalfV; l <= kernelSizeHalfVEnd; l++)
            {
                auto maskLine  = maskCenterData + (int32_t)mask.byteSpan       * l;
                auto inputLine = inputRow  + (int32_t)inputFrame.byteSpan * l - kernelSizeHalfH;
                for (int k = -kernelSizeHalfH; k <= kernelSizeHalfHEnd; k++)
                {
                    if (*maskLine > 0)
                    {
                        if (value < *inputLine)
                        {
                            value = *inputLine;
                        }
                    }
                    
                    ++inputLine;
                    ++maskLine;
                }
            }
            
            // Todo create separate function for bResultOnly true/false.
            *outputRow = (bResultOnly ? ((value != *inputRow) ? value : 0) : value);
        };
        
        ROI roi  = {static_cast<uint32_t>(kernelSizeHalfH), static_cast<uint32_t>(kernelSizeHalfV), inputFrame->width - 2 * kernelSizeHalfH, inputFrame->height - 2 * kernelSizeHalfV};
        
        FrameEx inputFrameEx  = *inputFrame;
        FrameEx outputFrameEx = *outputFrame;
        
        // Horizontal filter
        return processFrameToFramePixel(processAlpha8, inputFrameEx, outputFrameEx, &roi, &roi);
    }
    
    // Process function.
    /*
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
    return 2;
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
    else if (index == 1)
    {
        static BoolParameterInfo resOnly("Result only", false, false, true);
        return resOnly;
    }
    return emptyParam;
}

// @return name. Latin only letters.
const char* const Dilate::name()
{
    return "Dilate";
}
