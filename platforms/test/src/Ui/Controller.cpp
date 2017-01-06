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
#include "BaseFilter.h"


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

void Controller::applyFilter(index_t index, const IParameterSet* parameters, QVector<QImage>& source, QVector<QImage>& dest)
{
    auto filter = bridge.createFilter(index);
    if (filter)
    {
        if (filter->inputsNumber() <= source.size())
        {
            Frame sourceFrame[2];
            
            // Convert Qt image to Frame.
            for (int i = 0; i < 2; i++)
            {
                sourceFrame[i].width    = source[i].width();
                sourceFrame[i].height   = source[i].height();
                sourceFrame[i].format   = formatMap.value(source[i].format());
                sourceFrame[i].byteSpan = source[i].bytesPerLine();
                sourceFrame[i].data     = reinterpret_cast<uint8_t*>(source[i].bits());
            }
            
            FrameParams inputsFrameParams[2];
            
            for (int i = 0; i < filter->inputsNumber(); i++)
            {
                inputsFrameParams[i] = sourceFrame[i];
            }
            
            FrameParams outputFrameParams[2];
            
            if (filter->outputFrameParams(inputsFrameParams, outputFrameParams))
            {
                Frame destFrame[2];
            
                for (int i = 0; i < filter->outputsNumber(); i ++)
                {
                    QImage destImage = QImage(outputFrameParams[i].width, outputFrameParams[i].height, formatMap.key(outputFrameParams[i].format));
                    dest.push_back(destImage);
                    
                    destFrame[i].width    = dest[i].width();
                    destFrame[i].height   = dest[i].height();
                    destFrame[i].format   = formatMap.value(dest[i].format());
                    destFrame[i].byteSpan = dest[i].bytesPerLine();
                    destFrame[i].data     = reinterpret_cast<uint8_t*>(dest[i].bits());
                }
                
                filter.get()->apply(sourceFrame, filter->inputsNumber(), destFrame, filter->outputsNumber(), *parameters);
            }
            else
            {
                QImage destImage = QImage(source[0].width(), source[0].height(), source[0].format());
                drawMessage(destImage, "Unsupported format");
                dest.push_back(destImage);
            }
        }
        else
        {
            QImage destImage = QImage(source[0].width(), source[0].height(), source[0].format());
            drawMessage(destImage, "Wrong number of inputs");
            dest.push_back(destImage);
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

void Controller::drawMessage(QImage& image, const QString& text)
{
    QPainter painter(&image);
    painter.fillRect(0, 0, image.width(), image.height(), QBrush(QColor(Qt::white)));
    painter.setFont(QFont("Arial", 30));
    painter.drawText(QRect(QPoint(0, 0), image.size()), Qt::AlignCenter, text);
}


