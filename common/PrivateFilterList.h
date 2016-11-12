//
//  PrivateFilterList.hpp
//  test
//
//  Created by Олег on 11.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef PrivateFilterList_hpp
#define PrivateFilterList_hpp

#include <stdio.h>
#include "IPrivateFilterList.h"
#include <vector>
#include <unordered_map>
#include <string>

class PrivateFilterList : public IPrivateFilterList
{
public:

    // Add factory function for filter.
    void addFilter(const FilterFactory& factory);
    
    // Create filter by index.
    virtual IFilter* createFilter(index_t, const IPrivateFilterList*, IResourceManager* ) const override;
    
    // Create filter by name.
    virtual IFilter* createFilter(const char* const name, const IPrivateFilterList*, IResourceManager* ) const override;
    
    // @return number of filters in list.
    virtual index_t filtersNumber() const override;
    
private:

    // Factory list.
    std::vector<FilterFactory> factoryList;
    // Filter name to index.
    std::unordered_map<std::string, index_t> nameToIndex;
    
};

#endif /* PrivateFilterList_hpp */
