//
//  Copy.cpp
//  test
//
//  Created by Олег on 18.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "ROICopy.h"


#include <iostream>
#include "BaseParameters.h"
#include "FrameEx.h"
#include "FilterTemplates.h"
#include "ParameterHelpers.h"


ROICopy::ROICopy (const IPrivateFilterList* filterList, IResourceManager* resourceManager)
{}

// Apply filter to frame.
bool ROICopy::apply(const Frame* inputFrame, Frame* outputFrame, const IParameterSet* params)
{
    bool res = false;
    
    if (inputFrame->width  == outputFrame->width
        && inputFrame->height == outputFrame->height
        && inputFrame->format == outputFrame->format)
    {
        if (inputFrame->byteSpan  == outputFrame->byteSpan)
        {
            memcpy(outputFrame->data, inputFrame->data, inputFrame->height * inputFrame->byteSpan);
            res = true;
        }
        else
        {
            FrameEx inputFrameEx  = *inputFrame;
            FrameEx outputFrameEx = *outputFrame;
    
            // Process function.
            auto processRGB8 = [](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i)
            {
                memcpy(outputRow, inputRow, std::min(inputFrame.byteSpan, outputFrame.byteSpan));
            };
            
            res = processFrameToFrameRow(processRGB8, inputFrameEx, outputFrameEx);
        }
    }
    
    return res;
}

// @return number of parameters.
index_t ROICopy::parameterNumber()
{
    return 2;
}

// @return parameter info.
const ParameterInfo& ROICopy::parameterInfo(index_t index)
{
    static ParameterInfo emptyParam;
    if (index == 0)
    {
        static ROIParameterInfo roiSrc("Source", {0, 0, 1, 1}, {0, 0, 0, 0}, {1920, 1920, 1920, 1920});
        
        return roiSrc;
    }
    else if (index == 1)
    {
        static ROIParameterInfo roiDst("Dest", {0, 0, 1, 1}, {0, 0, 0, 0}, {1920, 1920, 1920, 1920});
        
        return roiDst;
    }
    return emptyParam;
}

// @return name. Latin only letters.
const char* const ROICopy::name()
{
    return "ROI Copy";
}
