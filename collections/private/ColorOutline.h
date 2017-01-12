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

class ColorOutline : public FilterGraph
{
public:

    ColorOutline(const IPrivateFilterList& filterList, IResourceManager& resourceManager);

protected:

    // Init filter graph.
    bool init() override;
    
};
