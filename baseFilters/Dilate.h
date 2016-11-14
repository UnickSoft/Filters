//
//  Dilate.hpp
//  UiTest
//
//  Created by Олег on 12.11.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#pragma once

#include <stdio.h>
#include "IFilter.h"
#include "IPrivateFilterList.h"
#include "IResourceManager.h"


class Dilate : public IFilter
{
public:

    Dilate (const IPrivateFilterList* filterList, IResourceManager* resourceManager);
    
    // Apply filter to frame.
    bool apply(const Frame* inputFrame, Frame* outputFrame, const IParameterSet* params) override;
    
    // @return number of parameters.
    index_t parameterNumber() override;
    
    // @return parameter info.
    const ParameterInfo& parameterInfo(index_t) override;
    
    // @return name. Latin only letters.
    const char* const name() override;
    
protected:


    IResourceManager& resourceManager;
    const IPrivateFilterList& filterList;

};
