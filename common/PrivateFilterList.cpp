//
//  PrivateFilterList.cpp
//  test
//
//  Created by Олег on 11.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "PrivateFilterList.h"


// Add factory function for filter.
void PrivateFilterList::addFilter(const FilterFactory& factory)
{
    std::unique_ptr<IFilter> filter = std::unique_ptr<IFilter>(factory(nullptr, nullptr));
    if (filter)
    {
        factoryList.push_back(factory);
        nameToIndex[filter->name()] = factoryList.size() - 1;
    }
}

// Create filter by index.
IFilter* PrivateFilterList::createFilter(index_t index, const IPrivateFilterList* privateFilterList, IResourceManager* resourceManager) const
{
    if (index < factoryList.size())
    {
        return factoryList.at(index)(privateFilterList, resourceManager);
    }
    
    return nullptr;
}

// Create filter by name.
IFilter* PrivateFilterList::createFilter(const char* const name, const IPrivateFilterList* privateFilterList, IResourceManager* resourceManager) const
{
    if (nameToIndex.count(name) > 0)
    {
        return createFilter(nameToIndex.at(name), privateFilterList, resourceManager);
    }
    
    return nullptr;
}

// @return number of filters in list.
index_t PrivateFilterList::filtersNumber() const
{
    return factoryList.size();
}