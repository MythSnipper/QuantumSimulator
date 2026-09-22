#include "MainWindow.hpp"

#include "GLRenderer.hpp"

#include <QStatusBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QToolBar>
#include <QDockWidget>
#include <QWidget>

auto QDFEATURES = QDockWidget::DockWidgetMovable |
        QDockWidget::DockWidgetFloatable |
        QDockWidget::DockWidgetClosable;

        
MainWindow::MainWindow(){
    resize(1280, 720);

    setDockNestingEnabled(true);

    addToolBar(Qt::TopToolBarArea, createToolbarDock());
    
    QDockWidget* rendererDock = createRendererDock();
    QDockWidget* timelineDock = createTimelineDock();
    QDockWidget* sceneDock = createSceneDock();
    QDockWidget* propertiesDock = createPropertiesDock();

    addDockWidget(Qt::LeftDockWidgetArea, rendererDock);

    splitDockWidget(rendererDock, sceneDock, Qt::Horizontal);
    resizeDocks({rendererDock, sceneDock}, {3, 2}, Qt::Horizontal);

    splitDockWidget(rendererDock, timelineDock, Qt::Vertical);
    resizeDocks({rendererDock, timelineDock}, {4, 1}, Qt::Vertical);

    splitDockWidget(sceneDock, propertiesDock, Qt::Horizontal);
    resizeDocks({sceneDock, propertiesDock}, {1, 1}, Qt::Horizontal);

}


QToolBar* MainWindow::createToolbarDock(){
    QToolBar* toolbar = new QToolBar("Toolbar", this);
    toolbar->setObjectName("toolbar");

    toolbar->setMovable(true);
    toolbar->setFloatable(true);

    toolbar->addAction("Add Particle");
    toolbar->addAction("Add Potential");
    toolbar->addAction("Add Field");

    return toolbar;
}

QDockWidget* MainWindow::createRendererDock(){
    QDockWidget* dock = new QDockWidget("Renderer", this);
    GLRenderer* renderer = new GLRenderer();

    dock->setWidget(renderer);

    dock->setFeatures(
        QDFEATURES
    );

    dock->setAllowedAreas(Qt::AllDockWidgetAreas);

    return dock;
}

QDockWidget* MainWindow::createTimelineDock(){
    QDockWidget* dock = new QDockWidget("Timeline", this);

    dock->setWidget(createTimeline());

    dock->setFeatures(
        QDFEATURES
    );

    dock->setAllowedAreas(Qt::AllDockWidgetAreas);

    return dock;
}

QDockWidget* MainWindow::createSceneDock(){
    QDockWidget* dock = new QDockWidget("Scene", this);

    dock->setWidget(createScenePanel());

    dock->setFeatures(
        QDFEATURES
    );

    dock->setAllowedAreas(Qt::AllDockWidgetAreas);

    return dock;
}

QDockWidget* MainWindow::createPropertiesDock(){
    QDockWidget* dock = new QDockWidget("Properties", this);

    dock->setWidget(createPropertiesPanel());

    dock->setFeatures(
        QDFEATURES
    );

    dock->setAllowedAreas(Qt::AllDockWidgetAreas);

    return dock;
}

QWidget* MainWindow::createRenderer(){
    QWidget* renderer = new QWidget();
    renderer->setStyleSheet(
        "border: 2px solid blue;"
        "background-color: #64ed8b;"
    );

    return renderer;
}

QWidget* MainWindow::createTimeline(){
    // timeline
    QWidget* timeline = new QWidget();
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

    return timeline;
}

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


