//
// Created by Srf on 2018/9/23.
//

#ifndef CG_LAB1_TEXTUREFILTERINGSCENE_H
#define CG_LAB1_TEXTUREFILTERINGSCENE_H

#include <cstring>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../tools/ResourceManager.h"
#include "../tools/SpriteRenderer.h"

class TextureFilteringScene {
private:
    const GLchar* vertexShaderPath = "../shader/vertex.glsl";
    const GLchar* fragmentShaderPath = "../shader/fragment.glsl";
    const GLchar* texturePath = "../resource/subject.jpg";
    GLuint width;
    GLuint height;
    glm::vec2 position = glm::vec2(0.0f);
    GLfloat rotateX = glm::radians(-80.0f);
    GLfloat rotateY = glm::radians(0.0f);
    glm::vec3 rotateAxis = glm::vec3(1.0f, 0.0f, 0.0f); // x-axis
    SpriteRenderer *renderer = nullptr;
    GLuint filteringMode = 1;

public:
    GLboolean keys[1024];
    GLboolean keysProcessed[1024];
    TextureFilteringScene(GLuint width, GLuint height): width(width), height(height), keys(), keysProcessed() {}
    ~TextureFilteringScene() = default;
    void init();
    void render();
    void update();
};


#endif //CG_LAB1_TEXTUREFILTERINGSCENE_H
