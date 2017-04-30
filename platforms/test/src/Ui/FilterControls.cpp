//
//  FilterControls.cpp
//  UiTest
//
//  Created by Олег on 28.10.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "FilterControls.h"
#include <QtWidgets/QLayoutItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSizePolicy>
#include <cassert>
#include "ParameterHelpers.h"
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include "ImageControl.h"
#include <QtWidgets/QFileDialog>
#include <QtCore/QByteArray.h>
#include <QtWidgets/QCheckBox.h>
#include "ColorControl.h"
#include "BaseParameterSet.h"



FilterControls::FilterControls ()
{
    setLayout(new QVBoxLayout());
}

void FilterControls::setFilter(const QString& title, const QVector<ParameterInfo>& parameters, index_t firstParamIndex)
{
    removeOldControls();
    
    auto vLayout  = qobject_cast<QVBoxLayout*>(layout());
    vLayout->setContentsMargins(0, 0, 0, 0);
    
    auto header = new QWidget(this);
    auto headerLayout  = new QHBoxLayout(header);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    header->setLayout(headerLayout);
    
    auto remove = new QPushButton("X", this);
    headerLayout->addWidget(new QLabel(title));
    headerLayout->addWidget(remove, 0, Qt::AlignRight);
    
    vLayout->addWidget(header);
    
    index_t index = 0;
    for (ParameterInfo param : parameters)
    {
        auto control = createControl(param, firstParamIndex + index);
        if (control)
        {
            control->setContentsMargins(0, 0, 0, 0);
            vLayout->addWidget(control);
        }
        index++;
    }
    
    vLayout->addStretch();
    
    connect(remove, &QPushButton::clicked, [=](){
        emit removeFilter(filter_);
    });
}

QWidget* FilterControls::createControl(const ParameterInfo& parameterInfo, index_t index)
{
    QWidget* res = nullptr;
    switch (parameterInfo.type)
    {
        case BS_UINT:  res = createUintControl(parameterInfo, index); break;
        case BS_ROI:   res = createROIControl(parameterInfo, index); break;
        case BS_MASK:  res = createMaskControl(parameterInfo, index); break;
        case BS_BOOL:  res = createBoolControl(parameterInfo, index); break;
        case BS_COLOR: res = createColorControl(parameterInfo, index); break;
            
        default: assert(false && "Unknown parameter");
    }
    
    return res;
}


QWidget* FilterControls::createUintControl(const ParameterInfo& parameterInfo, index_t index)
{
    QWidget* res = new QWidget();
    auto layout = new QVBoxLayout();
    res->setLayout(layout);
    
    QLabel* currentValue = new QLabel("0");
    QHBoxLayout* titleLayout = new QHBoxLayout();
    
    titleLayout->addWidget(new QLabel(parameterInfo.title));
    titleLayout->addWidget(currentValue);
    
    layout->addLayout(titleLayout);
    
    auto slider = new QSlider(Qt::Horizontal);
    slider->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    slider->setMaximum(UintParameter::field(&parameterInfo.values.range.max));
    slider->setMinimum(UintParameter::field(&parameterInfo.values.range.min));

    connect(slider, &QSlider::valueChanged, [=]()
    {
        currentValue->setText(QString::number(slider->value()));
        emit paramChanged(index, UintParameter(slider->value()));
    });
    
    slider->setValue(UintParameter::field(&parameterInfo.defaultValue));
    
    layout->addWidget(slider);
    layout->addStretch();
    
    layout->setContentsMargins(0, 0, 0, 0);
    
    return res;
}

void FilterControls::removeOldControls()
{
    while (QWidget* w = findChild<QWidget*>())
    {
        delete w;
    }
    
    while (QLayoutItem *wItem = layout()->takeAt(0))
    {
        delete wItem;
    }
}

QWidget* FilterControls::createROIControl(const ParameterInfo& parameterInfo, index_t index)
{
    QWidget* res = new QWidget();
    auto layout = new QVBoxLayout();
    res->setLayout(layout);
    
    layout->addWidget(new QLabel(parameterInfo.title));
    
    QWidget* controls = new QWidget(this);
    auto controlsLayout = new QHBoxLayout();
    controlsLayout->addWidget(new QLabel("x"));
    auto x = new QLineEdit("0", this);
    controlsLayout->addWidget(x);
    controlsLayout->addWidget(new QLabel("y"));
    auto y = new QLineEdit("0", this);
    controlsLayout->addWidget(y);
    controlsLayout->addWidget(new QLabel("w"));
    auto width = new QLineEdit("0", this);
    controlsLayout->addWidget(width);
    controlsLayout->addWidget(new QLabel("h"));
    auto height = new QLineEdit("0", this);
    controlsLayout->addWidget(height);
    
    controlsLayout->setContentsMargins(0, 0, 0, 0);

    controls->setLayout(controlsLayout);
    
    layout->addWidget(controls);
    layout->addStretch();

    auto roi = ROIParameter::field(&parameterInfo.defaultValue);
    x->setText(QString::number(roi.x));
    y->setText(QString::number(roi.y));
    width->setText(QString::number(roi.width));
    height->setText(QString::number(roi.height));
    
    layout->setContentsMargins(0, 0, 0, 0);

    auto eventHandler = [=](const QString & text)
    {
        bool bOk = true;
        ROI param = {};
        param.x = bOk ? x->text().toUInt(&bOk) : 0;
        param.y = bOk ? y->text().toUInt(&bOk) : 0;
        param.width  = bOk ? width->text().toUInt(&bOk)  : 0;
        param.height = bOk ? height->text().toUInt(&bOk) : 0;
        
        if (bOk)
        {
            emit paramChanged(index, ROIParameter(param));
        }
    };
    connect(x, &QLineEdit::textChanged, eventHandler);
    connect(y, &QLineEdit::textChanged, eventHandler);
    connect(width, &QLineEdit::textChanged, eventHandler);
    connect(height, &QLineEdit::textChanged, eventHandler);
    
    return res;
}


