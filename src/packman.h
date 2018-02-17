#include "main.h"

#ifndef PACKMAN_H
#define PACKMAN_H


class Packman {
public:
    Packman() {}
    Packman(float x, float y, color_t color, float radius);
    glm::vec3 position;
    glm::vec3 velocity;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float radius;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // PACKMAN_H
