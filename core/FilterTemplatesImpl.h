//
//  FilterTemplates.cpp
//  UiTest
//
//  Created by Олег on 02.11.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

template <typename T> bool processFrameToFrameRow(T function, FrameEx& inputFrame, FrameEx& outputFrame, const ROI* inputROI, const ROI* outputROI)
{
    bool res = false;
    
    ROI input  = inputROI  ? *inputROI : ROI{0, 0, inputFrame.width, inputFrame.height};
    ROI output = outputROI ? *outputROI : ROI{0, 0, inputFrame.width, inputFrame.height};
    
    if (input.width  == output.width &&
        input.height == output.height)
    {
        res = true;
        
        auto inputDataStart  = inputFrame.dataPtr(input.x, input.y);
        auto outputDataStart = outputFrame.dataPtr(output.x, output.y);
        
        for (int i = 0; i < input.height; i++)
        {
            auto inputRow  = inputDataStart  + inputFrame.byteSpan  * i;
            auto outputRow = outputDataStart + outputFrame.byteSpan * i;
            
            function(inputFrame, outputFrame, inputRow, outputRow, i);
        }
    }
    
    return res;
}


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
        
        auto inputDataStart  = inputFrame.dataPtr(input.x, input.y);
        auto outputDataStart = outputFrame.dataPtr(output.x, output.y);
        
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

template <typename T> bool process2FramesToFramePixel(T function, FrameEx& inputFrame1, FrameEx& inputFrame2, FrameEx& outputFrame, const ROI* inputROI1, const ROI* inputROI2,const ROI* outputROI)
{
    bool res = false;
    
    ROI input1  = inputROI1  ? *inputROI1 : ROI{0, 0, inputFrame1.width, inputFrame1.height};
    ROI input2  = inputROI2  ? *inputROI2 : ROI{0, 0, inputFrame2.width, inputFrame2.height};
    ROI output = outputROI ? *outputROI : ROI{0, 0, outputFrame.width, outputFrame.height};
    
    if (input1.width  == output.width &&
        input1.height == output.height &&
        input2.width  == output.width &&
        input2.height == output.height)
    {
        res = true;
        
        auto inputPixelSize1  = inputFrame1.pixelDepth();
        auto inputPixelSize2  = inputFrame2.pixelDepth();
        auto outputPixelSize = outputFrame.pixelDepth();
        
        auto inputDataStart1  = inputFrame1.dataPtr(input1.x, input1.y);
        auto inputDataStart2  = inputFrame2.dataPtr(input2.x, input2.y);
        auto outputDataStart  = outputFrame.dataPtr(output.x, output.y);
        
        for (int i = 0; i < input1.height; i++)
        {
            auto inputRow1  = inputDataStart1  + inputFrame1.byteSpan  * i;
            auto inputRow2  = inputDataStart2  + inputFrame2.byteSpan  * i;
            auto outputRow = outputDataStart + outputFrame.byteSpan * i;
            
            for (int j = 0; j < input1.width; j++)
            {
                function(inputFrame1, inputFrame2, outputFrame, inputRow1, inputRow2, outputRow, i, j);
                
                inputRow1   += inputPixelSize1;
                inputRow2   += inputPixelSize2;
                outputRow   += outputPixelSize;
            }
        }
    }
    
    return res;
}