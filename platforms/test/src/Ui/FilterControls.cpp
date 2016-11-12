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

FilterControls::FilterControls ()
{
    setLayout(new QVBoxLayout());
}

void FilterControls::setFilter(const QString& title, const QVector<ParameterInfo>& parameters)
{
    removeOldControls();
    
    auto vLayout  = qobject_cast<QVBoxLayout*>(layout());
    
    vLayout->addWidget(new QLabel(title));
    
    index_t index = 0;
    for (ParameterInfo param : parameters)
    {
        auto control = createControl(param, index);
        if (control)
        {
            vLayout->addWidget(control);
        }
        index++;
    }
    
    vLayout->addStretch();
}

QWidget* FilterControls::createControl(const ParameterInfo& parameterInfo, index_t index)
{
    QWidget* res = nullptr;
    switch (parameterInfo.type)
    {
        case BS_UINT: res = createUintControl(parameterInfo, index); break;
        case BS_ROI:  res = createROIControl(parameterInfo, index); break;
        
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


//void paramChanged(index_t index, const QVariant& value);
