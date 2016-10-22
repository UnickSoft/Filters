//
//  IParameterSet.h
//  test
//
//  Created by Олег on 11.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef IParameterSet_h
#define IParameterSet_h

#include "Base.h"

class IParameterSet : public VirtualDestructor
{
public:

    // @return parameter from set.
    virtual const Parameter& value(index_t index) = 0;
    
    // @return parameters number.
    virtual index_t parametersNumber() = 0;
};


#endif /* IParameterSet_h */
