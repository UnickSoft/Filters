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
#include "FilterPreset.h"


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
        if (std::string(filter->name()) == std::string("ROI Copy"))
        {
            copyFilter_ = filter;
        }
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
            const int maxOutputFrames = 2;
            Frame sourceFrame[maxOutputFrames];
            
            // Convert Qt image to Frame.
            for (int i = 0; i < maxOutputFrames; i++)
            {
                sourceFrame[i].width    = source[i].width();
                sourceFrame[i].height   = source[i].height();
                sourceFrame[i].format   = formatMap.value(source[i].format());
                sourceFrame[i].byteSpan = source[i].bytesPerLine();
                sourceFrame[i].data     = reinterpret_cast<uint8_t*>(source[i].bits());
                sourceFrame[i].roi      = {0, 0, sourceFrame[i].width, sourceFrame[i].height};
            }
            
            FrameParams inputsFrameParams[maxOutputFrames];
            
            for (int i = 0; i < filter->inputsNumber(); i++)
            {
                inputsFrameParams[i] = sourceFrame[i];
            }
            
            FrameParams outputFrameParams[maxOutputFrames];
            
            if (filter->outputFrameParams(inputsFrameParams, outputFrameParams))
            {
                Frame destFrame[maxOutputFrames];
                
                ROI inputRoi = {0, 0, sourceFrame[0].width, sourceFrame[0].height};
                auto outputRoi = filter->outputRoi(inputRoi, *parameters);
                
                Frame* freeFrame[maxOutputFrames] = {};
                
                if (std::memcmp(&inputRoi, &outputRoi, sizeof(inputRoi)) != 0)
                {
                    for (int i = 0; i < filter->inputsNumber(); i++)
                    {
                        freeFrame[i] = getFrameWithROI(sourceFrame[i], outputRoi);
                        sourceFrame[i] = *freeFrame[i];
                    }
                }
                

                for (int i = 0; i < filter->outputsNumber(); i ++)
                {
                    QImage destImage = QImage(sourceFrame[0].width, sourceFrame[0].height, formatMap.key(outputFrameParams[i].format));
                    dest.push_back(destImage);
                    
                    dest[i] = QImage(sourceFrame[0].width, sourceFrame[0].height, formatMap.key(outputFrameParams[i].format));
                    
                    dest[i].fill(0x00);
                    destFrame[i].width    = dest[i].width();
                    destFrame[i].height   = dest[i].height();
                    destFrame[i].format   = formatMap.value(dest[i].format());
                    destFrame[i].byteSpan = dest[i].bytesPerLine();
                    destFrame[i].data     = reinterpret_cast<uint8_t*>(dest[i].bits());
                }
                
                if (!filter.get()->apply(sourceFrame, filter->inputsNumber(), destFrame, filter->outputsNumber(), *parameters))
                {
                    drawMessage(dest[0], "Render error");
                }
                else
                {
                    // Draw output roi
                    for (int i = 0; i < filter->outputsNumber(); i ++)
                    {
                        QPainter painter(&dest[i]);
                        
                        painter.drawRect(destFrame[i].roi.x - 1, destFrame[i].roi.y - 1, destFrame[i].roi.width + 1, destFrame[i].roi.height + 1);
                    }
                }
                
                for (int i = 0; i < maxOutputFrames; i++)
                {
                    resourceManager.releaseFrame(freeFrame[i]);
                }
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

bool Controller::savePreset(const IParameterSet& params, index_t filterIndex, const QString& presetName, const QString& fileName)
{
    return FilterPreset(params, bridge.createFilter(filterIndex), presetName.toStdString()).save(fileName.toStdString());
}

bool Controller::loadPreset(const QString& fileName, FilterPreset& preset)
{
    preset = FilterPreset(fileName.toStdString());
    return preset.isValid();
}

Frame* Controller::getFrameWithROI(const Frame& inputFrame, const ROI& outputROI)
{
    FrameParams resFrameParams = inputFrame;
    
    ROI inputROI = {-std::min(0, outputROI.x), -std::min(0, outputROI.y), inputFrame.width, inputFrame.height};
    
    resFrameParams.width = std::max(inputFrame.width, outputROI.x + outputROI.width) + inputROI.x;
    resFrameParams.height = std::max(inputFrame.height, outputROI.y + outputROI.height) + inputROI.y;
    

    Frame* res = resourceManager.createFrame(resFrameParams);
    std::memset(res->data, 0, res->height * res->byteSpan);
    res->roi = inputROI;
    
    BaseParameterSet params;
    params.push_back(ROIParameter({0, 0, inputFrame.width, inputFrame.height}));
    params.push_back(ROIParameter(inputROI));
    
    bool copyed = copyFilter_->apply(&inputFrame, 1, res, 1, params);
    if (!copyed)
    {
        resourceManager.releaseFrame(res);
        res = nullptr;
        assert(!"Cannot copy frames");
    }
    
    return res;
}


