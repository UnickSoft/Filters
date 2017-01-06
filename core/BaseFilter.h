//
//  BaseFilter.hpp
//  UiTest
//
//  Created by Олег on 20.12.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#pragma once


#include "IFilter.h"
#include "IPrivateFilterList.h"
#include "IResourceManager.h"
#include <string>


class BaseFilter : public IFilter
{
public:

    BaseFilter (const IPrivateFilterList& filterList, IResourceManager& resourceManager, const std::string& name);
    
    // Apply filter to frame.
    virtual bool apply(const Frame& inputFrame, Frame& outputFrame, const IParameterSet& params) = 0;
    
    // @return output frame params for input frame.
    virtual FrameParams outputFrameParams(const FrameParams& inputFrame) = 0;
    
    // @return name. Latin only letters.
    const char* const name() override;
    
    // @return number of input frames.
    index_t inputsNumber() override {return 1;};
    
    // @return number of output frames.
    index_t outputsNumber() override {return 1;};
    
    static bool apply(IFilter* filter, const Frame& inputFrame, Frame& outputFrame, const IParameterSet& params);
    
protected:

    // Apply filter to frame.
    bool apply(const Frame* inputFrames, index_t inputFramesNumber, Frame* outputFrames, index_t outputFramesNumber, const IParameterSet& params) final;
    
    // @return output frames params for input frames.
    // If input frame format is unsupported, out frame format will be unsupported.
    // Input frames and output frames should be inputsNumber/outputsNumber.
    bool outputFrameParams(const FrameParams* inputFrames, FrameParams* outputFrames) final;

    IResourceManager& resourceManager;
    const IPrivateFilterList& filterList;
    const std::string name_;

};
