//
//  TestCollection.hpp
//  test
//
//  Created by Олег on 11.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef TestCollection_hpp
#define TestCollection_hpp

#include <stdio.h>
#include "IFilterCollection.h"
#include "PrivateFilterList.h"

class TestCollection : public IFilterCollection
{
public:

    TestCollection ();

    // Empty for now.
    virtual void setResourceManager(IResourceManager*) {;}
    
    // Create filter by index.
    virtual IFilter* createFilter(index_t);
    
    // Create filter by name.
    virtual IFilter* createFilter(const char* const name);
    
    // @return number of filters in list.
    virtual index_t filtersNumber();
    
public:
    PrivateFilterList privateFilters;
    PrivateFilterList publicFilters;
    
};

#endif /* TestCollection_hpp */
