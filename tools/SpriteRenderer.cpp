//
// Created by Srf on 2017/10/9.
//

#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader &shader) {
    this->shader = shader;
    this->initRenderData();
}

SpriteRenderer::~SpriteRenderer() {
    glDeleteVertexArrays(1, &this->quadVAO);
}

void SpriteRenderer::drawSprite(
        Texture2D &texture,
        GLuint filterMode,
        glm::vec2 position,
        glm::vec2 size,
        GLfloat rotateX,
        GLfloat rotateY,
        GLfloat rotateZ
        ) {
    this->shader.use();

    // model matrix may be changed per frame so we set it when draw
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    // rotate is radians parameter
    model = glm::rotate(model, rotateY, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotateX, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotateZ, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(size, 1.0f));

    this->shader.setMatrix4("model", model);
    this->shader.setInteger("filteringMode", filterMode);

    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    glBindVertexArray(this->quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void SpriteRenderer::initRenderData() {
    GLfloat vertices[] = {
            //  ---- 位置 ----    - 纹理坐标 -
            1.0f, 1.0f, 0.0f,     1.0f, 1.0f,   // 右上
            1.0f, -1.0f, 0.0f,    1.0f, 0.0f,   // 右下
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,   // 左下
            -1.0f, 1.0f, 0.0f,    0.0f, 1.0f    // 左上
    };

    GLuint indices[] = {
        0, 2, 3,
        0, 1, 2
    };

    GLuint VBO, EBO; // Element Buffer Object also called Index Buffer Object IBO

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(this->quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    /*
     * parameters:
     * 0 -- layout position
     * 3 -- number of attributes
     * GL_FLOAT -- type of attributes
     * GL_FALSE -- is normalized
     * 5 * sizeof(GLfloat) -- attributes' data length
     * nullptr(0) -- offset
     */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    assert(glGetError() == GL_NO_ERROR);
}