QWidget* FilterControls::createMaskControl(const ParameterInfo& parameterInfo, index_t index)
{
    QWidget* res = new QWidget();

    auto layout = new QVBoxLayout();
    res->setLayout(layout);
    
    auto openButton   = new QPushButton("Open mask", this);
    auto imagePreview = new ImageControl("Mask", this);
    
    imagePreview->setFixedSize(64, 64);

    layout->addWidget(openButton);
    layout->addWidget(imagePreview);

    auto displayMask = [=](const MaskBitmap& mask)
    {
        QImage maskImage(mask.data, mask.width, mask.height, mask.byteSpan, QImage::Format_Indexed8);
        
        // White for 0 and black for othres.
        QVector<QRgb> colors;
        colors.push_back(0xFFFFFFFF);
        for (int i = 0; i < 256; i++)
        {
            colors.push_back(qRgb(255 - i, 255 - i, 255 - i));
        }
        maskImage.setColorTable(colors);
        
        imagePreview->setImage(QPixmap::fromImage(maskImage));
    };

    auto openMaskFromFile = [=]()
    {
        QString filename = QFileDialog::getOpenFileName(this,
            tr("Open Image"), "~/Pictures", tr("Image Files (*.png *.jpg *.bmp)"));
        
        QPixmap pixmap(filename);
        
        auto image = pixmap.toImage().convertToFormat(QImage::Format_Grayscale8);
        
        auto maxImage = MaskParameter::field(&parameterInfo.values.range.max);
        if (image.width() > maxImage.width)
        {
            image = image.scaledToWidth(maxImage.width);
        }
        if (image.height() > maxImage.height)
        {
            image = image.scaledToHeight(maxImage.height);
        }
        
        uint8_t* buffer = new uint8_t[image.byteCount()];
        memcpy(buffer, (const char *)image.constBits(), image.byteCount());
        
        MaskBitmap mask = {(uint8_t*)(buffer), static_cast<uint32_t>(image.width()), static_cast<uint32_t>(image.height()),
            static_cast<uint32_t>(image.bytesPerLine()), true};
        
        displayMask(mask);
        
        emit paramChanged(index, MaskParameter(mask));
    };

    auto mask = MaskParameter::field(&parameterInfo.defaultValue);
    
    displayMask(mask);
    
    connect(openButton, &QPushButton::clicked, openMaskFromFile);

    return res;
}


QWidget* FilterControls::createBoolControl(const ParameterInfo& parameterInfo, index_t index)
{
    QWidget* res = new QWidget();
    auto layout = new QVBoxLayout();
    res->setLayout(layout);
    
    auto* control = new QCheckBox(parameterInfo.title);
    layout->addWidget(control);
    
    connect(control, &QCheckBox::stateChanged, [=](int state)
    {
        emit paramChanged(index, BoolParameter(state > 0));
    });
    
    control->setCheckState(UintParameter::field(&parameterInfo.defaultValue) ? Qt::Checked : Qt::Unchecked);
    
    layout->addStretch();
    
    layout->setContentsMargins(0, 0, 0, 0);
    
    return res;
}


QWidget* FilterControls::createColorControl(const ParameterInfo& parameterInfo, index_t index)
{
    auto colorControl = new ColorControl(this, parameterInfo);

    connect(colorControl, &ColorControl::paramChanged, [=](const Parameter& value)
    {
        emit paramChanged(index, value);
    });

    return colorControl;
}

void FilterControls::setFilter(FilterPtr filter, const BaseParameterSet& parameters, index_t firstParamIndex)
{
    QVector<ParameterInfo> parametersInfo;
    
    for (int i = 0; i < filter->parameterNumber();  i ++)
    {
        auto info = filter->parameterInfo(i);
        info.defaultValue = parameters.value(i + firstParamIndex);
        parametersInfo.push_back(info);
    }
    
    filter_ = filter;
    setFilter(filter->name(), parametersInfo, firstParamIndex);
}

