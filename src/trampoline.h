#include "main.h"

#ifndef Trampoline_H
#define Trampoline_H


class Trampoline {
public:
    Trampoline() {}
    Trampoline(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    bool on_trampoline(bounding_box_t a);
private:
    VAO *object;
};

#endif // Trampoline_H
