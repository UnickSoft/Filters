//
//  LUTGenerator.hpp
//  UiTest
//
//  Created by Олег on 26.03.17.
//  Copyright © 2017 Oleg. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "IFilter.h"
#include "IPrivateFilterList.h"
#include "IResourceManager.h"
#include "BaseFilter.h"

class LUTGenerator : public BaseFilter
{
public:

    LUTGenerator (const IPrivateFilterList& filterList, IResourceManager& resourceManager);
    
    // Apply filter to frame.
    bool apply(const Frame& inputFrame, Frame& outputFrame, const IParameterSet& params) override;
    
    // @return number of parameters.
    index_t parameterNumber() override;
    
    // @return parameter info.
    const ParameterInfo& parameterInfo(index_t) override;
    
    // @return output frame params for input frame.
    FrameParams outputFrameParams(const FrameParams& inputFrame) override;
};
