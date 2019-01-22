#include "main.h"

#ifndef BALL_H
#define BALL_H


class Object {
    /* This is the primary class for all movable objects
     * and sprites */
public:
    Object() {}
    Object(float x, float y, color_t color, GLfloat vertex_buffer_data[], int num_vertices, GLenum primitive_mode = GL_TRIANGLES);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
    bounding_box_t box;
private:
    VAO *object;
};

class Player : public Object {
public:
    Player() {}
    Player(float x, float y, color_t color, float a_x, float a_y, float v_x, float v_y, GLfloat vertex_buffer_data[], int num_vertices) : Object(x,y,color, vertex_buffer_data, num_vertices)
        {
            std::cerr << sizeof(vertex_buffer_data) << ' ' << num_vertices << '\n';
            this->a = glm::vec3(a_x, a_y, 0);
            this->v = glm::vec3(v_x, v_y, 0);
            this->joy = false;
        }
    glm::vec3 a; //Acceleration +ve downwards -ve upwards
    glm::vec3 v; //Velocities in x & y directions
    bool joy; //Press the jetpack?
    void tick();
};

class Platform : public Object {
public:
    Platform() {}
    Platform(color_t color, GLfloat vertex_buffer_data[]) : Object(0.0f, 0.0f, color, vertex_buffer_data, 6){};
};

class Coin : public Object {
public:
    Coin() {}
    Coin(float x, float y, int value, color_t color, GLfloat vertex_buffer_data[], int number_vertices) : Object(x, y, color, vertex_buffer_data, number_vertices, GL_TRIANGLE_FAN){
        this->value = value;
        this->visible = true;
    }
    int value;
    bool visible;
};

#endif // BALL_H
