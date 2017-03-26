//
//  LUTGenerator.cpp
//  UiTest
//
//  Created by Олег on 26.03.17.
//  Copyright © 2017 Oleg. All rights reserved.
//

#include <iostream>
#include "LUTGenerator.h"
#include "ParameterHelpers.h"
#include "FilterTemplates.h"
#include <memory>
#include "BaseParameterSet.h"
#include <unordered_map>


LUTGenerator::LUTGenerator (const IPrivateFilterList& filterList, IResourceManager& resourceManager) : BaseFilter(filterList, resourceManager, "LUT Generator")
{}

// Apply filter to frame.
bool LUTGenerator::apply(const Frame& inputFrame, Frame& outputFrame, const IParameterSet& params)
{
    if (inputFrame.format == FrameParams::RGBA8 && outputFrame.format == FrameParams::RGBA8)
    {
        std::unordered_map<size_t, Color> table[256] = {};
        
        auto processFillRGBA8 = [&table](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i, int j)
        {
            int brithness = ((uint)inputRow[0] * 299 + (uint)inputRow[1] * 587 + (uint)inputRow[2] * 114) / ((587 + 299 + 114));
            
            int brithnessClamped = brithness > 255 ? 255 : brithness;
            
            Color color = {inputRow[0], inputRow[1], inputRow[2]};
            
            auto& t = table[brithnessClamped];
            t[t.size()] = color;
            
            /*
            if (table[brithnessClamped].count(color) <= 0)
            {
                table[brithnessClamped][color] = 1;
            }
            else
            {
                ++table[brithnessClamped][color];
            }
            */
        };
        
        auto processApplyRGBA8 = [&table](FrameEx& inputFrame, FrameEx& outputFrame, uint8_t* inputRow, uint8_t* outputRow, int i, int j)
        {
            if (j < 256)
            {
                auto& map = table[j];
                if (i < map.size())
                {
                    Color color = map[i];
                    
                    outputRow[0] = color.red;
                    outputRow[1] = color.green;
                    outputRow[2] = color.blue;
                    outputRow[3] = 255;
                }
            }
        };

        
        FrameEx inputFrameEx  = inputFrame;
        FrameEx outputFrameEx = outputFrame;
        
        bool res = processFrameToFramePixel(processFillRGBA8, inputFrameEx, outputFrameEx);
        
        if (res)
        {
            res = processFrameToFramePixel(processApplyRGBA8, inputFrameEx, outputFrameEx);
        }
        
        return res;
    }
    
    return false;
}

// @return number of parameters.
index_t LUTGenerator::parameterNumber()
{
    return 0;
}

// @return parameter info.
const ParameterInfo& LUTGenerator::parameterInfo(index_t index)
{
    static ParameterInfo emptyParam;
    return emptyParam;
}

// @return output frame params for input frame.
FrameParams LUTGenerator::outputFrameParams(const FrameParams& inputFrame)
{
    FrameParams res = {0, 0, FrameParams::Unsupported};
    if (inputFrame.format == FrameParams::RGBA8)
    {
        res = inputFrame;
        res.format = FrameParams::RGBA8;
    }
    return res;
}
