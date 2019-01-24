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
    this->num_vertices = num_vertices;
    if(primitive_mode == GL_TRIANGLES) {
        this->advanced_collision_detection = true;
        this->mesh = std::vector<GLfloat>(vertex_buffer_data, vertex_buffer_data+(num_vertices*3));
    }
    else {
        this->advanced_collision_detection = false;
    }
    this->object = create3DObject(primitive_mode, num_vertices, vertex_buffer_data, color, GL_FILL);
}

void Object::draw(glm::mat4 VP) {
//    std::cerr << "Draw at " << this->position.x << ' ' << this->position.y << std::endl;
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation), glm::vec3(0, 0, 1));
    std::cerr << this->rotation << std::endl;
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
        y = min(6, y+0.075f); //prevent going to space
        this->v.y = 0;
        this->joy = false;
    }
    this->set_position(x, y);
}

void Combo::set_position(float x, float y, float rotation) {
//    std::cerr << rotation << std::endl;
    this->x = x;
    this->y = y;
    for(auto &o : this->objects) {
        o.first.set_position(x+o.second.x*cos(rotation)-o.second.y*sin(rotation), y+o.second.y*cos(rotation)+x*sin(rotation));
        o.first.rotation = rotation;
//        std::cerr << o.first.position.x << ' ' << o.first.position.y << ' ' << x+o.second.x << ' ' << y+o.second.y << std::endl;
    }
    return;
}

void Combo::draw(glm::mat4 VP) {
//    std::cerr << this->objects.size() << std::endl;
    for(auto &o : this->objects) {
        o.first.set_position(x+o.second.x, y+o.second.y);
    }
    for(auto &o : this->objects) {
        o.first.draw(VP);
    }
}

bool Combo::detect(bounding_box_t b) {
    for(auto &o : this->objects) {
        if(detect_collision(o.first.box, b))
            return true;
    }
    return false;
}

void Boomerang::tick() {
    this->set_position(a*cos(M_PI*(t/180)), b*sin(M_PI*(t/180)), M_PI*(t/30));
    t = t+1.0f;
    if(t == 361.0f) t = 0.0f;
}
