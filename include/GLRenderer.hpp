#pragma once

#include <QOpenGLFunctions_3_3_Core>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    GLuint id = 0;

    VertexShader();
    ~VertexShader();
    void from_source(const char* shader_source_addr);
    void from_file(const char* filename);
};
class FragmentShader : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    FragmentShader();
    ~FragmentShader();
    void from_source(const char* shader_source_addr);
    void from_file(const char* filename);
};
class ShaderProgram : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;
    //if shaders need to be owned by the program
    VertexShader* vertShader = nullptr;
    FragmentShader* fragShader = nullptr;

    ShaderProgram();
    ~ShaderProgram();
    void from_shaders(VertexShader* vertex_shader, FragmentShader* fragment_shader);
    void from_sources(const char* vert_source, const char* frag_source);
    void from_files(const char* vert_filename, const char* frag_filename);

    void set_bool(const char* name, std::initializer_list<int> values);
    void set_bool(UniformID uid, std::initializer_list<int> values);
    void set_int(const char* name, std::initializer_list<int> values);
    void set_int(UniformID uid, std::initializer_list<int> values);
    void set_float(const char* name, std::initializer_list<float> values);
    void set_float(UniformID uid, std::initializer_list<float> values);
    void set_mat4(const char* name, bool transpose, glm::mat4 matrix);
    void set_mat4(UniformID uid, bool transpose, glm::mat4 matrix);
    void activate();
};





class VBO : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    VBO();
    ~VBO();
    VBO(float vertices[], size_t vertices_size, GLenum usage);
    void bind();
    void unbind();
    void fill(float vertices[], size_t vertices_size, GLenum usage);
};
class VAO : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    VAO();
    ~VAO();
    void bind();
    void unbind();

};
class EBO : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    EBO();
    ~EBO();
    EBO(uint32_t indices[], size_t indices_size, GLenum usage);
    void bind();
    void unbind();
    void fill(uint32_t indices[], size_t indices_size, GLenum usage);

};
class Texture2D : protected QOpenGLFunctions_3_3_Core{
public:
    GLuint id = 0;

    Texture2D(char* file_path, GLenum internal_format);
    Texture2D(uint8_t* data, uint32_t width, uint32_t height, GLenum image_format, GLenum internal_format);
    ~Texture2D();
    void fill(char* file_path, GLenum internal_format);
    void fill(uint8_t* data, uint32_t width, uint32_t height, GLenum image_format, GLenum internal_format);
    void bind();
    void bind_texture_unit(GLenum texture_unit);

private:
    int texture_width;
    int texture_height;
    int color_channels_count;
};

struct Transform{
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);
    glm::vec3 scale    = glm::vec3(1.0f);

    glm::mat4 get_matrix();
};

class Camera{
public:
    glm::vec3 position = glm::vec3(4.0f, 3.0f, 5.0f);
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    float fov = 45.0f; //degrees
    float aspect_ratio = 1.0f;

    float near_plane = 0.1f;
    float far_plane = 100.0f;

    glm::mat4 get_view_matrix();
    glm::mat4 get_projection_matrix();
};
class RenderObject{
public:
    Transform transform;

    VAO* vao = nullptr;
    ShaderProgram* shader = nullptr;

    unsigned int vertexCount = 0;
};









//others
char* read_file(const char* filename);





