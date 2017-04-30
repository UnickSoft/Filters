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

FilterPreset::FilterPreset(bool paramOwner) : BaseParameterSet(paramOwner) {}

FilterPreset::FilterPreset(const IParameterSet& params, FilterPtr filter, const std::string& presetName)
{
    for (int i = 0; i < params.parametersNumber(); i ++)
    {
        push_back((BaseParameters)filter->parameterInfo(i).type, params.value(i));
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
    return at(index);
}
    
index_t FilterPreset::parametersNumber() const
{
    return size();
}

bool FilterPreset::save(const std::string& filename)
{
    json j;
    
    saveJson(j);
    
    std::ofstream out(filename);
    if (out.is_open())
    {
        out << std::setw(4) << j << std::endl;
        _filename = filename;
    }
    
    return out.is_open();
}

bool FilterPreset::load(const std::string& filename)
{
    bool res = false;
    _filterName.clear();
    _presetName.clear();
    clear();
    
    std::ifstream inFile(filename);
    if (inFile.is_open())
    {
        res = true;
        try
        {
            json j;
            inFile >> j;
            res = loadJson(j);
            if (res)
            {
                _filename = filename;
            }
        }
        catch (...)
        {
            assert(!"Cannot read preset.");
            _filterName.clear();
            _presetName.clear();
            clear();
        }
    }
    
    return res;
}

const std::string& FilterPreset::name()
{
    return _presetName;
}

const std::string& FilterPreset::filterName()
{
    return _filterName;
}

bool FilterPreset::isValid()
{
    return !_filterName.empty();
}

// @return filter name.
const std::string& FilterPreset::fileName()
{
    return _filename;
}

bool FilterPreset::saveJson(json& j)
{
    std::for_each(data.begin(), data.end(), [&j](const ParamAndType& parameterAndType)
    {
        json node = saveParameter(parameterAndType.first, &parameterAndType.second);
        
        j["params"].push_back(node);
    });
    
    j["filterName"] = _filterName;
    j["presetName"] = _presetName;
    
    return true;
}

// Load preset to file.
bool FilterPreset::loadJson(json& j)
{
    bool res = false;
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
                          
                          push_back(current.first, current.second);
                      });
        res = true;
    }
    
    return res;
}

