//
//  TestApplicationBridge.hpp
//  test
//
//  Created by Олег on 11.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef TestApplicationBridge_hpp
#define TestApplicationBridge_hpp

#include <stdio.h>
#include "Base.h"
#include "IFilter.h"
#include <memory>
#include "IFilterCollection.h"

typedef std::shared_ptr<IFilter> FilterPtr;


class TestApplicationBridge
{
public:

    TestApplicationBridge();
    ~TestApplicationBridge();
    
    index_t filtersNumber();
    FilterPtr createFilter(index_t index);
    
    void setResourceManager(IResourceManager*);
    
private:

    std::unique_ptr<IFilterCollection> filterCollection;
    
};


#endif /* TestApplicationBridge_hpp */
