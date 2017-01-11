//
//  ImageControl.cpp
//  UiTest
//
//  Created by Олег on 22.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "ImageControl.h"
#include <QtGui/QPainter>
#include <QtWidgets/QBoxLayout>


ImageControl::ImageControl (const QString& text, QWidget* parent) : QWidget(parent)
{
    _scrollArea = new QScrollArea(this);
    _imageLabel = new QLabel(text, _scrollArea);
    _scrollArea->setWidget(_imageLabel);
    QBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(_scrollArea);
    setLayout(layout);
}

void ImageControl::mousePressEvent(QMouseEvent * event)
{
    emit onMousePress(event);
}

void ImageControl::setImage(const QPixmap& pixmap)
{
    _image = pixmap;
    _imageLabel->setPixmap(_image);
    _imageLabel->setFixedSize(_image.size());
    repaint();
}

const QPixmap& ImageControl::image()
{
    return _image;
}


