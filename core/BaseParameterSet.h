//
//  BaseParameterSet.hpp
//  UiTest
//
//  Created by Олег on 22.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef BaseParameterSet_hpp
#define BaseParameterSet_hpp

#include <vector>
#include "IParameterSet.h"
#include "BaseParameters.h"

class BaseParameterSet : public std::vector<Parameter>, public IParameterSet
{
public:

    BaseParameterSet() = default;
    BaseParameterSet(const IParameterSet& paramSet);
    
    // @return parameter from set.
    virtual const Parameter& value(index_t index) const override;
    
    // @return parameters number.
    virtual index_t parametersNumber() const override;
};

#endif /* BaseParameterSet_hpp */
