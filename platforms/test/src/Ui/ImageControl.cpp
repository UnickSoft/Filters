//
//  ImageControl.cpp
//  UiTest
//
//  Created by Олег on 22.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "ImageControl.h"


ImageControl::ImageControl (const QString& text, QWidget* parent) : QLabel(text, parent)
{

}

void ImageControl::mousePressEvent(QMouseEvent * event)
{
    emit onMousePress(event);
}