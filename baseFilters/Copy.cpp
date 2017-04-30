//
//  Copy.cpp
//  test
//
//  Created by Олег on 18.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "Copy.h"


#include <iostream>
#include "BaseParameters.h"
#include "FrameEx.h"
#include "FilterTemplates.h"


Copy::Copy (const IPrivateFilterList& filterList, IResourceManager& resourceManager) : BaseFilter(filterList, resourceManager, "Copy")
{}

// Apply filter to frame.
bool Copy::apply(const Frame& inputFrame, Frame& outputFrame, const IParameterSet& params)
{
    bool res = false;
    
    if (inputFrame.width  == outputFrame.width
        && inputFrame.height == outputFrame.height
        && inputFrame.format == outputFrame.format)
    {
        outputFrame.roi = inputFrame.roi;
        if (inputFrame.byteSpan == outputFrame.byteSpan && !useRoi(inputFrame) && !useRoi(outputFrame))
        {
            memcpy(outputFrame.data, inputFrame.data, inputFrame.height * inputFrame.byteSpan);
            res = true;
        }
        else
        {
            FrameEx inputFrameEx  = inputFrame;
            FrameEx outputFrameEx = outputFrame;
    
            auto copyByte = inputFrame.roi.width * inputFrameEx.pixelDepth();
            
            // Process function.
            auto processRGB8 = [=](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i)
            {
                memcpy(outputRow, inputRow, copyByte);
            };
            
            res = processFrameToFrameRow(processRGB8, inputFrameEx, outputFrameEx);
        }
    }
    
    return res;
}

// @return number of parameters.
index_t Copy::parameterNumber()
{
    return 0;
}

// @return parameter info.
const ParameterInfo& Copy::parameterInfo(index_t)
{
    static ParameterInfo emptyParam;
    return emptyParam;
}

// @return output frame params for input frame.
FrameParams Copy::outputFrameParams(const FrameParams& inputFrame)
{
    return inputFrame;
}


