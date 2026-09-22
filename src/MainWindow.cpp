#include "MainWindow.hpp"

#include <QStatusBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QToolBar>
#include <QWidget>

MainWindow::MainWindow(){
    resize(1280, 720);

    createToolbar();
    createCentralWidget();
}

void MainWindow::createToolbar(){
    QToolBar* toolbar = new QToolBar(this);
    toolbar->setObjectName("toolbar");

    toolbar->addAction("Add Particle");
    toolbar->addAction("Add Potential");
    toolbar->addAction("Add Field");

    addToolBar(toolbar);
}

void MainWindow::createCentralWidget(){
    QWidget* central = new QWidget(this);
    central->setStyleSheet("background-color: #202020;");
    QHBoxLayout* centralLayout = new QHBoxLayout(central); //left panel and right panels

    QWidget* leftPanel = this->createLeftPanel();

    QWidget* scenePanel = this->createScenePanel();

    QWidget* propertiesPanel = this->createPropertiesPanel();

    centralLayout->addWidget(leftPanel, 4);
    centralLayout->addWidget(scenePanel, 1);
    centralLayout->addWidget(propertiesPanel, 1);

    setCentralWidget(central);
}

//left panel containing rendering window and timeline
QWidget* MainWindow::createLeftPanel(){
    QWidget* leftPanel = new QWidget();
    leftPanel->setObjectName("leftPanel");
    leftPanel->setStyleSheet(
        "#leftPanel{"
        "   border: 2px solid gray;"
        "}"
    );
    QVBoxLayout* leftPanelLayout = new QVBoxLayout(leftPanel);

    // renderer window on the left panel
    QWidget* renderer = new QWidget();
    renderer->setStyleSheet(
        "border: 2px solid blue;"
        "background-color: #d864ed;"
    );

    // timeline
    QWidget* timeline;
    {
        timeline = new QWidget();
        QVBoxLayout* layout = new QVBoxLayout(timeline);

        // slider and time display
        QWidget* slidertime = new QWidget();
        QHBoxLayout* slidertimeLayout = new QHBoxLayout(slidertime);

        QSlider* slider = new QSlider(Qt::Horizontal);
        QLabel* time = new QLabel("0.00 s");

        slidertimeLayout->addWidget(slider, 1);
        slidertimeLayout->addWidget(time);

        // control buttons
        QWidget* buttons = new QWidget();
        QHBoxLayout* buttonsLayout = new QHBoxLayout(buttons);

        QPushButton* previous = new QPushButton("◀");
        QPushButton* play = new QPushButton("▶");
        QPushButton* pause = new QPushButton("▶|");
        QWidget* none = new QWidget();

        buttonsLayout->addWidget(previous);
        buttonsLayout->addWidget(play);
        buttonsLayout->addWidget(pause);
        buttonsLayout->addWidget(none, 1);


        layout->addWidget(slidertime);
        layout->addWidget(buttons);
    }

    leftPanelLayout->addWidget(renderer, 4);
    leftPanelLayout->addWidget(timeline, 1);

    return leftPanel;
}

//scene panel
QWidget* MainWindow::createScenePanel(){
    QWidget* scenePanel = new QWidget();
    scenePanel->setObjectName("scenePanel");
    scenePanel->setStyleSheet(
        "#scenePanel{"
        "   border: 2px solid gray;"
        "}"
    );
    QVBoxLayout* scenePanelLayout = new QVBoxLayout(scenePanel);

    QLabel* scene = new QLabel("Scene");
    scene->setStyleSheet("border: 1px solid blue;");

    QLabel* objects = new QLabel("objects listed here");
    objects->setStyleSheet("border: 1px solid blue;");
    objects->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    scenePanelLayout->addWidget(scene);
    scenePanelLayout->addWidget(objects, 1);

    return scenePanel;
}

//properties panel
QWidget* MainWindow::createPropertiesPanel(){
    QWidget* propertiesPanel = new QWidget();
    propertiesPanel->setObjectName("propertiesPanel");
    propertiesPanel->setStyleSheet(
        "#propertiesPanel{"
        "   border: 2px solid gray;"
        "}"
    );
    QVBoxLayout* propertiesPanelLayout = new QVBoxLayout(propertiesPanel);

    QLabel* properties = new QLabel("Properties");
    properties->setStyleSheet("border: 1px solid blue;");

    QLabel* propertieslisted = new QLabel("properties listed here");
    propertieslisted->setStyleSheet("border: 1px solid blue;");
    propertieslisted->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    propertiesPanelLayout->addWidget(properties);
    propertiesPanelLayout->addWidget(propertieslisted, 1);
    return propertiesPanel;
}


