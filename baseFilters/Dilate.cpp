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


Dilate::Dilate (const IPrivateFilterList& filterList, IResourceManager& resourceManager) : BaseFilter(filterList, resourceManager, "Dilate")
{}

// Apply filter to frame.
bool Dilate::apply(const Frame& inputFrame, Frame& outputFrame, const IParameterSet& params)
{
    const MaskBitmap mask = MaskParameter::field(&params.value(0));
    const bool bResultOnly = BoolParameter::field(&params.value(1));
    
    //int pixelDepth = inputFrameEx.pixelDepth();
    
    if (inputFrame.format == FrameParams::Alpha8)
    {
        int kernelSizeHalfH = mask.width  / 2;
        int kernelSizeHalfV = mask.height / 2;
        int kernelSizeHalfHEnd = kernelSizeHalfH - ((mask.width % 2 == 0) ? 1 : 0);
        int kernelSizeHalfVEnd = kernelSizeHalfV - ((mask.height % 2 == 0) ? 1 : 0);
        
        auto processAlpha8 = [=, &mask](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i, int j)
        {
            uint32_t value = 0;
            uint32_t temp = 0;
            auto maskCenterData = mask.data + (int32_t)mask.byteSpan * kernelSizeHalfV;
            
            for (int l = -kernelSizeHalfV; l <= kernelSizeHalfVEnd; l++)
            {
                auto maskLine  = maskCenterData + (int32_t)mask.byteSpan       * l;
                auto inputLine = inputRow  + (int32_t)inputFrame.byteSpan * l - kernelSizeHalfH;
                for (int k = -kernelSizeHalfH; k <= kernelSizeHalfHEnd; k++)
                {
                    if (*maskLine > 0)
                    {
                        temp = *inputLine * *maskLine;
                        if (value < temp)
                        {
                            value = temp;
                        }
                    }
                    
                    ++inputLine;
                    ++maskLine;
                }
            }
            
            // Todo create separate function for bResultOnly true/false.
            value /= 255;
            *outputRow = (bResultOnly ? value - *inputRow : value);
        };
        
        ROI roi  = {inputFrame.roi.x - kernelSizeHalfH, inputFrame.roi.y - kernelSizeHalfV, inputFrame.roi.width + 2 * kernelSizeHalfH, inputFrame.roi.height + 2 * kernelSizeHalfV};
        
        FrameEx inputFrameEx  = inputFrame;
        FrameEx outputFrameEx = outputFrame;
        
        bool res = processFrameToFramePixel(processAlpha8, inputFrameEx, outputFrameEx, &roi, &roi);

        if (res)
        {
            outputFrame.roi = roi;
        }
        
        return processFrameToFramePixel(processAlpha8, inputFrameEx, outputFrameEx, &roi, &roi);
    }

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
        static uint8_t data[] = {0, 0, 255, 0, 0,
                                 0, 255, 255, 255, 0,
                                 255, 255, 255, 255, 255,
                                 0, 255, 255, 255, 0,
                                 0, 0, 255, 0, 0};
        
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

// @return output frame params for input frame.
FrameParams Dilate::outputFrameParams(const FrameParams& inputFrame)
{
    FrameParams res = {0, 0, FrameParams::Unsupported};
    if (inputFrame.format == FrameParams::Alpha8)
    {
        res = inputFrame;
    }
    return res;
}

//@return output roi. It can be larget or smaller or the same as inout frame.
ROI Dilate::outputRoi(const ROI& inputRoi, const IParameterSet& params)
{
    const MaskBitmap mask = MaskParameter::field(&params.value(0));
    
    return {static_cast<int32_t>(-mask.width), static_cast<int32_t>(-mask.height), inputRoi.width + 2 * mask.width, inputRoi.height + 2 * mask.height};
}
