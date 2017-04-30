//
//  Splitter.hpp
//  UiTest
//
//  Created by Олег on 02.01.17.
//  Copyright © 2017 Oleg. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "IFilter.h"
#include "IPrivateFilterList.h"
#include "IResourceManager.h"
#include "BaseFilter.h"


class Splitter : public IFilter
{
public:

    Splitter (const IPrivateFilterList& filterList, IResourceManager& resourceManager);
    
    // Apply filter to frame.
    virtual bool apply(const Frame* inputFrames, index_t inputFramesNumber, Frame* outputFrames, index_t outputFramesNumber, const IParameterSet& params) override;
    
    // @return number of input frames.
    virtual index_t inputsNumber() override;
    
    // @return number of output frames.
    virtual index_t outputsNumber() override;
    
    // @return number of parameters.
    virtual index_t parameterNumber() override;
    
    // @return parameter info.
    virtual const ParameterInfo& parameterInfo(index_t) override;
    
    // @return name. Latin only letters.
    virtual const char* const name() override;
    
    // @return output frames params for input frames.
    // If input frame format is unsupported, out frame format will be unsupported.
    // Input frames and output frames should be inputsNumber/outputsNumber.
    virtual bool outputFrameParams(const FrameParams* inputFrames, FrameParams* outputFrames) override;
    
    virtual ROI outputRoi(const ROI& inputRoi, const IParameterSet& params) override;
    
    void setOutputsNumber(index_t number);
    
private:

    IResourceManager& resourceManager;
    const IPrivateFilterList& filterList;
    index_t outputsNumber_;
    
};

