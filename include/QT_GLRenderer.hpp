#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



//forward declare
class VertexShader;
class FragmentShader;
class ShaderProgram;
class VBO;
class VAO;

class QT_GLRenderer : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core{
public:
    explicit QT_GLRenderer(QWidget* parent = nullptr);
    ~QT_GLRenderer();

protected:
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

private:
    void printInfo();
    void applySettings();
    //settings
    bool wireframe_mode = false;
    bool depth_test = false;

    //renderer objects
    VertexShader* vertexShader = nullptr;
    FragmentShader* fragmentShader = nullptr;
    ShaderProgram* shaderProgram = nullptr;

    VBO* vbo = nullptr;
    VAO* vao = nullptr;
};

//Renderer objects
struct Color{
    float r;
    float g;
    float b;
    float a = 1.0f;
};
struct ColorRGBA{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};
typedef int UniformID;

class VertexShader : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id;

    VertexShader(const char* shader_source_addr);
    VertexShader(char* filename);
    ~VertexShader();
};
class FragmentShader : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id;

    FragmentShader(const char* shader_source_addr);
    FragmentShader(char* filename);
    ~FragmentShader();
};
class ShaderProgram : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id;

    ShaderProgram(VertexShader* vertex_shader, FragmentShader* fragment_shader);
    ShaderProgram(const char* vert_source, const char* frag_source);
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

class VBO : protected QOpenGLFunctions_3_3_Core{
public:
    uint32_t id;

    VBO();
    VBO(float vertices[], size_t vertices_size, GLenum usage);
    void bind();
    void unbind();
    void fill(float vertices[], size_t vertices_size, GLenum usage);
};
class VAO : protected QOpenGLFunctions_3_3_Core{
public:
    uint32_t id;

    VAO();
    void bind();
    void unbind();

};
class EBO : protected QOpenGLFunctions_3_3_Core{
public:
    uint32_t id;

    EBO();
    EBO(uint32_t indices[], size_t indices_size, GLenum usage);
    void bind();
    void unbind();
    void fill(uint32_t indices[], size_t indices_size, GLenum usage);

};
class Texture2D : protected QOpenGLFunctions_3_3_Core{
public:
    uint32_t id;

    Texture2D(char* file_path, GLenum internal_format);
    Texture2D(uint8_t* data, uint32_t width, uint32_t height, GLenum image_format, GLenum internal_format);
    void fill(char* file_path, GLenum internal_format);
    void fill(uint8_t* data, uint32_t width, uint32_t height, GLenum image_format, GLenum internal_format);
    void bind();
    void bind_texture_unit(GLenum texture_unit);

private:
    int texture_width;
    int texture_height;
    int color_channels_count;
};

//others
char* read_file(const char* filename);





