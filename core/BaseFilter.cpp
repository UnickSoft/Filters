//
//  BaseFilter.cpp
//  UiTest
//
//  Created by Олег on 20.12.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "BaseFilter.h"
#include "ParameterHelpers.h"
#include "FilterTemplates.h"
#include <memory>
#include "BaseParameterSet.h"



BaseFilter::BaseFilter (const IPrivateFilterList& filterList, IResourceManager& resourceManager, const std::string& name) : name_(name), filterList(filterList),
    resourceManager(resourceManager) {}

const char* const BaseFilter::name()
{
    return name_.c_str();
}

bool BaseFilter::apply(IFilter* filter, const Frame& inputFrame, Frame& outputFrame, const IParameterSet& params)
{
    return  filter->apply(&inputFrame, 1, &outputFrame, 1, params);
}

bool BaseFilter::apply(const Frame* inputFrames, index_t inputFramesNumber, Frame* outputFrames, index_t outputFramesNumber, const IParameterSet& params)
{
    return (inputFramesNumber == inputsNumber() && outputFramesNumber == outputsNumber()
        ? apply(*inputFrames, *outputFrames, params)
        : false);
}
    
bool BaseFilter::outputFrameParams(const FrameParams* inputFrames, FrameParams* outputFrames)
{
    *outputFrames = outputFrameParams(*inputFrames);
    return outputFrames->format != FrameParams::Unsupported;
}

FilterPtr BaseFilter::createFilter(const char* const name)
{
    return  FilterPtr(filterList.createFilter(name, filterList, resourceManager));
}
