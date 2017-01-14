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
#include <unordered_map>
#include <stack>

class FilterNodeImpl : public FilterGraph::FilterNode
{
public:
    FilterNodeImpl (FilterPtr filter, index_t startParameterIndex) : filterPtr(filter), startParameterIndex(startParameterIndex) {}
    
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
    
    index_t firstParameterIndex() override
    {
        return startParameterIndex;
    }
    
private:

    std::vector<FilterGraph::FilterNodePtr>& outputs() override
    {
        return nodes;
    }
    
    std::vector<FilterGraph::FilterNodePtr> nodes;
    FilterPtr filterPtr;
    index_t startParameterIndex;
};


FilterGraph::FilterGraph(const IPrivateFilterList& filterList, IResourceManager& resourceManager, const std::string& filterName) : rootNode(new FilterNodeImpl(FilterPtr(), 0)), resourceManager(resourceManager), filterName(filterName), isInited(false), filterList(filterList), paramNumber(0)
{}

// Apply filter to frame.
bool FilterGraph::apply(const Frame* inputFrames, index_t inputFramesNumber, Frame* outputFrames, index_t outputFramesNumber, const IParameterSet& params)
{
    if (!isInited)
    {
        isInited = init();
    }
    
    std::unordered_map<FilterNodePtr, std::vector<Frame>> data;
    std::vector<Frame*> allocatedFrames;
    
    auto func = [&](FilterNodePtr node, std::unordered_map<FilterNodePtr, std::vector<Frame>>& data)
    {
        auto filter = node->filter();
        FrameParams outputFrameParam[2];
        filter->outputFrameParams(data[node].data(), outputFrameParam);
        
        std::vector<Frame> currentOutputFrames;
        
        for (int i = 0; i < filter->outputsNumber(); i++)
        {
            if (!node->outputs().empty())
            {
                allocatedFrames.push_back(resourceManager.createFrame(outputFrameParam[i]));
                currentOutputFrames.push_back(*allocatedFrames.back());
            }
            else
            {
                currentOutputFrames.push_back(outputFrames[i]);
            }
        }
        
        // Fill params for current filter.
        BaseParameterSet parameters;
        for (index_t i = 0; i < filter->parameterNumber(); i ++)
        {
            parameters.push_back(params.value(i + node->firstParameterIndex()));
        }
        
        bool res = filter->apply(data[node].data(), data[node].size(), currentOutputFrames.data(), currentOutputFrames.size(), parameters);
        
        int index = 0;
        for (FilterNodePtr childNode : node->outputs())
        {
            data[childNode].push_back(currentOutputFrames[index]);
            index++;
        }
        
        return res;
    };
    
    for (FilterNodePtr node : root()->outputs())
    {
        data[node].push_back(*inputFrames);
    }
    
    bool res = aroundGraph(root(), func, data);
    
    for (auto* frame : allocatedFrames)
    {
        resourceManager.releaseFrame(frame);
    }
                                     
    return res;
}

// @return number of parameters.
index_t FilterGraph::parameterNumber()
{
    if (!isInited)
    {
        isInited = init();
    }
    
    return paramNumber;
}

// @return parameter info.
const ParameterInfo& FilterGraph::parameterInfo(index_t index)
{
    if (!isInited)
    {
        isInited = init();
    }
    
    FilterPtr filter;
    static ParameterInfo emptyParamInfo;
    
    index_t localIndex = 0;
    auto func = [&](FilterNodePtr node)
    {
        if (index >= node->firstParameterIndex() && index < node->firstParameterIndex() + node->filter()->parameterNumber())
        {
            filter = node->filter();
            localIndex = index - node->firstParameterIndex();
            return false;
        }

        return true;
    };
    
    aroundGraph(root(), func);
    
    return filter ? filter->parameterInfo(localIndex) : emptyParamInfo;
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
    if (!isInited)
    {
        isInited = init();
    }
    
    std::unordered_map<FilterNodePtr, std::vector<FrameParams>> data;
    
    auto func = [&](FilterNodePtr node, std::unordered_map<FilterNodePtr, std::vector<FrameParams>>& data) -> bool
    {
        FrameParams outputFrameParams[2];
        
        bool res = node->filter()->outputFrameParams(data[node].data(), outputFrameParams);
        
        if (!node->outputs().empty())
        {
            int index = 0;
            for (FilterNodePtr childNode : node->outputs())
            {
                data[childNode].push_back(outputFrameParams[index]);
                index++;
            }
        }
        else
        {
            data[root()].push_back(outputFrameParams[0]);
        }
        return res;
    };
    
    for (FilterNodePtr node : root()->outputs())
    {
        data[node].push_back(*inputFrames);
    }
    bool res = aroundGraph(root(), func, data);
    if (res)
    {
        outputFrames[0] = data[root()].front();
    }
    return res;
}

// @return number of input frames.
index_t FilterGraph::inputsNumber()
{
    return 1;
}

// @return number of output frames.
index_t FilterGraph::outputsNumber()
{
    return 1;
}

// Add filter to graph.
FilterGraph::FilterNodePtr FilterGraph::addFilter(FilterPtr filter)
{
    auto res = FilterNodePtr(new FilterNodeImpl(filter, paramNumber));
    paramNumber += filter->parameterNumber();
    return res;
}

//@return pseudo root node
FilterGraph::FilterNodePtr FilterGraph::root()
{
    return rootNode;
}

template <typename TFunc, typename TInputData> bool FilterGraph::aroundGraph(FilterNodePtr rootNode, TFunc func, TInputData& data)
{
    bool res = false;
    if (!rootNode->outputs().empty())
    {
        res = true;
        std::stack<FilterNodePtr> aroundOrder;
        
        aroundOrder.push(rootNode);
        
        // Holds number of inputs ready for node.
        std::unordered_map<FilterNodePtr, index_t> processedNumber;
        
        while (!aroundOrder.empty() && !aroundOrder.top()->outputs().empty() && res)
        {
            FilterNodePtr current = aroundOrder.top();
            aroundOrder.pop();
            for (FilterNodePtr node : current->outputs())
            {
                auto readyInputs = ++processedNumber[node];
                if (readyInputs == node->filter()->inputsNumber())
                {
                    res = func(node, data);
                    aroundOrder.push(node);
                    if (!res)
                    {
                        break;
                    }
                }
            }
        }
    }
    
    return res;
}

template <typename TFunc> bool FilterGraph::aroundGraph(FilterNodePtr rootNode, TFunc func)
{
    std::unordered_map<FilterNodePtr, bool> data;
    auto funcNew = [&](FilterNodePtr node, std::unordered_map<FilterNodePtr, bool>) -> bool
    {
        return func(node);
    };
    return FilterGraph::aroundGraph(rootNode, funcNew, data);
}

