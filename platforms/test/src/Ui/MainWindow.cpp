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

MainWindow::MainWindow()
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
    
    bridge.setResourceManager(&resourceManager);
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
    
    
    for (index_t i = 0; i < bridge.filtersNumber(); i ++)
    {
        filterList.addAction(bridge.createFilter(i)->name(), [=]()
                             {
                                 this->applyFilter(i);
                             });
    }
    
    filterList.exec(event->globalPos());
}

void MainWindow::applyFilter(index_t index)
{
    auto filter = bridge.createFilter(index);
    auto pix = source->pixmap();
    if (pix)
    {
        auto inputImage = pix->toImage().convertToFormat(QImage::Format_RGB888);
        
        Frame frame;
        frame.width    = inputImage.width();
        frame.height   = inputImage.height();
        frame.byteSpan = inputImage.bytesPerLine();
        frame.data     = reinterpret_cast<uint8_t*>(inputImage.bits());
        
        auto outFrame = resourceManager.createFrame(frame);
        
        filter->apply(&frame, outFrame, nullptr);
        
        dest->setPixmap(QPixmap::fromImage(QImage(outFrame->data, outFrame->width, outFrame->height, outFrame->byteSpan, QImage::Format_RGB888)));
        
        resourceManager.releaseFrame(outFrame);
    }
}





