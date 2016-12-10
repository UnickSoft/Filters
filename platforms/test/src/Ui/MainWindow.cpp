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

static const QString previewImageKey = "testImage";

MainWindow::MainWindow(Controller& controller) : controller(controller), filterIndex(0), settings("UnickSoft", "TestUI")
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

    QVBoxLayout* controlsLayout = new QVBoxLayout();
    leftPart->setLayout(controlsLayout);

    mainLayout->addWidget(leftPart, 1);
    mainLayout->addWidget(rightPart, 1);
    
    // Create effect controls.
    filterList = new QComboBox();
    auto filters = controller.filters();
    for (auto filter : filters)
    {
        filterList->addItem(filter);
    }
    
    controlsLayout->addWidget(filterList);
    controlsLayout->addWidget(&controls);
    controlsLayout->addWidget(createRenderSetup());
    controlsLayout->addStretch();
    
    // Setup preview outputs.
    source = new ImageControl("Source (Click to open image)", this);
    dest   = new ImageControl("Dest (Click to open filter)", this);
    
    preview->addWidget(source);
    preview->addWidget(dest);
    
    // Set QWidget as the central layout of the main window
    setCentralWidget(window);

    connect(filterList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::addFilter);
    connect(source, &ImageControl::onMousePress, this, &MainWindow::openImage);
    connect(&controls, &FilterControls::paramChanged, this, &MainWindow::paramChanged);
    connect(renderFormats, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &MainWindow::renderFormatChanged);
    connect(dest, &ImageControl::onMousePress, this, &MainWindow::saveImage);
    
    resize(600, 600);

    if (settings.contains(previewImageKey))
    {
        loadImage(settings.value(previewImageKey).toString());
    }
    else
    {
        loadImage(QApplication::applicationDirPath() + "/../Resources/resources/Lenna.png");
    }
    
    addFilter((index_t)0);
}


void MainWindow::openImage()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "~/Pictures", tr("Image Files (*.png *.jpg *.bmp)"));
    loadImage(filename);
}

void MainWindow::loadImage(const QString& filename)
{
    if (!filename.isEmpty())
    {
        settings.setValue(previewImageKey, filename);
        source->setImage(QPixmap(filename));
    }
}

void MainWindow::applyFilter(QMouseEvent * event)
{
    QMenu filterList;
    
    auto filters = controller.filters();
    
    for (index_t i = 0; i < filters.size(); i ++)
    {
        filterList.addAction(filters[(int)i], [=]()
                             {
                                 this->applyFilter(i);
                             });
    }
    
    filterList.exec(event->globalPos());
}

void MainWindow::applyFilter(index_t index)
{
    auto sourcePix = source->image();
    if (!sourcePix.isNull())
    {
        QImage::Format renderFormat = (QImage::Format)renderFormats->itemData(renderFormats->currentIndex()).toInt();
        QImage destImage;
        
        auto sourceImage = sourcePix.toImage().convertToFormat(renderFormat);
        controller.applyFilter(index, &parameters, sourceImage, destImage);
        
        dest->setImage(QPixmap::fromImage(destImage.convertToFormat(QImage::Format_RGBA8888)));
    }
}

void MainWindow::addFilter(int index)
{
    filterIndex = index;
    auto parameterInfo = controller.parameterList(index);
    
    fillDefaultParameters(parameterInfo);

    controls.setFilter(filterList->currentText(), parameterInfo);
    
    applyFilter(index);
}

void MainWindow::paramChanged(index_t index, const Parameter& value)
{
    parameters[index] = value;
    
    applyFilter(filterIndex);
}

void MainWindow::fillDefaultParameters(const QVector<ParameterInfo>& parameterInfo)
{
    parameters.clear();
    
    for (auto paramInfo : parameterInfo)
    {
        parameters.push_back(paramInfo.defaultValue);
    }
}

QWidget* MainWindow::createRenderSetup()
{
    auto res = new QWidget();
    QVBoxLayout* setupLayout = new QVBoxLayout();
    res->setLayout(setupLayout);
    
    auto title = new QLabel("Render setup");
    setupLayout->addWidget(title);
    
    renderFormats = new QComboBox();
    renderFormats->addItem("RGBA8",  QImage::Format_RGBA8888);
    renderFormats->addItem("RGB8",   QImage::Format_RGB888);
    renderFormats->addItem("Alpha8", QImage::Format_Alpha8);
    
    setupLayout->addWidget(renderFormats);
    
    return res;
}

void MainWindow::renderFormatChanged(int index)
{
    applyFilter(filterIndex);
}

void MainWindow::saveImage()
{
    QString filename = QFileDialog::getSaveFileName(this,
        tr("Save Image"), "~/Pictures/result.png", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!filename.isEmpty())
    {
        dest->image().toImage().convertToFormat(QImage::Format_RGBA8888).save(filename);
    }
}

