#include "QT_GLRenderer.hpp"

#include <iostream>
#include <fstream>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "} \n";

QT_GLRenderer::QT_GLRenderer(QWidget* parent) : QOpenGLWidget(parent){
    //settings
    wireframe_mode = false;
    depth_test = false;
}

QT_GLRenderer::~QT_GLRenderer(){
    makeCurrent();

    delete shaderProgram;
    delete vertexShader;
    delete fragmentShader;

    delete vbo;
    delete vao;

    doneCurrent();
}

void QT_GLRenderer::initializeGL(){
    initializeOpenGLFunctions();
    
    //print info
    printInfo();
    applySettings();

    vertexShader = new VertexShader(vertexShaderSource);
    fragmentShader = new FragmentShader(fragmentShaderSource);
    shaderProgram = new ShaderProgram(vertexShader, fragmentShader);

    static float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };
    
    vao = new VAO();
    vao->bind();

    vbo = new VBO(vertices, sizeof(vertices), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    vao->unbind();

}


void QT_GLRenderer::resizeGL(int width, int height){
    glViewport(0, 0, width, height);
}

void QT_GLRenderer::paintGL(){
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shaderProgram->activate();
    vao->bind();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void QT_GLRenderer::printInfo(){
    for(int i=0;i<20;i++)printf("-");
    puts("");
    printf("OS: ");
    #ifdef _WIN32
        printf("Windows\n");
    #else
        printf("Linux\n");
    #endif
    printf("Wireframe mode: %s\n", (wireframe_mode) ? "On" : "Off");
    printf("Depth testing: %s\n", (depth_test) ? "On" : "Off");

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    printf("Number of vertex attributes available: %d\n", nrAttributes);
}

void QT_GLRenderer::applySettings(){
    glPolygonMode(GL_FRONT_AND_BACK, (wireframe_mode) ? GL_LINE : GL_FILL);
    if(depth_test){
        glEnable(GL_DEPTH_TEST);
    }
    else{
        glDisable(GL_DEPTH_TEST);
    }
}


//Renderer objects
VertexShader::VertexShader(const char* shader_source_addr){
    initializeOpenGLFunctions();
    id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(id, 1, &shader_source_addr, NULL);
    glCompileShader(id);
    {
        int success;
        char infoLog[512];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            printf("VERTEX SHADER COMPILATION FAILED! \n%s\n", infoLog);
            throw std::runtime_error(" ^ ^");
        }
        else{
            puts("VERTEX SHADER COMPILED SUCCESSFULLY");
        }
    }
}
VertexShader::VertexShader(char* filename){
    initializeOpenGLFunctions();
    char* shader_source = read_file(filename);
    id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(id, 1, &shader_source, NULL);
    glCompileShader(id);
    {
        int success;
        char infoLog[512];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            printf("VERTEX SHADER COMPILATION FAILED! \n%s\n", infoLog);
            throw std::runtime_error(" ^ ^");
        }
        else{
            puts("VERTEX SHADER COMPILED SUCCESSFULLY");
        }
    }
}
VertexShader::~VertexShader(){
    if(id != 0){
        puts("Killing VertexShader");
        glDeleteShader(id);
        id = 0;
    }
}

FragmentShader::FragmentShader(const char* shader_source_addr){
    initializeOpenGLFunctions();
    id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(id, 1, &shader_source_addr, NULL);
    glCompileShader(id);
    {
        int success;
        char infoLog[512];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            printf("FRAGMENT SHADER COMPILATION FAILED! \n%s\n", infoLog);
            throw std::runtime_error(" ^ ^");
        }
        else{
            puts("FRAGMENT SHADER COMPILED SUCCESSFULLY");
        }
    }
}
FragmentShader::FragmentShader(char* filename){
    initializeOpenGLFunctions();
    char* shader_source = read_file(filename);
    id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(id, 1, &shader_source, NULL);
    glCompileShader(id);
    {
        int success;
        char infoLog[512];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            printf("FRAGMENT SHADER COMPILATION FAILED! \n%s\n", infoLog);
            throw std::runtime_error(" ^ ^");
        }
        else{
            puts("FRAGMENT SHADER COMPILED SUCCESSFULLY");
        }
    }
}
FragmentShader::~FragmentShader(){
    if(id != 0){
        puts("Killing FragmentShader");
        glDeleteShader(id);
        id = 0;
    }
}

