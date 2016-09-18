//
//  TestCollection.cpp
//  test
//
//  Created by Олег on 11.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "TestCollection.h"
#include "Blur.h"
#include "Copy.h"

// Create this collection.
IFilterCollection* createCollection()
{
    return new TestCollection();
}

TestCollection::TestCollection ()
{
    addFilter<Copy>();
    addFilter<Blur>();
}

// Create filter by index.
IFilter* TestCollection::createFilter(index_t index)
{
    return publicFilters.createFilter(index, &privateFilters, nullptr);
}
    
// Create filter by name.
IFilter* TestCollection::createFilter(const char* const name)
{
    return publicFilters.createFilter(name, &privateFilters, nullptr);
}

// @return number of filters in list.
index_t TestCollection::filtersNumber()
{
    return publicFilters.filtersNumber();
}


template <class T> void TestCollection::addFilter()
{
        privateFilters.addFilter ([](const IPrivateFilterList* privateFilterList, IResourceManager* resourceManager)
    {
        return new T(privateFilterList, resourceManager);
    });
    
    publicFilters.addFilter ([](const IPrivateFilterList* privateFilterList, IResourceManager* resourceManager)
    {
        return new T(privateFilterList, resourceManager);
    });
}

