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


class FilterGraph : public IFilter
{
public:

    class FilterNode;
    
    typedef std::shared_ptr<FilterNode> FilterNodePtr;
    
    class FilterNode
    {
        friend FilterGraph;
    public:
        virtual ~FilterNode() {}
        virtual FilterPtr filter() = 0;
        virtual const std::vector<FilterNodePtr>& outputs() const = 0;
        virtual void addOutput(FilterNodePtr node) = 0;
        
    private:
        virtual std::vector<FilterNodePtr>& outputs() = 0;
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
    
    // @return number of input frames.
    virtual index_t inputsNumber() override;
    
    // @return number of output frames.
    virtual index_t outputsNumber() override;

    
    // Add filter to graph.
    FilterNodePtr addFilter(FilterPtr filter);
    
    //@return pseudo root node
    FilterNodePtr root();
    
protected:
    
    // Init filter graph.
    virtual bool init() = 0;
    
    IResourceManager& resourceManager;
    const IPrivateFilterList& filterList;

private:

    template <typename TFunc, typename TInputData> bool aroundGraph(FilterNodePtr rootNode, TFunc func, TInputData& data);
    template <typename TFunc> bool aroundGraph(FilterNodePtr rootNode, TFunc func);

    FilterNodePtr rootNode;
    std::string filterName;
    
    bool isInited;
    index_t paramNumber;

};