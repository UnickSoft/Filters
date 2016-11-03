//
//  FilterTemplates.cpp
//  UiTest
//
//  Created by Олег on 02.11.16.
//  Copyright © 2016 Oleg. All rights reserved.
//


template <typename T> bool processFrameToFramePixel(T function, FrameEx& inputFrame, FrameEx& outputFrame, const ROI* inputROI, const ROI* outputROI)
{
    bool res = false;
    
    ROI input  = inputROI  ? *inputROI : ROI{0, 0, inputFrame.width, inputFrame.height};
    ROI output = outputROI ? *outputROI : ROI{0, 0, inputFrame.width, inputFrame.height};
    
    if (input.width  == output.width &&
        input.height == output.height)
    {
        res = true;
        
        auto inputPixelSize  = inputFrame.pixelDepth();
        auto outputPixelSize = outputFrame.pixelDepth();
        
        auto inputDataStart  = inputFrame.data  + input.y  * inputFrame.byteSpan  + inputPixelSize * input.x;
        auto outputDataStart = outputFrame.data + output.y * outputFrame.byteSpan + outputPixelSize * output.x;
        
        for (int i = 0; i < input.height; i++)
        {
            auto inputRow  = inputDataStart  + inputFrame.byteSpan  * i;
            auto outputRow = outputDataStart + outputFrame.byteSpan * i;
            
            for (int j = 0; j < input.width; j++)
            {
                function(inputFrame, outputFrame, inputRow, outputRow, i, j);
                
                inputRow   += inputPixelSize;
                outputRow  += outputPixelSize;
            }
        }
    }
    
    return res;
}