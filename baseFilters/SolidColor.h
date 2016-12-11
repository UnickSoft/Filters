//
//  SolidColor.hpp
//  UiTest
//
//  Created by Олег on 19.11.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "IFilter.h"
#include "IPrivateFilterList.h"
#include "IResourceManager.h"

class SolidColor : public IFilter
{
public:

    SolidColor (const IPrivateFilterList& filterList, IResourceManager& resourceManager);
    
    // Apply filter to frame.
    bool apply(const Frame& inputFrame, Frame& outputFrame, const IParameterSet& params) override;
    
    // @return number of parameters.
    index_t parameterNumber() override;
    
    // @return parameter info.
    const ParameterInfo& parameterInfo(index_t) override;
    
    // @return name. Latin only letters.
    const char* const name() override;
    
    // @return output frame params for input frame.
    FrameParams outputFrameParams(const FrameParams& inputFrame) override;
    
protected:


    IResourceManager& resourceManager;
    const IPrivateFilterList& filterList;

};
