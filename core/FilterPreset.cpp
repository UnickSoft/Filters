//
//  FilterPreset.cpp
//  UiTest
//
//  Created by Олег on 29.01.17.
//  Copyright © 2017 Oleg. All rights reserved.
//

#include "FilterPreset.h"
#include "json.hpp"
#include "ParameterHelpers.h"
#include <fstream>

using json = nlohmann::json;

FilterPreset::FilterPreset(const IParameterSet& params, FilterPtr filter, const std::string& presetName)
{
    for (int i = 0; i < params.parametersNumber(); i ++)
    {
        _params.push_back({(BaseParameters)filter->parameterInfo(i).type, params.value(i)});
    }
    _filterName = filter->name();
    _presetName = presetName;
}

FilterPreset::FilterPreset(const std::string& filename)
{
    load(filename);
}

const Parameter& FilterPreset::value(index_t index) const
{
    return std::get<1>(_params.at(index));
}
    
index_t FilterPreset::parametersNumber() const
{
    return _params.size();
}

bool FilterPreset::save(const std::string& filename)
{
    json j;
    std::for_each(_params.begin(), _params.end(), [&j](const ParamAndType& parameterAndType)
    {
        json node = saveParameter(parameterAndType.first, &parameterAndType.second);
        
        j["params"].push_back(node);
    });
    
    j["filterName"] = _filterName;
    j["presetName"] = _presetName;
    
    std::ofstream out(filename);
    if (out.is_open())
    {
        out << std::setw(4) << j << std::endl;
    }
    
    return out.is_open();
}

bool FilterPreset::load(const std::string& filename)
{
    _filterName.clear();
    _presetName.clear();
    _params.clear();
    
    std::ifstream inFile(filename);
    if (inFile.is_open())
    {
        json j;
        inFile >> j;
        _filterName = j["filterName"];
        _presetName = j["presetName"];

        auto params = j["params"];
        if (params.is_array())
        {
            std::for_each(params.begin(), params.end(), [this](const json& param)
                          {
                              ParamAndType current;
                              current.first  = (BaseParameters)param["type"];
                              Parameter filterParameter;
                              current.second = loadParameter(current.first, param);
                              
                              _params.push_back(current);
                          });

        }
    }
    
    return inFile.is_open();
}

const std::string& FilterPreset::name()
{
    return _presetName;
}

const std::string FilterPreset::filterName()
{
    return _filterName;
}
