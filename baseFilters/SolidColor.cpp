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


SolidColor::SolidColor (const IPrivateFilterList& filterList, IResourceManager& resourceManager) : resourceManager(resourceManager), filterList(filterList)
{}

// Apply filter to frame.
bool SolidColor::apply(const Frame& inputFrame, Frame& outputFrame, const IParameterSet& params)
{
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

// @return name. Latin only letters.
const char* const SolidColor::name()
{
    return "Solid Color";
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
