#include "main.h"

#ifndef PACKMANSLOPE_H
#define PACKMANSLOPE_H


class PackmanSlope {
public:
    PackmanSlope() {}
    PackmanSlope(float x, float y, color_t color, float radius);
    glm::vec3 position;
    glm::vec3 velocity;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    float radius;
    bounding_box_t bounding_box();
    bool collision(float x, float y, float radius);
private:
    VAO *object;
};

#endif // PACKMANSLOPE_H
