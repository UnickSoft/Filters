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
#include "ROICopy.h"
#include "Dilate.h"
#include "SolidColor.h"
#include "Splitter.h"
#include "AlphaBlend.h"
#include "ColorOutline.h"

// Create this collection.
IFilterCollection* createCollection()
{
    return new TestCollection();
}

TestCollection::TestCollection () : resourceManager(nullptr)
{
    addFilter<Copy>();
    addFilter<Blur>();
    addFilter<ROICopy>();
    addFilter<Dilate>();
    addFilter<SolidColor>();
    addFilter<Splitter>();
    addFilter<AlphaBlend>();
    addFilter<ColorOutline>();
}

// Create filter by index.
IFilter* TestCollection::createFilter(index_t index)
{
    return publicFilters.createFilter(index, privateFilters, *resourceManager);
}
    
// Create filter by name.
IFilter* TestCollection::createFilter(const char* const name)
{
    return publicFilters.createFilter(name, privateFilters, *resourceManager);
}

// @return number of filters in list.
index_t TestCollection::filtersNumber()
{
    return publicFilters.filtersNumber();
}

void TestCollection::setResourceManager(IResourceManager* resourceManager)
{
    this->resourceManager = resourceManager;
}


template <class T> void TestCollection::addFilter()
{
        privateFilters.addFilter ([](const IPrivateFilterList& privateFilterList, IResourceManager& resourceManager)
    {
        return new T(privateFilterList, resourceManager);
    });
    
    publicFilters.addFilter ([](const IPrivateFilterList& privateFilterList, IResourceManager& resourceManager)
    {
        return new T(privateFilterList, resourceManager);
    });
}

