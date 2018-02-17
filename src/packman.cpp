#include "packman.h"
#include "main.h"

Packman::Packman(float x, float y, color_t color, float radius) {
    this->position = glm::vec3(x, y, 0);
    this->velocity = glm::vec3(0.01, 0, 0);
    this->rotation = 0;
    this->radius = radius;
    glm::vec3 curr = glm::vec3(radius, 0, 0);
    int s = 0;
    int n = 30;
    GLfloat vertex_buffer_data[9 * n];
    double theta = 12;
    double PI = 3.1415926;
    for(int i = 0; i < n; i++) {
        vertex_buffer_data[s++] = 0;
        vertex_buffer_data[s++] = 0;
        vertex_buffer_data[s++] = 0.0f;
        vertex_buffer_data[s++] = curr.x;
        vertex_buffer_data[s++] = curr.y;
        vertex_buffer_data[s++] = 0.0f;
        vertex_buffer_data[s++] = curr.x*cos(theta * PI / 180) + curr.y * sin(theta * PI / 180);
        vertex_buffer_data[s++] = -curr.x*sin(theta * PI / 180) + curr.y * cos(theta * PI / 180);
        vertex_buffer_data[s++] = 0.0f;
        double temp1 = curr.x;
        double temp2 = curr.y;
        curr.x = temp1 * cos(theta * PI/180) + temp2 * sin(theta * PI/180);
        curr.y = temp2 * cos(theta * PI / 180) - temp1 * sin(theta * PI / 180);
    }

    this->object = create3DObject(GL_TRIANGLES, 3 * n, vertex_buffer_data, color, GL_FILL);
}

void Packman::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Packman::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Packman::tick() {
    this->position.x += velocity.x;
    if(this->position.x >= 4) {
            float x = (float)(rand() % 7 + 4);
            this->position.x = -x;
    }
}

bounding_box_t Packman::bounding_box() {
    float x = this->position.x, y = this->position.y;
    bounding_box_t bbox = { x, y, this->radius, 2 * this->radius};
    return bbox;
}
