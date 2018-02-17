#include "main.h"

#ifndef PROCUPINES_H
#define PROCUPINES_H


class Procupines {
public:
    Procupines() {}
    Procupines(float start_x, float start_y, color_t color, int no_of_triangles);
    glm::vec3 position;
    float rotation;
    float no_of_triangles;
    void draw(glm::mat4 VP);
    bool on_procupines(float x, float y);
//    void tick();
//    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // Pond_H

