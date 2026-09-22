#pragma once

#include <QMainWindow>

class MainWindow : public QMainWindow{
public:
    MainWindow();

private:
    QToolBar* createToolbarDock();
    QDockWidget* createRendererDock();
    QDockWidget* createTimelineDock();
    QDockWidget* createSceneDock();
    QDockWidget* createPropertiesDock();

    QWidget* createRenderer();
    QWidget* createTimeline();
    QWidget* createScenePanel();
    QWidget* createPropertiesPanel();
};