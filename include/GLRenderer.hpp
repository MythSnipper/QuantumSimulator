#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class GLRenderer : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core{
public:
    explicit GLRenderer(QWidget* parent = nullptr);
    ~GLRenderer();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private:
    unsigned int VBO, VAO;
    unsigned int vertexShader, fragmentShader, shaderProgram;



};




