//
//  Copy.hpp
//  test
//
//  Created by Олег on 18.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef Copy_hpp
#define Copy_hpp

#include <stdio.h>
#include "IFilter.h"
#include "IPrivateFilterList.h"
#include "IResourceManager.h"


class Copy : public IFilter
{
public:

    Copy (const IPrivateFilterList* filterList, IResourceManager* resourceManager);
    
    // Apply filter to frame.
    bool apply(const Frame* inputFrame, Frame* outputFrame, IParameterSet* params) override;
    
    // @return number of parameters.
    index_t parameterNumber() override;
    
    // @return parameter info.
    ParameterInfo parameterInfo(index_t) override;
    
    // @return name. Latin only letters.
    const char* const name() override;


};

#endif /* Copy_hpp */
