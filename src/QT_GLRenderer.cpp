#include "QT_GLRenderer.hpp"

#include <stdio.h>
#include <iostream>

const char* vertexShaderPath = "shaders/vert.glsl";
const char* fragmentShaderPath = "shaders/frag.glsl";

QT_GLRenderer::QT_GLRenderer(QWidget* parent) : QOpenGLWidget(parent){
    //settings
    wireframe_mode = true;
    depth_test = true;
    FPS = 60;

    setFocusPolicy(Qt::StrongFocus);

    connect(&renderTimer, &QTimer::timeout, this, [this](){
            float deltaTime = frameTimer.restart() / 1000.0f;
            updateCamera(deltaTime);
            update();
        }
    );

    //renderTimer configuration
    renderTimer.setInterval(1000 / FPS); //how much ms to delay to achieve the target fps
    frameTimer.start();
    renderTimer.start();
}

QT_GLRenderer::~QT_GLRenderer(){
    makeCurrent();

    doneCurrent();
}

void QT_GLRenderer::initializeGL(){
    initializeOpenGLFunctions();

    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &QT_GLRenderer::destroyGLResources, Qt::DirectConnection);

    printInfo();
    applySettings();

    createGLResources();

    update();
}

void QT_GLRenderer::resizeGL(int width, int height){
    glViewport(0, 0, width, height);

    camera.aspect_ratio = ((float)width) / ((float)height);
}

void QT_GLRenderer::paintGL(){
    //clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //draw render objects
    for(auto& object : objects){
        glm::vec3 t = object.get()->transform.position;
        drawRenderObject(object);
    }
}

void QT_GLRenderer::keyPressEvent(QKeyEvent* event){
    switch (event->key()){
        case Qt::Key_W:
            moveForward = true;
            break;

        case Qt::Key_S:
            moveBackward = true;
            break;

        case Qt::Key_A:
            moveLeft = true;
            break;

        case Qt::Key_D:
            moveRight = true;
            break;

        case Qt::Key_Space:
            moveUp = true;
            break;

        case Qt::Key_Control:
            moveDown = true;
            break;
    }
}

void QT_GLRenderer::keyReleaseEvent(QKeyEvent* event){
    switch (event->key()){
        case Qt::Key_W:
            moveForward = false;
            break;

        case Qt::Key_S:
            moveBackward = false;
            break;

        case Qt::Key_A:
            moveLeft = false;
            break;

        case Qt::Key_D:
            moveRight = false;
            break;

        case Qt::Key_Space:
            moveUp = false;
            break;

        case Qt::Key_Control:
            moveDown = false;
            break;
    }
}

void QT_GLRenderer::mousePressEvent(QMouseEvent* event){
    setFocus();

    lastMousePosition = event->position().toPoint();

    if(event->button() == Qt::RightButton){
        rotating = true;
    }
    if(event->button() == Qt::MiddleButton){
        panning = true;
    }
}

void QT_GLRenderer::mouseReleaseEvent(QMouseEvent* event){
    if(event->button() == Qt::RightButton){
        rotating = false;
    }

    if(event->button() == Qt::MiddleButton){
        panning = false;
    }
}

void QT_GLRenderer::mouseMoveEvent(QMouseEvent* event){
    QPoint current = event->position().toPoint();

    QPoint delta = current - lastMousePosition;

    lastMousePosition = current;

    if(rotating){
        camera.rotate(
            delta.x(),
            delta.y()
        );
        update();
    }
    if(panning){
        camera.pan(
            delta.x(),
            delta.y()
        );
        update();
    }
}

void QT_GLRenderer::wheelEvent(QWheelEvent* event){
    camera.zoom(
        event->angleDelta().y() / 120.0f
    );
}


void QT_GLRenderer::updateCamera(float deltaTime){
    if(moveForward){
        camera.moveForward(camera.movementSpeed * deltaTime);
    }
    if(moveBackward){
        camera.moveForward(-camera.movementSpeed * deltaTime);
    }
    if(moveRight){
        camera.moveRight(camera.movementSpeed * deltaTime);
    }
    if(moveLeft){
        camera.moveRight(-camera.movementSpeed * deltaTime);
    }
    if(moveUp){
        camera.moveUp(camera.movementSpeed * deltaTime);
    }
    if(moveDown){
        camera.moveUp(-camera.movementSpeed * deltaTime);
    }
}

void QT_GLRenderer::createGLResources(){
    //make shader
    shaderProgram = std::make_unique<ShaderProgram>();
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

    //make mesh
    mesh = std::make_unique<Mesh>(vertices, sizeof(vertices));
    
    objects.clear();

    //make render object
    auto cube1 = std::make_unique<RenderObject>();
    cube1->transform.position = glm::vec3(-2.0f, 0.0f, 0.0f);
    cube1->mesh = mesh.get();
    cube1->shader = shaderProgram.get();
    objects.push_back(std::move(cube1));

    //make another render object
    auto cube2 = std::make_unique<RenderObject>();
    cube2->transform.position = glm::vec3(2.0f, 0.0f, 0.0f);
    cube2->mesh = mesh.get();
    cube2->shader = shaderProgram.get();
    objects.push_back(std::move(cube2));
}

void QT_GLRenderer::destroyGLResources(){
    objects.clear();

    mesh.reset();
    shaderProgram.reset();
}

void QT_GLRenderer::drawRenderObject(std::unique_ptr<RenderObject>& object){
    glm::mat4 model = object->transform.get_matrix();
    glm::mat4 view = camera.get_view_matrix();
    glm::mat4 projection = camera.get_projection_matrix();

    object->shader->activate();

    object->shader->set_mat4("model", false, model);
    object->shader->set_mat4("view", false, view);
    object->shader->set_mat4("projection", false, projection);

    object->mesh->draw();
}

void QT_GLRenderer::printInfo(){
    for(int i=0;i<20;i++){
        printf("-");
    }
    puts("");
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
    for(int i=0;i<20;i++){
        printf("-");
    }
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








