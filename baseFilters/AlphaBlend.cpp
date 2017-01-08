//
//  AlphaBlend.cpp
//  UiTest
//
//  Created by Олег on 07.01.17.
//  Copyright © 2017 Oleg. All rights reserved.
//

#include "AlphaBlend.h"
#include "ParameterHelpers.h"
#include "FilterTemplates.h"
#include <memory>
#include "BaseParameterSet.h"

AlphaBlend::AlphaBlend(const IPrivateFilterList& filterList, IResourceManager& resourceManager)
: resourceManager(resourceManager), filterList(filterList) {}

bool AlphaBlend::apply(const Frame* inputFrames, index_t inputFramesNumber, Frame* outputFrames, index_t outputFramesNumber, const IParameterSet& params)
{
    if (inputFramesNumber == 2 && outputFramesNumber == 1 && outputFrames[0].format == FrameParams::RGBA8 && inputFrames[0].format == FrameParams::RGBA8 && inputFrames[1].format == FrameParams::RGBA8)
    {
        auto processAlphaBlend = [=](FrameEx& inputFrame1, FrameEx& inputFrame2, FrameEx& outputFrame, uint8_t* inputRow1, uint8_t* inputRow2, uint8_t* outputRow, int i, int j)
        {
            int32_t a1 = inputRow1[3];
            int32_t a2 = inputRow2[3];
            int32_t tempAlpha = (a1 * 255 + a2 * (255 - a1));
            
            if (tempAlpha > 0)
            {
                int32_t r1 = inputRow1[0] * a1;
                int32_t r2 = inputRow2[0] * a2;
                
                int32_t g1 = inputRow1[1] * a1;
                int32_t g2 = inputRow2[1] * a2;
                
                int32_t b1 = inputRow1[2] * a1;
                int32_t b2 = inputRow2[2] * a2;
                
                outputRow[0] = (r1 * 255 + (r2 * (255 - a1))) / (tempAlpha);
                outputRow[1] = (g1 * 255 + (g2 * (255 - a1))) / (tempAlpha);
                outputRow[2] = (b1 * 255 + (b2 * (255 - a1))) / (tempAlpha);
                
                outputRow[3] = tempAlpha / 255;
            }
            else
            {
                memset(outputRow, 0, sizeof(4));
            }
        };
        
        FrameEx inputFrameEx1  = inputFrames[0];
        FrameEx inputFrameEx2  = inputFrames[1];
        FrameEx outputFrameEx  = outputFrames[0];
        
        return process2FramesToFramePixel(processAlphaBlend, inputFrameEx1, inputFrameEx2, outputFrameEx);
    }
    
    return false;
}

index_t AlphaBlend::inputsNumber()
{
    return 2;
}

index_t AlphaBlend::outputsNumber()
{
    return 1;
}

index_t AlphaBlend::parameterNumber()
{
    return 0;
}

const ParameterInfo& AlphaBlend::parameterInfo(index_t)
{
    static ParameterInfo paramInfo;
    return paramInfo;
}

const char* const AlphaBlend::name()
{
    return "Alpha Blend";
}

bool AlphaBlend::outputFrameParams(const FrameParams* inputFrames, FrameParams* outputFrames)
{
    if (FrameEx::isEquals(inputFrames[0], inputFrames[1]) && inputFrames[0].format == FrameParams::RGBA8)
    {
        outputFrames[0] = inputFrames[0];
        return true;
    }
    
    return false;
}
