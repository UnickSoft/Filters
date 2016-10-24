//
//  MainWindow.cpp
//  UiTest
//
//  Created by Олег on 20.09.16.
//  Copyright © 2016 Oleg. All rights reserved.
//

#include "MainWindow.h"
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenu>
#include <QtCore/QBuffer>
#include <QtGui/QMouseEvent>

MainWindow::MainWindow(Controller& controller) : controller(controller)
{
    source = new ImageControl("Source (Click to open image)", this);
    dest   = new ImageControl("Dest (Click to open filter)", this);
    
    source->setScaledContents(true);
    dest->setScaledContents(true);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->QLayout::addWidget(source);
    layout->QLayout::addWidget(dest);
    
    // Set layout in QWidget
    QWidget *window = new QWidget();
    window->setLayout(layout);
    
    // Set QWidget as the central layout of the main window
    setCentralWidget(window);
    
    connect(source, SIGNAL(onMousePress(QMouseEvent *)), this, SLOT(openImage()));
    connect(dest, SIGNAL(onMousePress(QMouseEvent *)), this, SLOT(applyFilter(QMouseEvent *)));
    
    resize(800, 300);
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





