#pragma once

#include <QMainWindow>

class MainWindow : public QMainWindow{
public:
    MainWindow();

private:
    void createToolbar();
    void createCentralWidget();
    void createTimeline();
};