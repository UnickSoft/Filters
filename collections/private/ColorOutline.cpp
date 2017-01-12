//
//  ColorOutline.cpp
//  UiTest
//
//  Created by Олег on 17.12.16.
//  Copyright © 2016 Oleg. All rights reserved.
//


#include "ColorOutline.h"

ColorOutline::ColorOutline(const IPrivateFilterList& filterList, IResourceManager& resourceManager) : FilterGraph(filterList, resourceManager, "Color Outline")
{}

    // Init filter graph.
bool ColorOutline::init()
{
    auto dilate = FilterPtr(filterList.createFilter("Dilate", filterList, resourceManager));
    auto solidColor1 = FilterPtr(filterList.createFilter("Solid Color", filterList, resourceManager));
    auto solidColor2 = FilterPtr(filterList.createFilter("Solid Color", filterList, resourceManager));
    auto blur = FilterPtr(filterList.createFilter("Blur", filterList, resourceManager));
    auto splitter = FilterPtr(filterList.createFilter("Splitter", filterList, resourceManager));
    auto blender = FilterPtr(filterList.createFilter("Alpha Blend", filterList, resourceManager));
    
    auto dilateNode      = addFilter(dilate);
    auto solidColorNode1 = addFilter(solidColor1);
    auto solidColorNode2 = addFilter(solidColor2);
    auto blurNode        = addFilter(blur);
    auto splitterNode    = addFilter(splitter);
    auto blenderNode     = addFilter(blender);
    
    
    //input -> splitter -> blur -> dilate -> solid color -> blender
    //            |------------------------> solid color ->
    
    root()->addOutput(splitterNode);
    splitterNode->addOutput(dilateNode);
    dilateNode->addOutput(blurNode);
    blurNode->addOutput(solidColorNode1);
    splitterNode->addOutput(solidColorNode2);
    solidColorNode1->addOutput(blenderNode);
    solidColorNode2->addOutput(blenderNode);
    
    return true;
}
