//
//  IFilterList.h
//  test
//
//  Created by Олег on 11.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef IFilterList_h
#define IFilterList_h

#include "Base.h"
#include "IFilter.h"
#include "IResourceManager.h"
#include <functional>

class IPrivateFilterList : public VirtualDestructor
{
public:

    // Create filter by index.
    virtual IFilter* createFilter(index_t, const IPrivateFilterList*, IResourceManager* ) const = 0;
    
    // Create filter by name.
    virtual IFilter* createFilter(const char* const name, const IPrivateFilterList*, IResourceManager* ) const = 0;
    
    // @return number of filters in list.
    virtual index_t filtersNumber() const = 0;
};

typedef std::function<IFilter*(const IPrivateFilterList*, IResourceManager*)> FilterFactory;



#endif /* IFilterList_h */
