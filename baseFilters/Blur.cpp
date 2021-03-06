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
#include <memory>
#include "BaseParameterSet.h"


Blur::Blur (const IPrivateFilterList& filterList, IResourceManager& resourceManager) : BaseFilter(filterList, resourceManager, "Blur")
{}

// Apply filter to frame.
bool Blur::apply(const Frame& inputFrame, Frame& outputFrame, const IParameterSet& params)
{
    const int kernelSizeHalf = UintParameter::field(&params.value(0));
    
    if (kernelSizeHalf == 0)
    {
        // Simple copy.
        auto copyFilter = filterList.createFilter("Copy", filterList, resourceManager);
        if (copyFilter)
        {
            return BaseFilter::apply(copyFilter, inputFrame, outputFrame, params);
        }
    }
    
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
    
    FrameEx inputFrameEx  = inputFrame;
    
    int pixelDepth = inputFrameEx.pixelDepth();
    
    // Process function.
    if (inputFrame.format == FrameParams::RGB8)
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
        
        
        return process(processRGB8H, processRGB8V, inputFrame, outputFrame, kernelSizeHalf);
    }
    else if (inputFrame.format == FrameParams::RGBA8)
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
        
        
        return process(processRGBA8H, processRGBA8V, inputFrame, outputFrame, kernelSizeHalf);
    }
    else if (inputFrame.format == FrameParams::Alpha8)
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
        
        
        return process(processAlpha8H, processAlpha8V, inputFrame, outputFrame, kernelSizeHalf);
    }
    
    return false;
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
        static UintParameterInfo radius("radius", 16, 0, 32);
        return radius;
    }
    return emptyParam;
}



template <typename HFunc, typename VFunc> bool Blur::process(HFunc hFunc, VFunc vFunc, const Frame& inputFrame, Frame& outputFrame, int kernelSizeHalf)
{
    bool res = false;
    auto tempFrame = resourceManager.createFrame(inputFrame);
    
    ROI roi  = {inputFrame.roi.x - kernelSizeHalf, inputFrame.roi.y - kernelSizeHalf, inputFrame.roi.width + 2 * kernelSizeHalf, inputFrame.roi.height + 2 * kernelSizeHalf};
 
    FrameEx inputFrameEx  = inputFrame;
    FrameEx outputFrameEx = outputFrame;
    FrameEx tempFrameFrameEx = *tempFrame;
    
    // Horizontal filter
    res = processFrameToFramePixel(hFunc, inputFrameEx, tempFrameFrameEx, &roi, &roi);
    
    // Vertical filter
    res = res && processFrameToFramePixel(vFunc, tempFrameFrameEx, outputFrameEx, &roi, &roi);
    
    outputFrame.roi = roi;
    //copyBorderFunction(*tempFrame, outputFrame);
    
    resourceManager.releaseFrame(tempFrame);
    
    return true;
}


// @return output frame params for input frame.
FrameParams Blur::outputFrameParams(const FrameParams& inputFrame)
{
    FrameParams res = {0, 0, FrameParams::Unsupported};
    if (inputFrame.format == FrameParams::RGB8  ||
        inputFrame.format == FrameParams::RGBA8 ||
        inputFrame.format == FrameParams::Alpha8)
    {
        res = inputFrame;
    }
    return res;
}

//@return output roi. It can be larget or smaller or the same as inout frame.
ROI Blur::outputRoi(const ROI& inputRoi, const IParameterSet& params)
{
    const int kernelSizeHalf = UintParameter::field(&params.value(0));
    
    if (kernelSizeHalf == 0)
    {
        return inputRoi;
    }
    
    const int kernelSize = kernelSizeHalf * 2;
    
    return {-kernelSize + inputRoi.x, -kernelSize + inputRoi.x, inputRoi.width + 2 * kernelSize, inputRoi.height + 2 * kernelSize};
}
