#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class QT_GLRenderer : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core{
public:
    explicit QT_GLRenderer(QWidget* parent = nullptr);
    ~QT_GLRenderer();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private:
    unsigned int VBO, VAO;
    unsigned int vertexShader, fragmentShader, shaderProgram;

};


//Renderer objects
class VertexShader{
public:
    uint32_t id;

    VertexShader(char** shader_source_addr);
    VertexShader(char* filename);
    ~VertexShader();
};
class FragmentShader{
public:
    uint32_t id;

    FragmentShader(char** shader_source_addr);
    FragmentShader(char* filename);
    ~FragmentShader();
};

class ShaderProgram{
public:
    uint32_t id;

    ShaderProgram(VertexShader* vertex_shader, FragmentShader* fragment_shader);
    ShaderProgram(char** vert_source, char** frag_source);
    ShaderProgram(char* vert_file, char* frag_file);
    void set_bool(char* name, std::initializer_list<int> values);
    void set_bool(UniformID uid, std::initializer_list<int> values);
    void set_int(char* name, std::initializer_list<int> values);
    void set_int(UniformID uid, std::initializer_list<int> values);
    void set_float(char* name, std::initializer_list<float> values);
    void set_float(UniformID uid, std::initializer_list<float> values);
    void set_mat4(char* name, bool transpose, glm::mat4 matrix);
    void set_mat4(UniformID uid, bool transpose, glm::mat4 matrix);
    void activate();
};






