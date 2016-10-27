//
//  ImageControl.cpp
//  UiTest
//
//  Created by Олег on 22.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "ImageControl.h"
#include <QtGui/QPainter>


ImageControl::ImageControl (const QString& text, QWidget* parent) : QLabel(text, parent)
{

}

void ImageControl::mousePressEvent(QMouseEvent * event)
{
    emit onMousePress(event);
}

void ImageControl::paintEvent(QPaintEvent * event)
{
    if (!_image.isNull())
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.drawPixmap(0, 0, _image.scaled(size()));
    }
    else
    {
        QLabel::paintEvent(event);
    }
}

void ImageControl::setImage(const QPixmap& pixmap)
{
    _image = pixmap;
    repaint();
}

const QPixmap& ImageControl::image()
{
    return _image;
}