//
//  FilterGraph.hpp
//  UiTest
//
//  Created by Олег on 17.12.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#pragma once

#include "IFilter.h"
#include <vector>
#include <string>
#include "IResourceManager.h"
#include "IPrivateFilterList.h"
#include <stack>
#include <unordered_map>

class FilterNodeImpl;

class FilterGraph : public IFilter
{
public:

    class FilterNode;
    
    typedef std::shared_ptr<FilterNode> FilterNodePtr;
    typedef std::pair<FilterGraph::FilterNodePtr, index_t> OutputAndIndex;
    
    class FilterNode
    {
        friend FilterGraph;
        friend FilterNodeImpl;
        
    public:
        virtual ~FilterNode() {}
        virtual FilterPtr filter() = 0;
        virtual const std::vector<OutputAndIndex>& outputs() const = 0;
        virtual void addOutput(FilterNodePtr node) = 0;
        virtual index_t firstParameterIndex() = 0;
        virtual void clearOutputs() = 0;
        virtual void updateInputsNumber() = 0;
        
    private:
        // Output and input frame number in output.
        virtual std::vector<OutputAndIndex>& _outputs() = 0;
        // @return selected input index.
        virtual int addInput(FilterNode* input, int preferIndex = -1) = 0;
        virtual void removeInput(FilterNode* input) = 0;
        virtual FilterGraph::FilterNode* input(index_t index) = 0;
        virtual index_t inputsNumber() = 0;
        virtual void setFirstParameterIndex(index_t index) = 0;
    };
    
    FilterGraph(const IPrivateFilterList& filterList, IResourceManager& resourceManager,const std::string& filterName);
    
    // Apply filter to frame.
    bool apply(const Frame* inputFrames, index_t inputFramesNumber, Frame* outputFrames, index_t outputFramesNumber, const IParameterSet& params) override;
    
    // @return number of parameters.
    index_t parameterNumber() override;
    
    // @return parameter info.
    const ParameterInfo& parameterInfo(index_t) override;
    
    // @return name. Latin only letters.
    const char* const name() override;
    
    // @return output frame params for input frame.
    // If input frame format is unsupported, out frame format will be unsupported.
    bool outputFrameParams(const FrameParams* inputFrames, FrameParams* outputFrames) override;
    
    ROI outputRoi(const ROI& inputRoi, const IParameterSet& params) override;
    
    // @return number of input frames.
    virtual index_t inputsNumber() override;
    
    // @return number of output frames.
    virtual index_t outputsNumber() override;
    
    // Dump current graph structure.
    virtual bool dumpGraph(char* buffer, int nBufferSize);
    
    // Add filter to graph.
    FilterNodePtr addFilter(FilterPtr filter);

    // Remove filter from graph.
    void removeFilter(FilterNodePtr filter);
    
    //@return pseudo root node
    FilterNodePtr root();
    
protected:
    
    // Init filter graph.
    virtual bool init() = 0;
    
    // Pre around the graph
    // This function called before aroundGraph.
    virtual void preAroundGraph();
    
    // Enum graph.
    template <typename TFunc> bool aroundGraph(FilterNodePtr rootNode, TFunc func);
    template <typename TFunc, typename TInputData> bool aroundGraph(FilterNodePtr rootNode, TFunc func, TInputData& data);
    
    // Update parameter indexes.
    void updateFilterOrder();
    
    IResourceManager& resourceManager;
    const IPrivateFilterList& filterList;
    
    bool isInited;

private:

    FilterNodePtr rootNode;
    std::string filterName;
    
    index_t paramNumber;

};


// Should be not internal.
template <typename TFunc, typename TInputData> bool FilterGraph::aroundGraph(FilterNodePtr rootNode, TFunc func, TInputData& data)
{
    bool res = false;
    
    preAroundGraph();
    
    if (!rootNode->outputs().empty())
    {
        res = true;
        std::stack<FilterNodePtr> aroundOrder;
        
        aroundOrder.push(rootNode);
        
        // Holds number of inputs ready for node.
        std::unordered_map<FilterNodePtr, index_t> processedNumber;
        
        while (!aroundOrder.empty() && /*!aroundOrder.top()->outputs().empty() &&*/ res)
        {
            FilterNodePtr current = aroundOrder.top();
            aroundOrder.pop();
            
            if (current != rootNode)
            {
                res = func(current, data);
                if (!res)
                {
                    break;
                }
            }
            for (OutputAndIndex node : current->outputs())
            {
                auto readyInputs = ++processedNumber[node.first];
                if (readyInputs == node.first->filter()->inputsNumber())
                {
                    //res = func(node.first, data);
                               
                    aroundOrder.push(node.first);
                    //if (!res)
                    //{
                    //    break;
                    //}
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