ShaderProgram::ShaderProgram(VertexShader* vertex_shader, FragmentShader* fragment_shader){
    initializeOpenGLFunctions();
    id = glCreateProgram();
    glAttachShader(id, vertex_shader->id);
    glAttachShader(id, fragment_shader->id);
    glLinkProgram(id);
    {
        int success;
        char infoLog[512];
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            printf("SHADER PROGRAM LINK FAILED! \n%s\n", infoLog);
            throw std::runtime_error(" ^ ^");
        }
        else{
            puts("SHADER PROGRAM LINKED SUCCESSFULLY");
        }
    }
}
ShaderProgram::ShaderProgram(const char* vert_source, const char* frag_source){
    initializeOpenGLFunctions();
    VertexShader vert(vert_source);
    FragmentShader frag(frag_source);

    id = glCreateProgram();
    glAttachShader(id, vert.id);
    glAttachShader(id, frag.id);
    glLinkProgram(id);
    {
        int success;
        char infoLog[512];
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            printf("SHADER PROGRAM LINK FAILED! \n%s\n", infoLog);
            throw std::runtime_error(" ^ ^");
        }
        else{
            puts("SHADER PROGRAM LINKED SUCCESSFULLY");
        }
    }
}
ShaderProgram::ShaderProgram(char* vert_file, char* frag_file){
    initializeOpenGLFunctions();
    VertexShader vert(vert_file);
    FragmentShader frag(frag_file);

    id = glCreateProgram();
    glAttachShader(id, vert.id);
    glAttachShader(id, frag.id);
    glLinkProgram(id);
    {
        int success;
        char infoLog[512];
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(id, 512, NULL, infoLog);
            printf("SHADER PROGRAM LINK FAILED! \n%s\n", infoLog);
            throw std::runtime_error(" ^ ^");
        }
        else{
            puts("SHADER PROGRAM LINKED SUCCESSFULLY");
        }
    }
}
void ShaderProgram::set_bool(char* name, std::initializer_list<int> values){
    set_int(name, values);
}
void ShaderProgram::set_bool(UniformID uid, std::initializer_list<int> values){
    set_int(uid, values);
}
void ShaderProgram::set_int(char* name, std::initializer_list<int> values){
    set_int(glGetUniformLocation(id, name), values);
}
void ShaderProgram::set_int(UniformID uid, std::initializer_list<int> values){
    const int* vel = values.begin();
    switch(values.size()){
        case 1:
            glUniform1i(uid, vel[0]);
        break;
        case 2:
            glUniform2i(uid, vel[0], vel[1]);
        break;
        case 3:
            glUniform3i(uid, vel[0], vel[1], vel[2]);
        break;
        case 4:
            glUniform4i(uid, vel[0], vel[1], vel[2], vel[3]);
        break;
        default:
            printf("Invalid number of values to set uniform\nExpected 1-4, got %d\n", values.size());
            throw std::runtime_error(" ^ ^");
    }
}
void ShaderProgram::set_float(char* name, std::initializer_list<float> values){
    set_float(glGetUniformLocation(id, name), values);
}
void ShaderProgram::set_float(UniformID uid, std::initializer_list<float> values){
    const float* vel = values.begin();
    switch(values.size()){
        case 1:
            glUniform1f(uid, vel[0]);
        break;
        case 2:
            glUniform2f(uid, vel[0], vel[1]);
        break;
        case 3:
            glUniform3f(uid, vel[0], vel[1], vel[2]);
        break;
        case 4:
            glUniform4f(uid, vel[0], vel[1], vel[2], vel[3]);
        break;
        default:
            printf("Invalid number of values to set uniform\nExpected 1-4, got %d\n", values.size());
            throw std::runtime_error(" ^ ^");
    }
}
void ShaderProgram::set_mat4(char* name, bool transpose, glm::mat4 matrix){
    set_mat4(glGetUniformLocation(id, name), transpose, matrix);
}
void ShaderProgram::set_mat4(UniformID uid, bool transpose, glm::mat4 matrix){
    glUniformMatrix4fv(uid, 1, (transpose) ? GL_TRUE : GL_FALSE, glm::value_ptr(matrix));
}
void ShaderProgram::activate(){
    glUseProgram(id);
}

