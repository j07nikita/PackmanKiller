#include "main.h"

#ifndef MAGNET_H
#define MAGNET_H


class Magnet {
public:
    Magnet() {}
    Magnet(float x, float y, color_t color, float radius, float theta);
    glm::vec3 position;
    glm::vec3 velocity;
    float rotation;
    void draw(glm::mat4 VP);
    int magneticEffect;
    float theta;
    glm::vec3 Force_due_to_magnet(float x, float y);
    float radius;
private:
    VAO *object;
};

#endif // MAGNET_H
