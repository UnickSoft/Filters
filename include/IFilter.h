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


class IFilter : public VirtualDestructor
{
    public:
    
        // Apply filter to frame.
        virtual bool apply(const Frame& inputFrame, Frame& outputFrame, const IParameterSet& params) = 0;
    
        // @return number of parameters.
        virtual index_t parameterNumber() = 0;
    
        // @return parameter info.
        virtual const ParameterInfo& parameterInfo(index_t) = 0;
    
        // @return name. Latin only letters.
        virtual const char* const name() = 0;
    
        // @return output frame params for input frame.
        // If input frame format is unsupported, out frame format will be unsupported.
        virtual FrameParams outputFrameParams(const FrameParams& inputFrame) = 0;
};

#endif /* IFilter_h */
