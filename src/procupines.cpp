#include "procupines.h"
#include "main.h"
#include <bits/stdc++.h>
using namespace std;

Procupines::Procupines(float start_x, float start_y, color_t color, int no_of_triangles) {
    this->position = glm::vec3(start_x, start_y, 0);
    this->rotation = 0;
    this->no_of_triangles = no_of_triangles;
    GLfloat vertex_buffer_data[] = {
     0, 0, 0,
     0.4, 0, 0,
     0.2, 0.565, 0,
     0.4, 0, 0,
     0.8, 0, 0,
     0.6, 0.565, 0,
     0.8, 0, 0,
     1.2, 0, 0,
     1, 0.565, 0,
    };
    this->object = create3DObject(GL_TRIANGLES, 3 * no_of_triangles, vertex_buffer_data, color, GL_FILL);
}

void Procupines::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}


bool Procupines::on_procupines(float x, float y) {
    return ((x >= this->position.x && x <= this->position.x + 0.4 * this->no_of_triangles) && (y >= -1 && y <= 0.5655));
}


