//
//  Controller.cpp
//  UiTest
//
//  Created by Олег on 25.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "Controller.h"
#include "BaseParameterSet.h"


Controller::Controller ()
{
    bridge.setResourceManager(&resourceManager);
    
}

QList<QString> Controller::filters()
{
    QList<QString> res;
    for (int i = 0; i < bridge.filtersNumber(); i++)
    {
        auto filter = bridge.createFilter(i);
        res.push_back(filter->name());
    }
    return res;
}

void Controller::applyFilter(index_t index, QImage& source, QImage& dest)
{
    auto filter = bridge.createFilter(index);
    if (filter)
    {
        Frame sourceFrame;
        sourceFrame.width    = source.width();
        sourceFrame.height   = source.height();
        sourceFrame.byteSpan = source.bytesPerLine();
        sourceFrame.data     = reinterpret_cast<uint8_t*>(source.bits());
    
        Frame destFrame;
        destFrame.width    = dest.width();
        destFrame.height   = dest.height();
        destFrame.byteSpan = dest.bytesPerLine();
        destFrame.data     = reinterpret_cast<uint8_t*>(dest.bits());
        
        // Fill parameters.
        BaseParameterSet params;
        for (index_t i = 0; i < filter->parameterNumber(); i ++)
        {
            params.add(filter->parameterInfo(i).defaultValue);
        }
        
        filter->apply(&sourceFrame, &destFrame, &params);
    }
}

