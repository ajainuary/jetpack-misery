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
    this->box = {((max_x+min_x)/2)+x, ((max_y+min_y)/2)+y, max_x-min_x, max_y-min_y};
    this->delta_x = this->box.x-x;
    this->delta_y = this->box.y-y;
    this->num_vertices = num_vertices;
    this->collision_detection = true;
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
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Object::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
    this->box.x = x+this->delta_x;
    this->box.y = y+this->delta_y;
}

void Object::tick() {
    // this->rotation += speed;
    // this->position.x -= speed;
    // this->position.y -= speed;
}

void Player::tick() {
    //Standard physics
    float x = this->x, y = this->y;
    this->v.x = this->v.x+this->a.x;
    if(this->gravity)
        this->v.y = this->v.y+this->a.y;
    x = x+this->v.x;
    if(this->gravity)
        y = max(y+this->v.y, -1); //prevent sinking into ground
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
    this->set_position(a*cos(M_PI*(t/180))+origin_x, b*sin(M_PI*(t/180))+origin_y, M_PI*(t/30));
    t = t+1.0f;
    if(t == 361.0f) t = 0.0f;
}

bool Water::tick() {
    float x = this->position.x, y = this->position.y;
    this->v_x = this->v_x+this->a_x;
    this->v_y = this->v_y+this->a_y;
    x = x+this->v_x;
    y = max(y+this->v_y, -1); //prevent sinking into ground
    y = min(y, 6); //prevent going to space
    this->rotation = 12*(this->t);
    this->set_position(x, y);
    ++t;
    return this->t > 120;
}

void Magnet::tick(Player &p) {
    float r_2 = (p.x - this->x)*(p.x - this->x)+(p.y - this->y)*(p.y - this->y);
    if(r_2 <= 25)
    {
        p.v.y = 0;
        p.v.x = 0;
        p.set_position(p.x+4.0f*(((this->x - p.x)/((sqrt(r_2)*r_2)+450))), p.y+4.0f*(((this->y - p.y)/((sqrt(r_2)*r_2)+450))));
    }
}

void FireBeam::tick() {
    float x = this->x, y = this->y;
    if(y >= 7)
        this->direction = -1;
    if(y <= 0)
        this->direction = 1;
    y += (this->direction) * 0.03f;
    this->set_position(x, y);
}

void FlyingObject::tick() {
    float x = this->x, y = this->y;
    if(x == -100)
        x = 5000;
    x -= 0.02f;
    y = 3.5 + 3.5*sin(x);
    this->set_position(x, y);
}

void Ring::tick(Player &p) {
    float r_2 = (p.x - this->x)*(p.x - this->x)+(p.y - this->y)*(p.y - this->y);
    if(2*2 < r_2 && r_2 < 3.5*3.5 && p.y < this->y)
    {
        p.invincible = true;
        p.v.y = 0;
        p.v.x = 0;
        p.set_position(p.x+0.01*(r_2*((this->y - p.y)/sqrt(r_2))), p.y-0.01*(r_2*((this->x - p.x)/sqrt(r_2))));
    }
    else {
        p.invincible = false;
    }
}

void Dragon::tick() {
    this->t = (t+1);
    for(int i = 0; i < 7; ++i)
        this->objects[i].second = {this->objects[i].second.x, this->y + 2*sin(t*(M_PI/60.0f)), 0};
    for(int i = 7; i < 32; ++i)
    {
        this->objects[i].second = {this->objects[i].second.x, this->y + 2*sin(t*(M_PI/60.0f)-((i-7)*0.2f)), 0};
    }
    if(std::rand() % 15 == 0)
    {
        GLfloat water_vertex_buffer[18*3];
        for (int i = 0;i < 6; ++i) {
            water_vertex_buffer[9*i] = 0;
            water_vertex_buffer[9*i+1] = 0;
            water_vertex_buffer[9*i+2] = 0;
            water_vertex_buffer[9*i+3] = 0.175*cos(i*M_PI/3);
            water_vertex_buffer[9*i+4] = 0.175*sin(i*M_PI/3);
            water_vertex_buffer[9*i+5] = 0;
            water_vertex_buffer[9*i+6] = 0.175*cos((i+1)*M_PI/3);
            water_vertex_buffer[9*i+7] = 0.175*sin((i+1)*M_PI/3);
            water_vertex_buffer[9*i+8] = 0;
        }
        this->fountain.push_back(Water(this->x, this->y + 2*sin(t*(M_PI/60.0f))+2, water_vertex_buffer, -0.008f));
    }
}
