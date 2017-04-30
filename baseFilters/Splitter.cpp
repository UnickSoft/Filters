//
//  Splitter.cpp
//  UiTest
//
//  Created by Олег on 02.01.17.
//  Copyright © 2017 Oleg. All rights reserved.
//

#include "Splitter.h"
#include "ParameterHelpers.h"
#include "FilterTemplates.h"
#include <memory>
#include "BaseParameterSet.h"

Splitter::Splitter (const IPrivateFilterList& filterList, IResourceManager& resourceManager)
: resourceManager(resourceManager), filterList(filterList), outputsNumber_(2) {}

bool Splitter::apply(const Frame* inputFrames, index_t inputFramesNumber, Frame* outputFrames, index_t outputFramesNumber, const IParameterSet& params)
{
    if (inputFramesNumber == 1 && outputFramesNumber == outputsNumber_)
    {
        auto copyFilter = std::unique_ptr<IFilter>(filterList.createFilter("Copy", filterList, resourceManager));
        
        bool res = true;
        
        for (index_t i = 0; i < outputsNumber_; i ++)
        {
            res = res && copyFilter->apply(inputFrames, 1, &outputFrames[i], 1, params);
        }
        
        return res;
    }
    
    return false;
}

index_t Splitter::inputsNumber()
{
    return 1;
}

index_t Splitter::outputsNumber()
{
    return outputsNumber_;
}

index_t Splitter::parameterNumber()
{
    return 0;
}

const ParameterInfo& Splitter::parameterInfo(index_t)
{
    static ParameterInfo paramInfo;
    return paramInfo;
}

const char* const Splitter::name()
{
    return "Splitter";
}

bool Splitter::outputFrameParams(const FrameParams* inputFrames, FrameParams* outputFrames)
{
    for (index_t i = 0; i < outputsNumber_; i ++)
    {
        outputFrames[i] = inputFrames[0];
    }

    return true;
}

void Splitter::setOutputsNumber(index_t number)
{
    outputsNumber_ = number;
}

ROI Splitter::outputRoi(const ROI& inputRoi, const IParameterSet& params)
{
    return inputRoi;
}
