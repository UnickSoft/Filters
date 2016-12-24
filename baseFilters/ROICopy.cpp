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


ROICopy::ROICopy (const IPrivateFilterList& filterList, IResourceManager& resourceManager) : BaseFilter(filterList, resourceManager, "ROI Copy")
{}

// Apply filter to frame.
bool ROICopy::apply(const Frame& inputFrame, Frame& outputFrame, const IParameterSet& params)
{
    const ROI srcROI = ROIParameter::field(&params.value(0));

    const ROI dstROI = ROIParameter::field(&params.value(1));
    
    bool res = false;
    
    // TODO out of border
    if (srcROI.width  == dstROI.width &&
        srcROI.height == dstROI.height &&
        srcROI.width > 0 && srcROI.height > 0 &&
        dstROI.width > 0 && dstROI.height > 0 &&
        inputFrame.format == outputFrame.format)
    {
        FrameEx inputFrameEx  = inputFrame;
        FrameEx outputFrameEx = outputFrame;
        auto pixelDepth = inputFrameEx.pixelDepth();
        
        // Process function.
        auto processRGB8 = [=](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i)
        {
            memcpy(outputRow, inputRow, srcROI.width * pixelDepth);
        };
        
        res = processFrameToFrameRow(processRGB8, inputFrameEx, outputFrameEx, &srcROI, &dstROI);
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

// @return output frame params for input frame.
FrameParams ROICopy::outputFrameParams(const FrameParams& inputFrame)
{
    return inputFrame;
}
