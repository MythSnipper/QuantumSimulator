#include "QT_GLRenderer.hpp"

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
    
}

QT_GLRenderer::~QT_GLRenderer(){
    makeCurrent();

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    doneCurrent();
}

void QT_GLRenderer::initializeGL(){
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);


    static float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

}

void QT_GLRenderer::resizeGL(int width, int height){
    glViewport(0, 0, width, height);
}


void QT_GLRenderer::paintGL(){

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //render frame
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

}










//Renderer objects
VertexShader::VertexShader(char** shader_source_addr){
    id = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(id, 1, shader_source_addr, NULL);
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

FragmentShader::FragmentShader(char** shader_source_addr){
    id = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(id, 1, shader_source_addr, NULL);
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
ShaderProgram::ShaderProgram(char** vert_source, char** frag_source){
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
    vert.~VertexShader();
    frag.~FragmentShader();
}
ShaderProgram::ShaderProgram(char* vert_file, char* frag_file){
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
    vert.~VertexShader();
    frag.~FragmentShader();
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




