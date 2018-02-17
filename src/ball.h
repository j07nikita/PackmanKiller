#include "main.h"
#include "trampoline.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, color_t color, float radius);
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 acceleration;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    float radius;
    color_t color;
    bounding_box_t bounding_box();
    void tick();
private:
    VAO *object;
};

#endif // BALL_H
