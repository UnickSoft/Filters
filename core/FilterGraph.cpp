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
#include <string.h>

class FilterNodeImpl : public FilterGraph::FilterNode
{
public:
    FilterNodeImpl (FilterPtr filter, index_t startParameterIndex) : filterPtr(filter), startParameterIndex(startParameterIndex)
    {
        updateInputsNumber();
    }
    
    virtual ~FilterNodeImpl() {}
    FilterPtr filter() override
    {
        return filterPtr;
    }
    
    const std::vector<FilterGraph::OutputAndIndex>& outputs() const override
    {
        return outputNodes;
    }
    
    void addOutput(FilterGraph::FilterNodePtr node) override
    {
        auto index = filterPtr ? node->addInput(this) : 0;
        assert(index >= 0 && index < node->filter()->inputsNumber());
        outputNodes.push_back(FilterGraph::OutputAndIndex(node, index));
    }
    
    index_t firstParameterIndex() override
    {
        return startParameterIndex;
    }
    
    void clearOutputs() override
    {
        for (auto node : outputNodes)
        {
            node.first->removeInput(this);
        }
        outputNodes.clear();
    }
    
    void updateInputsNumber() override
    {
        if (filterPtr)
        {
            inputs.clear();
            inputs.resize(filterPtr->inputsNumber());
        }
    }
    
private:

    std::vector<FilterGraph::OutputAndIndex>& _outputs() override
    {
        return outputNodes;
    }
    
    int addInput(FilterGraph::FilterNode* input, int preferIndex = -1) override
    {
        int res = -1;
        if (preferIndex < 0)
        {
            auto freeElement = std::find(inputs.begin(), inputs.end(), nullptr);
            if (freeElement != inputs.end())
            {
                res = (int)(freeElement - inputs.begin());
            }
        }
        else
        {
            res = preferIndex < inputs.size() && inputs[preferIndex] == nullptr ? preferIndex : -1;
        }
        
        if (res >= 0)
        {
            inputs[res] = input;
        }
        
        return res;
    }
    void removeInput(FilterGraph::FilterNode* input) override
    {
        auto freeElement = std::find(inputs.begin(), inputs.end(), input);
        if (freeElement != inputs.end())
        {
            *freeElement = nullptr;
        }
    }
    
    FilterGraph::FilterNode* input(index_t index) override
    {
        return inputs[index];
    }
    
    index_t inputsNumber() override
    {
        return inputs.size();
    }
    
    virtual void setFirstParameterIndex(index_t index) override
    {
        startParameterIndex = index;
    }
    
    std::vector<FilterGraph::OutputAndIndex> outputNodes;
    FilterPtr filterPtr;
    index_t startParameterIndex;
    // These pointers are not safty. Please do not use them.
    std::vector<FilterGraph::FilterNode*> inputs;
};


FilterGraph::FilterGraph(const IPrivateFilterList& filterList, IResourceManager& resourceManager, const std::string& filterName) : rootNode(new FilterNodeImpl(FilterPtr(), 0)), resourceManager(resourceManager), filterName(filterName), isInited(false), filterList(filterList), paramNumber(0)
{}

