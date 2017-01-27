//
//  AlphaConvert.hpp
//  UiTest
//
//  Created by Олег on 26.01.17.
//  Copyright © 2017 Oleg. All rights reserved.
//

#ifndef AlphaConvert_hpp
#define AlphaConvert_hpp

#include "IFilter.h"
#include "IPrivateFilterList.h"
#include "IResourceManager.h"
#include "BaseFilter.h"

class AlphaConvert : public BaseFilter
{
public:

    AlphaConvert (const IPrivateFilterList& filterList, IResourceManager& resourceManager);
    
    // Apply filter to frame.
    bool apply(const Frame& inputFrame, Frame& outputFrame, const IParameterSet& params) override;
    
    // @return number of parameters.
    index_t parameterNumber() override;
    
    // @return parameter info.
    const ParameterInfo& parameterInfo(index_t) override;
    
    // @return output frame params for input frame.
    FrameParams outputFrameParams(const FrameParams& inputFrame) override;
};


#endif /* AlphaConvert_hpp */
