//
//  IFilter.h
//  test
//
//  Created by Олег on 11.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#ifndef IFilter_h
#define IFilter_h

#include "Base.h"
#include "IParameterSet.h"
#include <memory>

class IFilter : public VirtualDestructor
{
    public:
    
        // Apply filter to frame.
        virtual bool apply(const Frame* inputFrames, index_t inputFramesNumber, Frame* outputFrames, index_t outputFramesNumber, const IParameterSet& params) = 0;
    
        // @return number of input frames.
        virtual index_t inputsNumber() = 0;
    
        // @return number of output frames.
        virtual index_t outputsNumber() = 0;
    
        // @return number of parameters.
        virtual index_t parameterNumber() = 0;
    
        // @return parameter info.
        virtual const ParameterInfo& parameterInfo(index_t) = 0;
    
        // @return name. Latin only letters.
        virtual const char* const name() = 0;
    
        // @return output frames params for input frames.
        // If input frame format is unsupported, out frame format will be unsupported.
        // Input frames and output frames should be inputsNumber/outputsNumber.
        virtual bool outputFrameParams(const FrameParams* inputFrames, FrameParams* outputFrames) = 0;
};

typedef std::shared_ptr<IFilter> FilterPtr;

#endif /* IFilter_h */