// Apply filter to frame.
bool FilterGraph::apply(const Frame* inputFrames, index_t inputFramesNumber, Frame* outputFrames, index_t outputFramesNumber, const IParameterSet& params)
{
    if (!isInited)
    {
        isInited = init();
        updateFilterOrder();
    }
    
    std::unordered_map<FilterNodePtr, std::vector<Frame>> data;
    std::vector<Frame*> allocatedFrames;
    
    /*
    Frame* outputFramesTemp = new Frame[outputFramesNumber];
    for (int i = 0; i < outputFramesNumber; i++)
    {
        outputFramesTemp[i] = outputFrames[i];
    }
    */
    
    auto addOutputNodes = [&data](FilterNodePtr inputNode, const Frame* frames)
        {
            int index = 0;
            for (OutputAndIndex node : inputNode->outputs())
            {
                if (data.count(node.first) == 0)
                {
                    data[node.first].resize(node.first->filter()->inputsNumber());
                }
                
                assert(node.second < data[node.first].size());
                
                data[node.first][node.second] = frames[index];
                index++;
            }
        };
    
    auto func = [&](FilterNodePtr node, std::unordered_map<FilterNodePtr, std::vector<Frame>>& data)
    {
        auto filter = node->filter();
        std::unique_ptr<FrameParams[]> outputFrameParams(new FrameParams[filter->outputsNumber()] );
        
        filter->outputFrameParams(data[node].data(), outputFrameParams.get());
        
        std::vector<Frame> currentOutputFrames;
        
        for (int i = 0; i < filter->outputsNumber(); i++)
        {
            if (!node->outputs().empty())
            {
                allocatedFrames.push_back(resourceManager.createFrame(outputFrameParams[i]));
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
            parameters.push_back((BaseParameters)filter->parameterInfo(i).type, params.value(i + node->firstParameterIndex()));
        }
        
        bool res = filter->apply(data[node].data(), data[node].size(), currentOutputFrames.data(), currentOutputFrames.size(), parameters);
        
        // Copy output frame
        if (node->outputs().empty())
        {
            for (int i = 0; i < filter->outputsNumber(); i++)
            {
                outputFrames[i] = currentOutputFrames[i];
            }
        }
        
        addOutputNodes(node, currentOutputFrames.data());
        //int index = 0;
        //for (OutputAndIndex childNode : node->outputs())
        //{
        //    data[childNode].push_back(currentOutputFrames[index]);
        //    index++;
        //}
        
        return res;
    };
    
    addOutputNodes(root(), inputFrames);
    //for (OutputAndIndex node : root()->outputs())
    //{
    //    data[node.first].push_back(*inputFrames);
    //}
    
    bool res = aroundGraph(root(), func, data);
    
    for (auto* frame : allocatedFrames)
    {
        resourceManager.releaseFrame(frame);
    }
    
    //delete[] outputFramesTemp;
                                     
    return res;
}

// @return number of parameters.
index_t FilterGraph::parameterNumber()
{
    if (!isInited)
    {
        isInited = init();
        updateFilterOrder();
    }
    
    return paramNumber;
}

// @return parameter info.
const ParameterInfo& FilterGraph::parameterInfo(index_t index)
{
    if (!isInited)
    {
        isInited = init();
        updateFilterOrder();
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
        updateFilterOrder();
    }
    
    std::unordered_map<FilterNodePtr, std::vector<FrameParams>> data;
    
    auto addOutputNodes = [&data](FilterNodePtr inputNode, const FrameParams* frames)
        {
            int index = 0;
            for (OutputAndIndex node : inputNode->outputs())
            {
                if (data.count(node.first) == 0)
                {
                    data[node.first].resize(node.first->filter()->inputsNumber());
                }
                
                assert(node.second < data[node.first].size());
                
                data[node.first][node.second] = frames[index];
                index++;
            }
        };
    
    auto func = [&](FilterNodePtr node, std::unordered_map<FilterNodePtr, std::vector<FrameParams>>& data) -> bool
    {
        auto filter = node->filter();
        std::unique_ptr<FrameParams[]> outputFrameParams(new FrameParams[filter->outputsNumber()] );
        
        assert(data.count(node) > 0);
        
        bool res = filter->outputFrameParams(data[node].data(), outputFrameParams.get());
        
        if (!node->outputs().empty())
        {
            addOutputNodes(node, outputFrameParams.get());
            //int index = 0;
            //for (FilterNodePtr childNode : node->outputs())
            //{
            //    data[childNode].push_back(outputFrameParams[index]);
            //    index++;
           ///}
        }
        else
        {
            data[root()].push_back(outputFrameParams[0]);
        }
        return res;
    };
    
    addOutputNodes(root(), inputFrames);
    //for (FilterNodePtr node : root()->outputs())
    //{
    //    data[node].push_back(*inputFrames);
    //}
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

void FilterGraph::preAroundGraph()
{
    // do nothing.
}

// Generate report like this:
//  Splitter ->       Blur -> Solid Colo ->
//              Solid Colo ->
//                  Dilate -> Solid Colo -> Alpha Blen -> x
bool FilterGraph::dumpGraph(char* buffer, int nBufferSize)
{
    const int columnWidth = 10;
    const char divider[] = " -> ";
    
    bool res = false;
    if (!isInited)
    {
        isInited = init();
        updateFilterOrder();
    }
    
    std::string report;
    
    // Hold current offset for node.
    std::unordered_map<FilterNodePtr, index_t> depth;
    // Calculate real number of parameters.
    index_t calculatedParameterNumber = 0;
    
    auto func = [&](FilterNodePtr node)
    {
        if (depth.count(node) == 0)
        {
            depth[node] = 0;
        }
        else
        {
            report += "\n";
        }
        
        calculatedParameterNumber += node->filter()->parameterNumber();
        
        // Fill offset
        auto spaceOffset = depth[node] * (columnWidth + (sizeof(divider) - 1));
        for (int i = 0; i < spaceOffset; i ++)
        {
            report += " ";
        }
        
        // Add filter name
        char formatedSting[columnWidth + 1];
        snprintf(formatedSting, sizeof(formatedSting), ("%" + std::to_string(10) + "s").c_str(), node->filter()->name());
  
        report += std::string(formatedSting) + divider;
        
        // Add x for last filter.
        if (node->outputs().empty())
        {
            report += "x";
        }
        
        // Fill node offset for non last outputs.
        for (int i = 0; i < (int)node->outputs().size() - 1; i++)
        {
            depth[node->outputs()[i].first] = depth[node] + 1;
        }

        return true;
    };
    
    res = aroundGraph(root(), func);
    
    if (calculatedParameterNumber != paramNumber)
    {
        report += "\n\nWrong parameter number!!! Holds in class is " + std::to_string(paramNumber) + ", but real number is " + std::to_string(calculatedParameterNumber) + ".\n";
    }
    
    if (res && report.size() <= nBufferSize)
    {
        strcpy(buffer, report.data());
        res = true;
    }
    
    return res;
}

void FilterGraph::removeFilter(FilterNodePtr filter)
{
    paramNumber -= filter->filter()->parameterNumber();
    
    const std::vector<FilterGraph::OutputAndIndex>& outputs = filter->outputs();
    
    for (index_t i = 0; i < outputs.size(); i++)
    {
        auto output = outputs[i];
        output.first->removeInput(filter.get());
        
        if (outputs.size() == filter->inputsNumber() && filter->input(i))
        {
            for (index_t j = 0; j < filter->input(i)->outputs().size(); j++)
            {
                if (filter->input(i)->outputs()[j].first == filter)
                {
                    filter->input(i)->_outputs()[j].first = output.first;
                    break;
                }
            }
        }
    }
}

void FilterGraph::updateFilterOrder()
{
    if (isInited)
    {
        FilterPtr filter;
        
        index_t numberParameters = 0;
        
        auto func = [&](FilterNodePtr node)
        {
            node->setFirstParameterIndex(numberParameters);
            numberParameters += node->filter()->parameterNumber();
            
            return true;
        };
        
        aroundGraph(root(), func);
        
        //paramNumber = numberParameters;
    }
}


ROI FilterGraph::outputRoi(const ROI& inputRoi, const IParameterSet& params)
{
    if (!isInited)
    {
        isInited = init();
        updateFilterOrder();
    }
    
    ROI res = {};
    std::unordered_map<FilterNodePtr, std::vector<ROI>> data;
    
    auto addOutputRoi = [&data](FilterNodePtr inputNode, const ROI& roi)
        {
            for (OutputAndIndex node : inputNode->outputs())
            {
                data[node.first].push_back(roi);
            }
        };
    
    auto func = [&](FilterNodePtr node, std::unordered_map<FilterNodePtr, std::vector<ROI>>& data) -> bool
    {
        auto filter = node->filter();
        
        BaseParameterSet parameters;
        for (index_t i = 0; i < filter->parameterNumber(); i ++)
        {
            parameters.push_back((BaseParameters)filter->parameterInfo(i).type, params.value(i + node->firstParameterIndex()));
        }
        
        assert(data.count(node) > 0);
        
        ROI outputRoi = {};
        
        for (auto roi : data[node])
        {
            ROI currentRoi = filter->outputRoi(roi, parameters);
            outputRoi = {
                  std::min(currentRoi.x, outputRoi.x),
                  std::min(currentRoi.y, outputRoi.y),
                  std::max(currentRoi.x + currentRoi.width, outputRoi.x + outputRoi.width),
                  std::max(currentRoi.y + currentRoi.height, outputRoi.y + outputRoi.height),
                  };
            
            outputRoi.width  = outputRoi.width  - outputRoi.x;
            outputRoi.height = outputRoi.height - outputRoi.y;
        }
        
        if (!node->outputs().empty())
        {
            addOutputRoi(node, outputRoi);
            //int index = 0;
            //for (FilterNodePtr childNode : node->outputs())
            //{
            //    data[childNode].push_back(outputFrameParams[index]);
            //    index++;
           ///}
        }
        else
        {
            data[root()].push_back(outputRoi);
        }
        return outputRoi.width > 0 && outputRoi.height > 0;
    };
    
    addOutputRoi(root(), inputRoi);
    
    if (aroundGraph(root(), func, data))
    {
        res = data[root()].front();
    }
    
    return res;
}
