//
//  SolidColor.cpp
//  UiTest
//
//  Created by Олег on 19.11.16.
//  Copyright © 2016 Oleg. All rights reserved.
//


#include <iostream>
#include "SolidColor.h"
#include "ParameterHelpers.h"
#include "FilterTemplates.h"
#include <memory>
#include "BaseParameterSet.h"


SolidColor::SolidColor (const IPrivateFilterList& filterList, IResourceManager& resourceManager) : BaseFilter(filterList, resourceManager, "SolidColor")
{}

// Apply filter to frame.
bool SolidColor::apply(const Frame& inputFrame, Frame& outputFrame, const IParameterSet& params)
{
    const Color color = ColorParameter::field(&params.value(0));
    
    if (inputFrame.format == FrameParams::Alpha8 && outputFrame.format == FrameParams::RGBA8)
    {
        auto processFullAlphaBGRA8 = [=](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i, int j)
        {
            outputRow[0] = color.red;
            outputRow[1] = color.green;
            outputRow[2] = color.blue;
            outputRow[3] = *inputRow;
        };
        
        auto processAlphaBGRA8 = [=](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i, int j)
        {
            outputRow[0] = color.red;
            outputRow[1] = color.green;
            outputRow[2] = color.blue;
            outputRow[3] = int(*inputRow * color.alpha) / 255;
        };
        
        FrameEx inputFrameEx  = inputFrame;
        FrameEx outputFrameEx = outputFrame;
        
        if (color.alpha == 255)
        {
            return processFrameToFramePixel(processFullAlphaBGRA8, inputFrameEx, outputFrameEx);
        }
        else
        {
            return processFrameToFramePixel(processAlphaBGRA8, inputFrameEx, outputFrameEx);
        }
    }
    
    return false;
}

// @return number of parameters.
index_t SolidColor::parameterNumber()
{
    return 1;
}

// @return parameter info.
const ParameterInfo& SolidColor::parameterInfo(index_t index)
{
    static ParameterInfo emptyParam;
    if (index == 0)
    {
        Color color{255, 255, 255, 255};
        
        static ColorParameterInfo colorParameter("color", color, Color(), Color());
        return colorParameter;
    }
    
    return emptyParam;
}

// @return output frame params for input frame.
FrameParams SolidColor::outputFrameParams(const FrameParams& inputFrame)
{
    FrameParams res = {0, 0, FrameParams::Unsupported};
    if (inputFrame.format == FrameParams::Alpha8)
    {
        res = inputFrame;
        res.format = FrameParams::RGBA8;
    }
    return res;
}
