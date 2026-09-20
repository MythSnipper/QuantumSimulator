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
    createTimeline();
}

void MainWindow::createToolbar()
{
    QToolBar* toolbar = new QToolBar(this);

    toolbar->addAction("Add Particle");
    toolbar->addAction("Add Potential");
    toolbar->addAction("Add Field");

    addToolBar(toolbar);
}

void MainWindow::createCentralWidget()
{
    QWidget* central = new QWidget(this);

    QHBoxLayout* layout = new QHBoxLayout(central);

    QWidget* renderer = new QWidget();
    renderer->setStyleSheet("background-color: #202020;");

    QWidget* rightPanel = new QWidget();

    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);

    QLabel* structure = new QLabel("Structure");
    QLabel* inspector = new QLabel("Inspector");

    rightLayout->addWidget(structure);
    rightLayout->addWidget(inspector);

    layout->addWidget(renderer, 3);
    layout->addWidget(rightPanel, 1);

    setCentralWidget(central);
}

void MainWindow::createTimeline()
{
    QWidget* timeline = new QWidget(this);

    QHBoxLayout* layout = new QHBoxLayout(timeline);

    QPushButton* previous = new QPushButton("◀");
    QPushButton* play = new QPushButton("▶");
    QPushButton* next = new QPushButton("▶|");

    QSlider* slider = new QSlider(Qt::Horizontal);

    QLabel* time = new QLabel("0.00 s");

    layout->addWidget(previous);
    layout->addWidget(play);
    layout->addWidget(next);
    layout->addWidget(slider);
    layout->addWidget(time);

    // For now, put it in the status bar.
    statusBar()->addPermanentWidget(timeline);
}

