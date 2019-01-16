#include "main.h"

#ifndef BALL_H
#define BALL_H


class Object {
    /* This is the primary class for all movable objects
     * and sprites */
public:
    Object() {}
    Object(float x, float y, color_t color, GLfloat vertex_buffer_data[], int num_vertices);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed;
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
        }
    glm::vec3 a; //Acceleration +ve downwards -ve upwards
    glm::vec3 v; //Velocities in x & y directions
    void tick();
};

#endif // BALL_H
