//
//  ColorOutline.hpp
//  UiTest
//
//  Created by Олег on 17.12.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#pragma once

#include "FilterGraph.h"
#include "IFilter.h"
#include "IResourceManager.h"

class TestFilterGraph : public FilterGraph
{
public:

    TestFilterGraph(const IPrivateFilterList& filterList, IResourceManager& resourceManager);
    
    // @return parameter info.
    const ParameterInfo& parameterInfo(index_t) override;

protected:


    // Init filter graph.
    bool init() override;
    
    
    FilterGraph::FilterNodePtr bluredColorNode1;
    FilterGraph::FilterNodePtr baseColorNode2;

    
};
