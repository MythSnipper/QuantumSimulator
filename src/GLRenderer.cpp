#include "GLRenderer.hpp"


GLRenderer::GLRenderer(QWidget* parent) : QOpenGLWidget(parent){

}

void GLRenderer::initializeGL(){
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

}

void GLRenderer::resizeGL(int width, int height){
    glViewport(0, 0, width, height);
}


void GLRenderer::paintGL(){
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //render frame



}

