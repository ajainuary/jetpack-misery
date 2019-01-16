#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, color_t color, GLfloat vertex_buffer_data[], int num_vertices) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    this->object = create3DObject(GL_TRIANGLES, num_vertices, vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
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

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

void Player::tick() {
    this->v.x = this->v.x+this->a.x;
    this->v.y = this->v.y+this->a.y;
    this->position.x = this->position.x+this->v.x;
    this->position.y = this->position.y+this->v.y;
    if(this->position.y < 0)
    {
        this->v.y = -0.64f*this->v.y;
        this->a.y += -0.1f*this->position.y;
    }
    else if(this->position.y > 6)
    {
        this->v.y = -0.64f*this->v.y;
        this->a.y += -0.1f*(this->position.y-6);
    }
    else {
        this->a.y = -0.02/60.0f;
    }
}
