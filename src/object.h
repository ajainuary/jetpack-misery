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
    }
    int value;
};

class Combo {
    /* Combines objects to form one sprite */
public:
    Combo() {}
    Combo(float x, float y) {
        this->set_position(x, y);
    }
    std::vector<std::pair<Object, glm::vec3>> objects; //Objects along with their relative position from the combined origin
    float x,y; //Origin
    float rotation;
    void set_position(float x, float y);
    void draw(glm::mat4 VP);
    bool detect(bounding_box_t p);
};

class FireLine : public Combo {
public:
    FireLine() {};
    FireLine(float x, float y, int angle = 0) : Combo(x, y) {
        if(angle == 0) {
            GLfloat circle_vertex[362*3];
            create_ellipse(0.175, 0.175, circle_vertex);
            std::cerr << this->objects.size() << std::endl;
            this->objects.push_back({Object(0, 0, COLOR_GREY, circle_vertex, 362, GL_TRIANGLE_FAN), {1,0,0}});
            std::cerr << this->objects.size() << std::endl;
            this->objects.push_back({Object(0, 0, COLOR_GREY, circle_vertex, 362, GL_TRIANGLE_FAN), {-1,0,0}});
            std::cerr << this->objects.size() << std::endl;
            GLfloat rectangle[] = {
                -1, 0.175, 0,
                1, 0.175, 0,
                -1, -0.175, 0,
                1, 0.175, 0,
                1, -0.175, 0,
                -1, -0.175, 0,
            };
            this->objects.push_back({Object(0, 0, COLOR_YELLOW, rectangle, 6), {0, 0, 0}});
        }
        else if(angle = 2) {
            GLfloat circle_vertex[362*3];
            create_ellipse(0.175, 0.175, circle_vertex);
            std::cerr << this->objects.size() << std::endl;
            this->objects.push_back({Object(0, 0, COLOR_GREY, circle_vertex, 362, GL_TRIANGLE_FAN), {0,1,0}});
            std::cerr << this->objects.size() << std::endl;
            this->objects.push_back({Object(0, 0, COLOR_GREY, circle_vertex, 362, GL_TRIANGLE_FAN), {0,-1,0}});
            std::cerr << this->objects.size() << std::endl;
            GLfloat rectangle[] = {
                0.175, -1, 0,
                0.175, 1, 0,
                -0.175, -1, 0,
                0.175, 1, 0,
                -0.175, 1, 0,
                -0.175, -1, 0
            };
            this->objects.push_back({Object(0, 0, COLOR_YELLOW, rectangle, 6), {0, 0, 0}});
        }
    }
};

#endif // BALL_H
