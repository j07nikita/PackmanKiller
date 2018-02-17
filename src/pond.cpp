#include "pond.h"
#include "main.h"

Pond::Pond(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    GLfloat vertex_buffer_data[1001];
    float ini1 = 0.8;
    float ini2 = 0;
    int s = 0;
    int n = 20;
    double theta = 9;
    double PI = 3.1415926;
    for(int i = 0; i < n; i++) {
        vertex_buffer_data[s++] = 0;
        vertex_buffer_data[s++] = 0;
        vertex_buffer_data[s++] = 0.0f;
        vertex_buffer_data[s++] = ini1;
        vertex_buffer_data[s++] = ini2;
        vertex_buffer_data[s++] = 0.0f;
        vertex_buffer_data[s++] = ini1*cos(theta * PI / 180) + ini2 * sin(theta * PI / 180);
        vertex_buffer_data[s++] = -ini1*sin(theta * PI / 180) + ini2 * cos(theta * PI / 180);
        vertex_buffer_data[s++] = 0.0f;
        double temp1 = ini1;
        double temp2 = ini2;
        ini1 = temp1 * cos(theta * PI/180) + temp2 * sin(theta * PI/180);
        ini2 = temp2 * cos(theta * PI / 180) - temp1 * sin(theta * PI / 180);
    }
    this->object = create3DObject(GL_TRIANGLES, 60, vertex_buffer_data, color, GL_FILL);
}

void Pond::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Pond::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bool Pond::inside_pond(bounding_box_t a) {
    return ((a.x >= this->position.x - 0.6) && (a.x <= this->position.x + 0.6));
}

