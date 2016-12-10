//
//  Controller.cpp
//  UiTest
//
//  Created by Олег on 25.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "Controller.h"
#include "BaseParameterSet.h"
#include <QTGui/QPainter>


Controller::Controller ()
{
    bridge.setResourceManager(&resourceManager);
    formatMap[QImage::Format_RGB888]   = FrameParams::RGB8;
    formatMap[QImage::Format_RGBA8888] = FrameParams::RGBA8;
    formatMap[QImage::Format_Alpha8]   = FrameParams::Alpha8;
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

void Controller::applyFilter(index_t index, const IParameterSet* parameters, QImage& source, QImage& dest)
{
    auto filter = bridge.createFilter(index);
    if (filter)
    {
        Frame sourceFrame;
        sourceFrame.width    = source.width();
        sourceFrame.height   = source.height();
        sourceFrame.format   = formatMap.value(source.format());
        sourceFrame.byteSpan = source.bytesPerLine();
        sourceFrame.data     = reinterpret_cast<uint8_t*>(source.bits());
    
        auto outputFrameParams = filter->outputFrameParams(sourceFrame);
        
        if (outputFrameParams.format != FrameParams::Unsupported)
        {
            dest = QImage(outputFrameParams.width, outputFrameParams.height, formatMap.key(outputFrameParams.format));
            
            Frame destFrame;
            destFrame.width    = dest.width();
            destFrame.height   = dest.height();
            destFrame.format   = formatMap.value(dest.format());
            destFrame.byteSpan = dest.bytesPerLine();
            destFrame.data     = reinterpret_cast<uint8_t*>(dest.bits());
            
            filter->apply(sourceFrame, destFrame, *parameters);
        }
        else
        {
            dest = QImage(source.width(), source.height(), source.format());

            QPainter painter(&dest);
            painter.setFont(QFont("Arial", 30));
            painter.drawText(QRect(QPoint(0, 0), dest.size()), Qt::AlignCenter, "Unsupported format");
        }
    }
}

QVector<ParameterInfo> Controller::parameterList(index_t index)
{
    QVector<ParameterInfo> res;
    
    auto filter = bridge.createFilter(index);

    if (filter)
    {
        for (index_t i = 0; i < filter->parameterNumber(); i ++)
        {
            res.push_back(filter->parameterInfo(i));
        }
    }
    
    return res;
}


