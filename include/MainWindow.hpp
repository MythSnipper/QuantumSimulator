#pragma once

#include <QMainWindow>
#include <QByteArray>
#include <QCloseEvent>
#include <QOpenGLWidget>

class MainWindow : public QMainWindow{
public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent* event) override;

private:

    void setupDefaultLayout();

    void createMenuBar();
    void createViewMenu();

    QByteArray defaultLayout;
    void saveLayout();
    void restoreLayout();

    QDockWidget* rendererDock;
    QDockWidget* timelineDock;
    QDockWidget* sceneDock;
    QDockWidget* propertiesDock;

    QToolBar* createToolbarDock();
    QDockWidget* createRendererDock();
    QDockWidget* createTimelineDock();
    QDockWidget* createSceneDock();
    QDockWidget* createPropertiesDock();

    QOpenGLWidget* createRenderer();
    QWidget* createTimeline();
    QWidget* createScenePanel();
    QWidget* createPropertiesPanel();
};