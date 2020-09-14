//
// Created by Srf on 2017/10/9.
//

#ifndef BREAKOUT_SPRITERENDERER_H
#define BREAKOUT_SPRITERENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture2D.h"
#include "Shader.h"

class SpriteRenderer {
public:
    explicit SpriteRenderer(Shader &shader);
    ~SpriteRenderer();
    void drawSprite(Texture2D &texture, GLuint filteringMode, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10),
                    GLfloat rotateX = 0.0f, GLfloat rotateY = 0.0f, GLfloat rotateZ = 0.0f);
private:
    Shader shader;
    GLuint quadVAO;
    void initRenderData();
};


#endif //BREAKOUT_SPRITERENDERER_H
