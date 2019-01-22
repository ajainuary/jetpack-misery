#include "object.h"
#include "main.h"

Object::Object(float x, float y, color_t color, GLfloat vertex_buffer_data[], int num_vertices, GLenum primitive_mode) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    GLfloat max_y = INT_MIN, max_x = INT_MIN, min_y = INT_MAX, min_x = INT_MAX;
    for (int i = 0;i < num_vertices; ++i) {
        max_x = max(max_x, vertex_buffer_data[3*i]);
        min_x = min(min_x, vertex_buffer_data[3*i]);
        max_y = max(max_y, vertex_buffer_data[3*i+1]);
        min_y = min(min_y, vertex_buffer_data[3*i+1]);
    }
    this->box = {x, y, max_x-min_x, max_y-min_y};
    this->object = create3DObject(primitive_mode, num_vertices, vertex_buffer_data, color, GL_FILL);
}

void Object::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Object::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->box.x = x;
    this->box.y = y;
}

void Object::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

void Player::tick() {
    //Standard physics
    float x = this->position.x, y = this->position.y;
    this->v.x = this->v.x+this->a.x;
    this->v.y = this->v.y+this->a.y;
    x = x+this->v.x;
    y = max(y+this->v.y, 0); //prevent sinking into ground
    //Jetpack physics
    if(this->joy)
    {
        y = min(6, y+0.05f); //prevent going to space
        this->v.y = 0;
        this->joy = false;
    }
    this->set_position(x, y);
}
