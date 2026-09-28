#include "GLRenderer.hpp"

#include <iostream>
#include <fstream>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Renderer objects
VertexShader::VertexShader(){
    initializeOpenGLFunctions();
}
VertexShader::~VertexShader(){
    if(id != 0){
        puts("Log: VertexShader: Killing VertexShader");
        glDeleteShader(id);
        id = 0;
    }
}
void VertexShader::from_source(const char* shader_source_addr){
    if(id != 0){
        puts("Log: VertexShader: Killing old VertexShader");
        glDeleteShader(id);
        id = 0;
    }

    id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(id, 1, &shader_source_addr, NULL);
    glCompileShader(id);
    {
        int success;
        char infoLog[1024];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(id, 1024, NULL, infoLog);
            printf("Err: VertexShader: VertexShader compilation failed\n\t%s\n", infoLog);
            throw std::runtime_error("^");
        }
        else{
            puts("Log: VertexShader: VertexShader compiled successfully");
        }
    }
}
void VertexShader::from_file(const char* filename){
    char* shader_source = read_file(filename);
    this->from_source(shader_source);
    delete[] shader_source;
}

FragmentShader::FragmentShader(){
    initializeOpenGLFunctions();
}
FragmentShader::~FragmentShader(){
    if(id != 0){
        puts("Log: FragmentShader: Killing FragmentShader");
        glDeleteShader(id);
        id = 0;
    }
}
void FragmentShader::from_source(const char* shader_source_addr){
    if(id != 0){
        puts("Log: FragmentShader: Killing old FragmentShader");
        glDeleteShader(id);
        id = 0;
    }

    id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(id, 1, &shader_source_addr, NULL);
    glCompileShader(id);
    {
        int success;
        char infoLog[1024];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if(!success){
            glGetShaderInfoLog(id, 1024, NULL, infoLog);
            printf("Err: FragmentShader: FragmentShader compilation failed\n\t%s\n", infoLog);
            throw std::runtime_error("^");
        }
        else{
            puts("Log: FragmentShader: FragmentShader compiled successfully");
        }
    }
}
void FragmentShader::from_file(const char* filename){
    char* shader_source = read_file(filename);
    this->from_source(shader_source);
    delete[] shader_source;
}

