//
//  MainWindow.cpp
//  UiTest
//
//  Created by Олег on 20.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "MainWindow.h"
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>
#include <QtCore/QBuffer>
#include <QtGui/QMouseEvent>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QApplication>

MainWindow::MainWindow(Controller& controller) : controller(controller)
{
    QHBoxLayout* mainLayout = new QHBoxLayout();
    
    // Set layout in QWidget
    QWidget *window = new QWidget();
    window->setLayout(mainLayout);
    
    // Left and right parts of main window.
    QWidget* leftPart  = new QWidget();
    QWidget* rightPart = new QWidget();

    QVBoxLayout* preview = new QVBoxLayout();
    rightPart->setLayout(preview);

    QVBoxLayout* controls = new QVBoxLayout();
    leftPart->setLayout(controls);

    mainLayout->addWidget(leftPart);
    mainLayout->addWidget(rightPart);
    
    // Create effect controls.
    QComboBox* filterList = new QComboBox();
    auto filters = controller.filters();
    for (auto filter : filters)
    {
        filterList->addItem(filter);
    }
    
    controls->addWidget(filterList);
    controls->addStretch();

    // Setup preview outputs.
    source = new ImageControl("Source (Click to open image)", this);
    dest   = new ImageControl("Dest (Click to open filter)", this);
    
    source->setScaledContents(true);
    dest->setScaledContents(true);
    
    preview->addWidget(source);
    preview->addWidget(dest);
    
    // Set QWidget as the central layout of the main window
    setCentralWidget(window);

    connect(filterList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::addFilter);
    connect(source, &ImageControl::onMousePress, this, &MainWindow::openImage);
    
    resize(600, 600);
}


void MainWindow::openImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,
    tr("Open Image"), "~/Pictures", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty())
    {
        QPixmap pix(fileName);
        source->setPixmap(pix.scaled(source->width(), source->height(), Qt::KeepAspectRatio));
    }
}

void MainWindow::applyFilter(QMouseEvent * event)
{
    QMenu filterList;
    
    auto filters = controller.filters();
    
    for (index_t i = 0; i < filters.size(); i ++)
    {
        filterList.addAction(filters[i], [=]()
                             {
                                 this->applyFilter(i);
                             });
    }
    
    filterList.exec(event->globalPos());
}

void MainWindow::applyFilter(index_t index)
{
    auto sourcePix = source->pixmap();
    if (sourcePix)
    {
        QImage destImage(sourcePix->width(), sourcePix->height(), QImage::Format_RGB888);
        
        auto sourceImage = sourcePix->toImage().convertToFormat(QImage::Format_RGB888);
        controller.applyFilter(index, sourceImage, destImage);
        
        dest->setPixmap(QPixmap::fromImage(destImage));
    }
}

void MainWindow::addFilter(int index)
{
    applyFilter(index);
}