VBO::VBO(){
    initializeOpenGLFunctions();
    glGenBuffers(1, &id);
}
VBO::VBO(float vertices[], size_t vertices_size, GLenum usage){
    initializeOpenGLFunctions();
    glGenBuffers(1, &id);
    fill(vertices, vertices_size, usage);
}
void VBO::bind(){
    glBindBuffer(GL_ARRAY_BUFFER, id);
}
void VBO::unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VBO::fill(float vertices[], size_t vertices_size, GLenum usage){
    bind();
    glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, usage);
}

VAO::VAO(){
    initializeOpenGLFunctions();
    glGenVertexArrays(1, &id);
}
void VAO::bind(){
    glBindVertexArray(id);
}
void VAO::unbind(){
    glBindVertexArray(0);
}

EBO::EBO(){
    initializeOpenGLFunctions();
    glGenBuffers(1, &id);
}
EBO::EBO(uint32_t indices[], size_t indices_size, GLenum usage){
    initializeOpenGLFunctions();
    glGenBuffers(1, &id);
    fill(indices, indices_size, usage);
}
void EBO::bind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}
void EBO::unbind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
void EBO::fill(uint32_t indices[], size_t indices_size, GLenum usage){
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_size, indices, usage);
}

Texture2D::Texture2D(char* file_path, GLenum internal_format){
    initializeOpenGLFunctions();
    glGenTextures(1, &id);
    fill(file_path, internal_format);
}
Texture2D::Texture2D(uint8_t* data, uint32_t width, uint32_t height, GLenum image_format, GLenum internal_format){
    initializeOpenGLFunctions();
    glGenTextures(1, &id);
    fill(data, width, height, image_format, internal_format);
}
void Texture2D::fill(char* file_path, GLenum internal_format){
    //load texture
    uint8_t* data = stbi_load(file_path, &texture_width, &texture_height, &color_channels_count, 0);
    if(!data){
        printf("Failed to load texture image %s\n", file_path);
        throw std::runtime_error(" ^ ^");
    }
    if(color_channels_count != 3 && color_channels_count != 4){
        printf("Invalid color channels count for texture image: %d\n", color_channels_count);
        throw std::runtime_error(" ^ ^");
    }
    fill(data, texture_width, texture_height, (color_channels_count == 4) ? GL_RGBA : GL_RGB, internal_format);
    stbi_image_free(data);
}
void Texture2D::fill(uint8_t* data, uint32_t width, uint32_t height, GLenum image_format, GLenum internal_format){
    if(internal_format == 0){
        internal_format = image_format;
    }
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, image_format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}
void Texture2D::bind(){
    glBindTexture(GL_TEXTURE_2D, id);
}
void Texture2D::bind_texture_unit(GLenum texture_unit){
    glActiveTexture(texture_unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

//others
char* read_file(const char* filename){
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if(!file){
        throw std::runtime_error("Failed to open file for reading");
    }
    int size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buf = new char[size + 1];
    file.read(buf, size);
    buf[size] = '\0';

    return buf;
}
