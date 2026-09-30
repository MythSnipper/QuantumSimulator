#pragma once

#include <GLRenderer.hpp>

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QTimer>

#include <memory>
#include <vector>

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
    void destroyGLResources();
    void drawRenderObject(std::unique_ptr<RenderObject>& object);
    void printInfo();
    void applySettings();

    //settings
    bool wireframe_mode = false;
    bool depth_test = true;

    //renderer objects
    std::unique_ptr<Mesh> mesh;
    std::unique_ptr<ShaderProgram> shaderProgram;

    Camera camera;
    std::vector<std::unique_ptr<RenderObject>> objects;

    QTimer renderTimer;
};

