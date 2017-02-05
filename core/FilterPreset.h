//
//  FilterPreset.hpp
//  UiTest
//
//  Created by Олег on 29.01.17.
//  Copyright © 2017 Oleg. All rights reserved.
//

#ifndef FilterPreset_hpp
#define FilterPreset_hpp

#include <stdio.h>
#include "IFilter.h"
#include "IResourceManager.h"
#include "IFilterCollection.h"
#include <string>
#include <vector>
#include <utility>
#include "BaseParameters.h"

class FilterPreset : public IParameterSet
{
public:
    // Create from param set.
    FilterPreset(const IParameterSet& params, FilterPtr filter, const std::string& presetName);
    
    // Load from file.
    FilterPreset(const std::string& filename);
    
    // @return parameter from set.
    const Parameter& value(index_t index) const;
    
    // @return parameters number.
    index_t parametersNumber() const;

    // Save preset to file.
    bool save(const std::string& filename);

    // Load preset from file.
    bool load(const std::string& filename);
    
    // @return preset name.
    const std::string& name();
    
    // @return filter name.
    const std::string filterName();
    
private:

    std::string _filterName;
    std::string _presetName;
    using ParamAndType = std::pair<BaseParameters, Parameter>;
    std::vector<ParamAndType> _params;
};

#endif /* FilterPreset_hpp */