ShaderProgram::ShaderProgram(){
    initializeOpenGLFunctions();
}
ShaderProgram::~ShaderProgram(){
    if(id != 0){
        puts("Log: ShaderProgram: Killing ShaderProgram");
        glDeleteProgram(id);
        id = 0;
    }

    if(vertShader != nullptr){
        puts("Log: ShaderProgram: Killing VertexShader");
        delete vertShader;
        vertShader = nullptr;
    }
    if(fragShader != nullptr){
        puts("Log: ShaderProgram: Killing FragmentShader");
        delete fragShader;
        fragShader = nullptr;
    }
}
void ShaderProgram::from_shaders(VertexShader* vertex_shader, FragmentShader* fragment_shader){
    if(id != 0){
        puts("Log: ShaderProgram: Killing old ShaderProgram");
        glDeleteProgram(id);
        id = 0;
    }

    id = glCreateProgram();
    glAttachShader(id, vertex_shader->id);
    glAttachShader(id, fragment_shader->id);
    glLinkProgram(id);
    {
        int success;
        char infoLog[1024];
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success){
            glGetProgramInfoLog(id, 1024, NULL, infoLog);
            printf("Err: ShaderProgram: ShaderProgram link failed\n\t%s\n", infoLog);
            throw std::runtime_error(" ^ ^");
        }
        else{
            puts("Log: ShaderProgram: ShaderProgram linked successfully");
        }
    }
}
void ShaderProgram::from_sources(const char* vert_source, const char* frag_source){
    if(id != 0){
        puts("Log: ShaderProgram: Killing old ShaderProgram");
        glDeleteProgram(id);
        id = 0;
    }

    if(vertShader != nullptr){
        puts("Log: ShaderProgram: Killing old VertexShader");
        delete vertShader;
        vertShader = nullptr;
    }
    if(fragShader != nullptr){
        puts("Log: ShaderProgram: Killing old FragmentShader");
        delete fragShader;
        fragShader = nullptr;
    }

    vertShader = new VertexShader();
    vertShader->from_source(vert_source);
    fragShader = new FragmentShader();
    fragShader->from_source(frag_source);

    this->from_shaders(vertShader, fragShader);
}
void ShaderProgram::from_files(const char* vert_filename, const char* frag_filename){
    if(id != 0){
        glDeleteProgram(id);
        id = 0;
    }

    if(vertShader != nullptr){
        puts("Log: ShaderProgram: Killing old VertexShader");
        delete vertShader;
        vertShader = nullptr;
    }
    if(fragShader != nullptr){
        puts("Log: ShaderProgram: Killing old FragmentShader");
        delete fragShader;
        fragShader = nullptr;
    }

    vertShader = new VertexShader();
    vertShader->from_file(vert_filename);
    fragShader = new FragmentShader();
    fragShader->from_file(frag_filename);

    this->from_shaders(vertShader, fragShader);
}
void ShaderProgram::set_bool(const char* name, std::initializer_list<int> values){
    set_int(name, values);
}
void ShaderProgram::set_bool(UniformID uid, std::initializer_list<int> values){
    set_int(uid, values);
}
void ShaderProgram::set_int(const char* name, std::initializer_list<int> values){
    set_int(glGetUniformLocation(id, name), values);
}
void ShaderProgram::set_int(UniformID uid, std::initializer_list<int> values){
    const int* jump = values.begin();
    switch(values.size()){
        case 1:
            glUniform1i(uid, jump[0]);
        break;
        case 2:
            glUniform2i(uid, jump[0], jump[1]);
        break;
        case 3:
            glUniform3i(uid, jump[0], jump[1], jump[2]);
        break;
        case 4:
            glUniform4i(uid, jump[0], jump[1], jump[2], jump[3]);
        break;
        default:
            printf("Err: ShaderProgram: Invalid number of values to set uniform. Expected 1-4, got %lu\n", values.size());
            throw std::runtime_error("^");
    }
}
void ShaderProgram::set_float(const char* name, std::initializer_list<float> values){
    set_float(glGetUniformLocation(id, name), values);
}
void ShaderProgram::set_float(UniformID uid, std::initializer_list<float> values){
    const float* jump = values.begin();
    switch(values.size()){
        case 1:
            glUniform1f(uid, jump[0]);
        break;
        case 2:
            glUniform2f(uid, jump[0], jump[1]);
        break;
        case 3:
            glUniform3f(uid, jump[0], jump[1], jump[2]);
        break;
        case 4:
            glUniform4f(uid, jump[0], jump[1], jump[2], jump[3]);
        break;
        default:
            printf("Err: ShaderProgram: Invalid number of values to set uniform. Expected 1-4, got %lu\n", values.size());
            throw std::runtime_error("^");
    }
}
void ShaderProgram::set_mat4(const char* name, bool transpose, glm::mat4 matrix){
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
VBO::~VBO(){
    if(id != 0){
        puts("Log: VBO: Killing VBO");
        glDeleteBuffers(1, &id);
        id = 0;
    }
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
VAO::~VAO(){
    if(id != 0){
        puts("Log: VAO: Killing VAO");
        glDeleteVertexArrays(1, &id);
        id = 0;
    }
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
EBO::~EBO(){
    if(id != 0){
        puts("Log: EBO: Killing EBO");
        glDeleteBuffers(1, &id);
        id = 0;
    }
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
Texture2D::~Texture2D(){
    if(id != 0){
        puts("Log: Texture2D: Killing Texture2D");
        glDeleteTextures(1, &id);
        id = 0;
    }
}
void Texture2D::fill(char* file_path, GLenum internal_format){
    //load texture
    uint8_t* data = stbi_load(file_path, &texture_width, &texture_height, &color_channels_count, 0);
    if(!data){
        printf("Err: Texture2D: Failed to load texture image at path %s\n", file_path);
        throw std::runtime_error("^");
    }
    if(color_channels_count != 3 && color_channels_count != 4){
        printf("Err: Texture2D: Invalid color channels count for texture image at path %s. Expected 3-4, got %d\n", file_path, color_channels_count);
        throw std::runtime_error("^");
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
        throw std::runtime_error("Err: read_file: Failed to open file for reading");
    }
    int size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buf = new char[size + 1];
    file.read(buf, size);
    buf[size] = '\0';

    return buf;
}



