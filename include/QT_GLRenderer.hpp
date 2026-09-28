#pragma once

#include <GLRenderer.hpp>

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

//forward declare
class VertexShader;
class FragmentShader;
class ShaderProgram;
class VBO;
class VAO;
class RenderObject;

class QT_GLRenderer : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core{
public:
    explicit QT_GLRenderer(QWidget* parent = nullptr);
    ~QT_GLRenderer();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private:
    void drawRenderObject(RenderObject* object);
    void printInfo();
    void applySettings();

    //settings
    bool wireframe_mode = false;
    bool depth_test = true;

    //renderer objects
    ShaderProgram* shaderProgram;
    VAO* vao;
    VBO* vbo;

    Camera camera;

    std::vector<RenderObject*> objects;
};

