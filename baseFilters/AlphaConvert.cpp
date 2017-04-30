//
//  AlphaConvert.cpp
//  UiTest
//
//  Created by Олег on 26.01.17.
//  Copyright © 2017 Oleg. All rights reserved.
//

#include "AlphaConvert.h"
#include "FrameEx.h"
#include "FilterTemplates.h"
#include "BaseParameters.h"

AlphaConvert::AlphaConvert (const IPrivateFilterList& filterList, IResourceManager& resourceManager) : BaseFilter(filterList, resourceManager, "Alpha Convert")
{}

// Apply filter to frame.
bool AlphaConvert::apply(const Frame& inputFrame, Frame& outputFrame, const IParameterSet& params)
{
    if (inputFrame.format == FrameParams::Alpha8)
    {
        FilterPtr copyFilter = createFilter("Copy");
        return BaseFilter::apply(copyFilter.get(), inputFrame, outputFrame, params);
    }
    else if (inputFrame.format == FrameParams::RGBA8)
    {
        auto processRGBA8 = [=](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i, int j)
        {
            *outputRow = inputRow[3];
        };
        
        FrameEx inputFrameEx  = inputFrame;
        FrameEx outputFrameEx = outputFrame;
        
        bool res = processFrameToFramePixel(processRGBA8, inputFrameEx, outputFrameEx);
        
        if (res)
        {
            outputFrame.roi = inputFrame.roi;
        }
        
        return res;
    }
    
    return false;
}

// @return number of parameters.
index_t AlphaConvert::parameterNumber()
{
    return 0;
}

// @return parameter info.
const ParameterInfo& AlphaConvert::parameterInfo(index_t index)
{
    static ParameterInfo emptyParam;
    return emptyParam;
}

// @return output frame params for input frame.
FrameParams AlphaConvert::outputFrameParams(const FrameParams& inputFrame)
{
    FrameParams res = inputFrame;
    res.format = FrameParams::Alpha8;
    return res;
}
