//
//  MainWindow.cpp
//  UiTest
//
//  Created by Олег on 20.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "MainWindow.h"
#include <QtWidgets/QHBoxLayout>


MainWindow::MainWindow()
{
    source = new QLabel("Source (Click to open image)", this);
    dest   = new QLabel("Dest (Click to open filter)", this);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->QLayout::addWidget(source);
    layout->QLayout::addWidget(dest);
    
    // Set layout in QWidget
    QWidget *window = new QWidget();
    window->setLayout(layout);
    
    // Set QWidget as the central layout of the main window
    setCentralWidget(window);
}