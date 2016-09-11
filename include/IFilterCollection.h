//
//  IFilterCollection.h
//  test
//
//  Created by Олег on 11.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef IFilterCollection_h
#define IFilterCollection_h

#include "IResourceManager.h"
#include "IFilter.h"


class IFilterCollection : public VirtualDestructor
{
public:
    
    // Create filter by index.
    virtual IFilter* createFilter(index_t) = 0;
    
    // Create filter by name.
    virtual IFilter* createFilter(const char* const name) = 0;
    
    // @return number of filters in list.
    virtual index_t filtersNumber() = 0;
    
    virtual void setResourceManager(IResourceManager*) = 0;

};


#endif /* IFilterCollection_h */
