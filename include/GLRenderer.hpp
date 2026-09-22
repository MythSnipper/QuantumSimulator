#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class GLRenderer : public QOpenGLWidget, protected QOpenGLFunctions{
public:
    explicit GLRenderer(QWidget* parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;
};




