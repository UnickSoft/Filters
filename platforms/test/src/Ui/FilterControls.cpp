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

FilterControls::FilterControls ()
{
    setLayout(new QVBoxLayout());
}

void FilterControls::setFilter(const QString& title, const QVector<ParameterInfo>& parameters)
{
    removeOldControls();
    
    auto vLayout  = qobject_cast<QVBoxLayout*>(layout());
    
    vLayout->addWidget(new QLabel(title));
    
    for (ParameterInfo param : parameters)
    {
        auto control = createControl(param);
        if (control)
        {
            vLayout->addWidget(control);
        }
    }
    
    vLayout->addStretch();
}

QWidget* FilterControls::createControl(const ParameterInfo& parameterInfo)
{
    QWidget* res = nullptr;
    switch (parameterInfo.type)
    {
        case BS_UINT: res = createUintControl(parameterInfo); break;
        default: assert(false && "Unknown parameter");
    }
    
    return res;
}


QWidget* FilterControls::createUintControl(const ParameterInfo& parameterInfo)
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
        emit paramChanged(0, UintParameter(slider->value()));
    });
    
    slider->setValue(UintParameter::field(&parameterInfo.defaultValue));
    
    layout->addWidget(slider);
    layout->addStretch();
    
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


//void paramChanged(index_t index, const QVariant& value);
