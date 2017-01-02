//
//  FilterGraph.cpp
//  UiTest
//
//  Created by Олег on 17.12.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "FilterGraph.h"
#include "BaseParameters.h"
#include "BaseParameterSet.h"
#include "BaseFilter.h"


class FilterNodeImpl : public FilterGraph::FilterNode
{
public:
    FilterNodeImpl (FilterPtr filter) : filterPtr(filter) {}
    virtual ~FilterNodeImpl() {}
    FilterPtr filter() override
    {
        return filterPtr;
    }
    
    const std::vector<FilterGraph::FilterNodePtr>& outputs() const override
    {
        return nodes;
    }
    
    void addOutput(FilterGraph::FilterNodePtr node) override
    {
        nodes.push_back(node);
    }
    
private:

    std::vector<FilterGraph::FilterNodePtr>& outputs() override
    {
        return nodes;
    }
    
    std::vector<FilterGraph::FilterNodePtr> nodes;
    FilterPtr filterPtr;
};


FilterGraph::FilterGraph(IResourceManager& resourceManager, const std::string& filterName) : rootNode(new FilterNodeImpl(FilterPtr())), resourceManager(resourceManager), filterName(filterName)
{
    
}

// Apply filter to frame.
bool FilterGraph::apply(const Frame* inputFrames, index_t inputFramesNumber, Frame* outputFrames, index_t outputFramesNumber, const IParameterSet& params)
{
    const Frame* currentFrame = inputFrames;
    index_t paramIndex = 0;
    
    auto func = [&](FilterNodePtr node)
    {
        auto filter = node->filter();
        FrameParams outputFrameParam;
        filter->outputFrameParams(currentFrame, &outputFrameParam);
        
        Frame* tempOutputFrame = (!node->outputs().empty() ? resourceManager.createFrame(outputFrameParam) : outputFrames);
        
        // Fill params for current filter.
        BaseParameterSet parameters;
        for (index_t i = 0; i < filter->parameterNumber(); i ++)
        {
            parameters.push_back(params.value(paramIndex));
            paramIndex++;
        }
        
        bool res = BaseFilter::apply(filter.get(), *currentFrame, *tempOutputFrame, parameters);
        if (currentFrame != inputFrames)
        {
            resourceManager.releaseFrame(const_cast<Frame*>(currentFrame));
        }
        
        currentFrame = tempOutputFrame;
        return res;
    };
    
    return aroundGraph(root(), func);
}

// @return number of parameters.
index_t FilterGraph::parameterNumber()
{
    index_t res = 0;
    
    auto func = [&](FilterNodePtr node)
    {
        res += node->filter()->parameterNumber();
        return true;
    };
    
    aroundGraph(root(), func);
    
    return res;
}

// @return parameter info.
const ParameterInfo& FilterGraph::parameterInfo(index_t index)
{
    FilterPtr filter;
    static ParameterInfo emptyParamInfo;
    
    auto func = [&](FilterNodePtr node)
    {
        if (index < node->filter()->parameterNumber())
        {
            filter = node->filter();
            return false;
        }

        index -= node->filter()->parameterNumber();
        return true;
    };
    
    aroundGraph(root(), func);
    
    return filter ? filter->parameterInfo(index) : emptyParamInfo;
}

// @return name. Latin only letters.
const char* const FilterGraph::name()
{
    return filterName.c_str();
}

// @return output frame params for input frame.
// If input frame format is unsupported, out frame format will be unsupported.
bool FilterGraph::outputFrameParams(const FrameParams* inputFrames, FrameParams* outputFrames)
{
    FrameParams currentFrameParams = *inputFrames;
    auto func = [&](FilterNodePtr node) -> bool
    {
        FrameParams outputFrameParams;
        bool res = node->filter()->outputFrameParams(&currentFrameParams, &outputFrameParams);
        currentFrameParams = outputFrameParams;
        return res;
    };
    
    bool res = aroundGraph(root(), func);
    *outputFrames = currentFrameParams;
    return res;
}

// @return number of input frames.
index_t FilterGraph::inputsNumber()
{
    return 0;
}

// @return number of output frames.
index_t FilterGraph::outputsNumber()
{
    return 0;
}

// Add filter to graph.
FilterGraph::FilterNodePtr FilterGraph::addFilter(FilterPtr filter)
{
    return FilterNodePtr(new FilterNodeImpl(filter));
}

//@return pseudo root node
FilterGraph::FilterNodePtr FilterGraph::root()
{
    return rootNode;
}

template <typename T> bool FilterGraph::aroundGraph(FilterNodePtr rootNode, T func)
{
    bool res = false;
    if (!rootNode->outputs().empty())
    {
        res = true;
        FilterNodePtr node = rootNode;
        while (!node->outputs().empty() && res)
        {
            node = node->outputs()[0];
            res = func(node);
        }
    }
    
    return res;
}
