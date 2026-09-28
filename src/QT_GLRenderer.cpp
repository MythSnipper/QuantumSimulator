#include "QT_GLRenderer.hpp"


const char* vertexShaderPath = "shaders/vert.glsl";
const char* fragmentShaderPath = "shaders/frag.glsl";

QT_GLRenderer::QT_GLRenderer(QWidget* parent) : QOpenGLWidget(parent){
    //settings
    wireframe_mode = true;
    depth_test = true;
}

QT_GLRenderer::~QT_GLRenderer(){
    makeCurrent();

    delete shaderProgram;
    delete vbo;
    delete vao;

    doneCurrent();
}

void QT_GLRenderer::initializeGL(){
    initializeOpenGLFunctions();
    
    printInfo();
    applySettings();

    shaderProgram = new ShaderProgram();
    shaderProgram->from_files(vertexShaderPath, fragmentShaderPath);

    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
    
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
    
        // front face
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
    
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
    
        // left face
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
    
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
    
        // right face
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
    
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
    
        // bottom face
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
    
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
    
        // top face
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
    
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f
    };
    
    vao = new VAO();
    vbo = new VBO();
    
    vao->bind();
    
    vbo->fill(vertices, sizeof(vertices), GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vao->unbind();

    vertexCount = 36;

}


void QT_GLRenderer::resizeGL(int width, int height){
    glViewport(0, 0, width, height);
}

void QT_GLRenderer::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view = glm::lookAt(
        glm::vec3(4.0f, 3.0f, 5.0f),  // camera position
        glm::vec3(0.0f, 0.0f, 0.0f),  // looking at
        glm::vec3(0.0f, 1.0f, 0.0f)   // up
    );

    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        float(width()) / float(height()),
        0.1f,
        100.0f
    );

    shaderProgram->activate();

    shaderProgram->set_mat4("model", false, model);
    shaderProgram->set_mat4("view", false, view);
    shaderProgram->set_mat4("projection", false, projection);

    vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    vao->unbind();
}

void QT_GLRenderer::printInfo(){
    for(int i=0;i<20;i++)printf("-");puts("");
    puts("Info:");
    printf("\tOS: ");
    #ifdef _WIN32
        puts("Windows");
    #else
        puts("Linux");
    #endif
    printf("\tWireframe mode: %s\n", (wireframe_mode) ? "On" : "Off");
    printf("\tDepth testing: %s\n", (depth_test) ? "On" : "Off");

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    printf("\tNumber of vertex attributes available: %d\n", nrAttributes);
    for(int i=0;i<20;i++)printf("-");
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
