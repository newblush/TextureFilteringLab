//
// Created by Srf on 2018/9/23.
//

#include "TextureFilteringScene.h"

void TextureFilteringScene::init() {
    ResourceManager::loadShader(this->vertexShaderPath, this->fragmentShaderPath, nullptr, "texture filtering");
    ResourceManager::loadTexture(this->texturePath, GL_FALSE, "subject");

    // configure shaders
    // Perspective projection matrix
    glm::mat4 projection = glm::perspective(
            glm::radians(75.0f), // Field of View -- FOV
            (float)width/(float)height,
            0.1f, // near plane
            100.0f // far plane
            );
    //view matrix
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -0.8f));
    Shader shader = ResourceManager::getShader("texture filtering");
    shader.use().setMatrix4("projection", projection);
    shader.use().setMatrix4("view", view);
    // model matrix is in drawSprite function
    // texture index, default first texture index is 0
    shader.use().setInteger("texSlot", 0);

    // init renderer
    renderer = new SpriteRenderer(shader);
}

void TextureFilteringScene::render() {
    Texture2D texture = ResourceManager::getTexture("subject");
    renderer -> drawSprite(
            texture,
            this->filteringMode,
            this->position,
            glm::vec2(1, 1),
            this->rotateX,
            this->rotateY
            );
    assert(glGetError() == GL_NO_ERROR);
}

void TextureFilteringScene::update() {
    if (this->keys[GLFW_KEY_UP] && !this->keysProcessed[GLFW_KEY_UP]) {
        this->keysProcessed[GLFW_KEY_UP] = GL_TRUE;
        this->rotateX += glm::radians(5.0f);
    }
    if (this->keys[GLFW_KEY_DOWN] && !this->keysProcessed[GLFW_KEY_DOWN]) {
        this->keysProcessed[GLFW_KEY_DOWN] = GL_TRUE;
        this->rotateX -= glm::radians(5.0f);
    }
    if (this->keys[GLFW_KEY_LEFT] && !this->keysProcessed[GLFW_KEY_LEFT]) {
        this->keysProcessed[GLFW_KEY_LEFT] = GL_TRUE;
        this->rotateY -= glm::radians(5.0f);
    }
    if (this->keys[GLFW_KEY_RIGHT] && !this->keysProcessed[GLFW_KEY_RIGHT]) {
        this->keysProcessed[GLFW_KEY_RIGHT] = GL_TRUE;
        this->rotateY += glm::radians(5.0f);
    }
    for (GLuint i = GLFW_KEY_1; i <= GLFW_KEY_8; i++) {
        if (this->keys[i] && !this->keysProcessed[i]) {
            this->keysProcessed[i] = GL_TRUE;
            this->filteringMode = i - 48; // 1 to 8
            const char *modeName = nullptr;
            switch (filteringMode) {
                case 1:
                    modeName = "nearest";
                    break;
                case 2:
                    modeName = "bilinear";
                    break;
                case 3:
                    modeName = "bilinear/mipmap";
                    break;
                case 4:
                    modeName = "trilinear/mipmap";
                    break;
                case 5:
                    modeName = "2x anisotropic";
                    break;
                case 6:
                    modeName = "4x anisotropic";
                    break;
                case 7:
                    modeName = "8x anisotropic";
                    break;
                case 8:
                    modeName = "16x anisotropic";
                    break;
                default:
                    modeName = "nearest";
            }
            std::cout << "Using texture filtering mode " << filteringMode << " : " << modeName << std::endl;
            break;
        }
    }
}