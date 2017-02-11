//
//  ColorOutline.cpp
//  UiTest
//
//  Created by Олег on 17.12.16.
//  Copyright © 2016 Oleg. All rights reserved.
//


#include "TestFilterGraph.h"
#include "ParameterHelpers.h"

TestFilterGraph::TestFilterGraph(const IPrivateFilterList& filterList, IResourceManager& resourceManager) : FilterGraph(filterList, resourceManager, "Test Filter Graph")
{}

    // Init filter graph.
bool TestFilterGraph::init()
{
    auto solidColor1 = FilterPtr(filterList.createFilter("Solid Color", filterList, resourceManager));
    auto solidColor2 = FilterPtr(filterList.createFilter("Solid Color", filterList, resourceManager));
    auto blur = FilterPtr(filterList.createFilter("Blur", filterList, resourceManager));
    auto splitter = FilterPtr(filterList.createFilter("Splitter", filterList, resourceManager));
    auto blender = FilterPtr(filterList.createFilter("Alpha Blend", filterList, resourceManager));
    
    bluredColorNode1 = addFilter(solidColor1);
    baseColorNode2 = addFilter(solidColor2);
    auto blurNode        = addFilter(blur);
    auto splitterNode    = addFilter(splitter);
    auto blenderNode     = addFilter(blender);
    
    
    //input -> splitter -> blur -> solid color -> blender
    //            |--------------> solid color ->
    
    root()->addOutput(splitterNode);
    splitterNode->addOutput(blurNode);
    blurNode->addOutput(bluredColorNode1);
    splitterNode->addOutput(baseColorNode2);
    bluredColorNode1->addOutput(blenderNode);
    baseColorNode2->addOutput(blenderNode);
    
    return true;
}

const ParameterInfo& TestFilterGraph::parameterInfo(index_t index)
{
    if (index == bluredColorNode1->firstParameterIndex())
    {
        Color color{255, 155, 5, 255};
        
        static ColorParameterInfo colorParameter("Blur Color", color, Color(), Color());
        return colorParameter;
    }
    else if (index == baseColorNode2->firstParameterIndex())
    {
        Color color{55, 155, 205, 255};
        
        static ColorParameterInfo colorParameter("Base Color", color, Color(), Color());
        return colorParameter;
    }
    
    return FilterGraph::parameterInfo(index);
}





