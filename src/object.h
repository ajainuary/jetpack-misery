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
    float delta_x, delta_y;
    std::vector<GLfloat> mesh;
    int num_vertices;
    bounding_box_t box;
    bool collision_detection,advanced_collision_detection;
private:
    VAO *object;
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
    void set_position(float x, float y, float rotation = 0);
    void draw(glm::mat4 VP);
    bool detect(bounding_box_t p);
};

class Player : public Combo {
public:
    Player() {}
    Player(float x, float y, float a_x, float a_y, float v_x, float v_y) : Combo(x, y)
        {
        GLfloat leg[] = {
            0, 0.2, 0,
            0.1, 0.2, 0,
            0, 0, 0,
            0, 0, 0,
            0.1, 0.2, 0,
            0.1, 0, 0,
        };
        this->objects.push_back({Object(0, 0, COLOR_GREY, leg, 6), {-1.5, -0.075, 0}});
        this->objects.push_back({Object(0, 0, COLOR_GREY, leg, 6), {-1.75, -0.075, 0}});
        this->objects.push_back({Object(0, 0, COLOR_GREY, leg, 6), {-1, -0.075, 0}});
        this->objects.push_back({Object(0, 0, COLOR_GREY, leg, 6), {-0.75, -0.075, 0}});
        GLfloat tail[] = {
            0, 0.1, 0,
            0.2, 0.1, 0,
            0, 0, 0,
            0, 0, 0,
            0.2, 0.1, 0,
            0.2, 0, 0,
        };
        this->objects.push_back({Object(0, 0, COLOR_GREY, tail, 6), {-2.2, 0.35, 0}});
        GLfloat body[] = {
            0, 1, 0,
            2, 1, 0,
            0, 0, 0,
            0, 0, 0,
            2, 1, 0,
            2, 0, 0,
        };
        this->objects.push_back({Object(0, 0, COLOR_PINK, body, 6), {-2, 0.05, 0}});

        GLfloat face[] = {
            0, 0, 0,
            -0.5, 0.5, 0,
            -0.25, 1, 0,
            0, 0, 0,
            0.25, 1, 0,
            0.5, 0.5, 0,
            -0.375, 0.75, 0,
            0.375, 0.75, 0,
            0, 0, 0,
        };
        this->objects.push_back({Object(0, 0, COLOR_GREY, face, 9), {-0.03, 0, 0}});
        GLfloat curve_buffer[272*3];
        curve_buffer[0] = 0.1f; curve_buffer[1] = 0.1f; curve_buffer[2] = 0.0f;
        for(int i = 0; i < 271; ++i) {
            curve_buffer[3*i+3] = 0.1+0.1*cos(M_PI*(float(i-180)/180.0f));
            curve_buffer[3*i+4] = 0.1+0.1*sin(M_PI*(float(i-180)/180.0f));
            curve_buffer[3*i+5] = 0;
        }
        this->objects.push_back({Object(0, 0, COLOR_BLACK, curve_buffer, 182, GL_TRIANGLE_FAN), {-0.05, 0.25, 0}});
        (this->objects.end()-1)->first.collision_detection = false;
        curve_buffer[0] = 0.1f; curve_buffer[1] = 0.1f; curve_buffer[2] = 0.0f;
        for(int i = 0; i < 271; ++i) {
            curve_buffer[3*i+3] = 0.1+(0.1*cos(M_PI*(float(i-180)/180.0f)))*0.5;
            curve_buffer[3*i+4] = 0.1+(0.1*sin(M_PI*(float(i-180)/180.0f)))*0.5;
            curve_buffer[3*i+5] = 0;
        }
        this->objects.push_back({Object(0, 0, COLOR_GREY, curve_buffer, 182, GL_TRIANGLE_FAN), {-0.05, 0.25, 0}});
        (this->objects.end()-1)->first.collision_detection = false;
        curve_buffer[0] = -0.1f; curve_buffer[1] = 0.1f; curve_buffer[2] = 0.0f;
        for(int i = 0; i < 271; ++i) {
            curve_buffer[3*i+3] = -0.1+0.1*cos(M_PI*(float(i-180)/180.0f));
            curve_buffer[3*i+4] = 0.1+0.1*sin(M_PI*(float(i-180)/180.0f));
            curve_buffer[3*i+5] = 0;
        }
        this->objects.push_back({Object(0, 0, COLOR_BLACK, curve_buffer, 182, GL_TRIANGLE_FAN), {0, 0.25, 0}});
        (this->objects.end()-1)->first.collision_detection = false;
        curve_buffer[0] = -0.1f; curve_buffer[1] = 0.1f; curve_buffer[2] = 0.0f;
        for(int i = 0; i < 271; ++i) {
            curve_buffer[3*i+3] = -0.1+(0.1*cos(M_PI*(float(i-180)/180.0f)))*0.5;
            curve_buffer[3*i+4] = 0.1+(0.1*sin(M_PI*(float(i-180)/180.0f)))*0.5;
            curve_buffer[3*i+5] = 0;
        }
        this->objects.push_back({Object(0, 0, COLOR_GREY, curve_buffer, 182, GL_TRIANGLE_FAN), {0, 0.25, 0}});
        (this->objects.end()-1)->first.collision_detection = false;
        GLfloat eye[] = {
            -0.05, 0.05, 0,
            0.05, 0.05, 0,
            -0.05, -0.05, 0,
            0.05, 0.05, 0,
            0.05, -0.05, 0,
            -0.05, -0.05, 0,
        };
        this->objects.push_back({Object(0, 0, COLOR_BLACK, eye, 6), {-0.2, 0.55, 0}});
        this->objects.push_back({Object(0, 0, COLOR_BLACK, eye, 6), {0.15, 0.55, 0}});
            this->a = glm::vec3(a_x, a_y, 0);
            this->v = glm::vec3(v_x, v_y, 0);
            this->joy = false;
            this->lives = 9;
        this->invincible = false;
        this->gravity = true;
        this->speed = 0.06f;
        }
    glm::vec3 a; //Acceleration +ve downwards -ve upwards
    glm::vec3 v; //Velocities in x & y directions
    bool joy; //Press the jetpack?
    int lives;
    float speed;
    bool invincible, gravity;
    void tick();
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
            (this->objects.end()-1)->first.collision_detection = false;
            std::cerr << this->objects.size() << std::endl;
            this->objects.push_back({Object(0, 0, COLOR_GREY, circle_vertex, 362, GL_TRIANGLE_FAN), {-1,0,0}});
            (this->objects.end()-1)->first.collision_detection = false;
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
        else if(angle == 1) {
            GLfloat circle_vertex[362*3];
            create_ellipse(0.175, 0.175, circle_vertex);
            std::cerr << this->objects.size() << std::endl;
            this->objects.push_back({Object(0, 0, COLOR_GREY, circle_vertex, 362, GL_TRIANGLE_FAN), {0.7071,0.7071,0}});
            (this->objects.end()-1)->first.collision_detection = false;
            std::cerr << this->objects.size() << std::endl;
            this->objects.push_back({Object(0, 0, COLOR_GREY, circle_vertex, 362, GL_TRIANGLE_FAN), {-0.7071,-0.7071,0}});
            (this->objects.end()-1)->first.collision_detection = false;
            std::cerr << this->objects.size() << std::endl;
            GLfloat rectangle[] = {
                0.5833, 0.8308, 0,
                -0.5833, -0.8308, 0,
                -0.8308, -0.5833, 0,
                0.5833, 0.8308, 0,
                0.8308, 0.5833, 0,
                -0.5833, -0.8308, 0
            };
            this->objects.push_back({Object(0, 0, COLOR_YELLOW, rectangle, 6), {0, 0, 0}});
        }
        else if(angle == 2) {
            GLfloat circle_vertex[362*3];
            create_ellipse(0.175, 0.175, circle_vertex);
            std::cerr << this->objects.size() << std::endl;
            this->objects.push_back({Object(0, 0, COLOR_GREY, circle_vertex, 362, GL_TRIANGLE_FAN), {0,1,0}});
            (this->objects.end()-1)->first.collision_detection = false;
            std::cerr << this->objects.size() << std::endl;
            this->objects.push_back({Object(0, 0, COLOR_GREY, circle_vertex, 362, GL_TRIANGLE_FAN), {0,-1,0}});
            (this->objects.end()-1)->first.collision_detection = false;
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

class FireBeam : public Combo {
public:
     FireBeam() {};
     FireBeam(float x) : Combo(x, 0) {
         GLfloat circle_vertex[362*3];
         create_ellipse(0.175, 0.175, circle_vertex);
         std::cerr << this->objects.size() << std::endl;
         this->objects.push_back({Object(0, 0, COLOR_GREY, circle_vertex, 362, GL_TRIANGLE_FAN), {2,0,0}});
         (this->objects.end()-1)->first.collision_detection = false;
         std::cerr << this->objects.size() << std::endl;
         this->objects.push_back({Object(0, 0, COLOR_GREY, circle_vertex, 362, GL_TRIANGLE_FAN), {-2,0,0}});
         (this->objects.end()-1)->first.collision_detection = false;
         std::cerr << this->objects.size() << std::endl;
         GLfloat rectangle[] = {
             -2, 0.175, 0,
             2, 0.175, 0,
             -2, -0.175, 0,
             2, 0.175, 0,
             2, -0.175, 0,
             -2, -0.175, 0,
         };
         this->objects.push_back({Object(0, 0, COLOR_RED, rectangle, 6), {0, 0, 0}});
         this->direction = 1;
     };
     void tick();
     int direction;
};

class FlyingObject : public Combo {
public:
    FlyingObject() {};
    FlyingObject(float x, float y, int type) : Combo(x, y) {
        if(type == 0) {
            //Backward Arrow
            GLfloat circle_vertex[362*3];
            create_ellipse(0.25, 0.25, circle_vertex);
            this->objects.push_back({Object(0, 0, COLOR_GREY, circle_vertex, 362, GL_TRIANGLE_FAN), {0,0,0}});
            GLfloat arrow_back[] = {
                0, 0.2, 0,
                0.1, 0.2, 0,
                -0.1, 0, 0,
                0.1, 0.2, 0,
                0, 0, 0,
                -0.1, 0, 0,
                -0.1, 0, 0,
                0, -0.2, 0,
                0.1, -0.2, 0,
                0, 0, 0,
                0.1, -0.2, 0,
                -0.1, 0, 0,
            };
            this->objects.push_back({Object(0, 0, COLOR_YELLOW, arrow_back, 12), {-0.025, 0, 0}});
        }
        else if(type == 1) {
            //Forward Arrow
            GLfloat circle_vertex[362*3];
            create_ellipse(0.25, 0.25, circle_vertex);
            this->objects.push_back({Object(0, 0, COLOR_GREY, circle_vertex, 362, GL_TRIANGLE_FAN), {0,0,0}});
            GLfloat arrow_forward[] = {
                0, 0.2, 0,
                -0.1, 0.2, 0,
                0.1, 0, 0,
                -0.1, 0.2, 0,
                0, 0, 0,
                0.1, 0, 0,
                0.1, 0, 0,
                0, -0.2, 0,
                -0.1, -0.2, 0,
                0, 0, 0,
                -0.1, -0.2, 0,
                0.1, 0, 0,
            };
            this->objects.push_back({Object(0, 0, COLOR_YELLOW, arrow_forward, 12), {0.025, 0, 0}});
        }
        else if(type == 2) {
            GLfloat circle_vertex[362*3];
            create_heart(circle_vertex);
            this->objects.push_back({Object(0, 0, COLOR_RED, circle_vertex, 362, GL_TRIANGLE_FAN), {0,0,0}});
        }
        this->type = type;
    }
    float type;
    void tick();
};

class Boomerang : public Combo {
public:
    Boomerang() {}
    Boomerang(float x, float y, float origin_x, float origin_y, float a, float b): Combo(x, y) {
        GLfloat left_wing[] = {
            0.25,0.25,0,
            0+0.25,-0.175+0.25,0,
            0, 0, 0
        };
        GLfloat right_wing[] = {
            0.25,0.25,0,
            0+0.25,-0.175+0.25,0,
            0.5, 0, 0
        };
        this->objects.push_back({Object(0, 0, COLOR_WOOD, left_wing, 3), {0,0,0}});
        this->objects.push_back({Object(0, 0, COLOR_SECONDARY_WOOD, right_wing, 3), {0,0,0}});
        this->origin_x = origin_x;
        this->origin_y = origin_y;
        this->t = 0;
        this->a = a;
        this->b = b;
    };
    float origin_x, origin_y; //Origin of trajectory
    float a, b; //Trajectory parameters
    float t; //Parameter
    void tick();
};

class Water : public Object {
public:
    Water() {}
    Water(float x, float y, GLfloat vertex_buffer_data[],float a_x = 0.0008f, float v_x = 0.01f, float v_y = 0.01f, float a_y = -0.001f) : Object(x, y, COLOR_WATER, vertex_buffer_data, 18){
        this->v_x = v_x;
        this->v_y = v_y;
        this->a_x = a_x;
        this->a_y = a_y;
        this->t = 0;
    }
    float v_x,v_y,a_x,a_y; //Parameter
    int t;
    bool tick();
};

class Magnet : public Combo {
public:
    Magnet() {}
    Magnet(float x, float y) : Combo(x, y){
        GLfloat vertex_buffer_data[182*3];
        vertex_buffer_data[0] = 0.0f; vertex_buffer_data[1] = 0.0f; vertex_buffer_data[2] = 0.0f;
        for(int i = 0; i < 181; ++i) {
            vertex_buffer_data[3*i+3] = 0.5*cos(M_PI*(float(i)/180.0f));
            vertex_buffer_data[3*i+4] = 0.5*sin(M_PI*(float(i)/180.0f));
            vertex_buffer_data[3*i+5] = 0;
        }
        this->objects.push_back({Object(0, 0, COLOR_RED, vertex_buffer_data, 182, GL_TRIANGLE_FAN), {0, 0, 0}});
        vertex_buffer_data[0] = 0.0f; vertex_buffer_data[1] = 0.0f; vertex_buffer_data[2] = 0.0f;
        for(int i = 0; i < 181; ++i) {
            vertex_buffer_data[3*i+3] = 0.3*cos(M_PI*(float(i)/180.0f));
            vertex_buffer_data[3*i+4] = 0.3*sin(M_PI*(float(i)/180.0f));
            vertex_buffer_data[3*i+5] = 0;
        }
        this->objects.push_back({Object(0, 0, COLOR_BACKGROUND, vertex_buffer_data, 182, GL_TRIANGLE_FAN), {0, 0, 0}});
        GLfloat box[] = {
            0,0,0,
            0.2, 0, 0,
            0, 0.2, 0,
            0.2, 0, 0,
            0.2, 0.2, 0.2,
            0, 0.2, 0,
        };
        this->objects.push_back({Object(0, 0, COLOR_GREY, box, 6), {0.3, -0.2, 0}});
        this->objects.push_back({Object(0, 0, COLOR_GREY, box, 6), {-0.5, -0.2, 0}});
    };
    void tick(Player &p);
};

class Ring : public Combo {
public:
    Ring() {}
    Ring(float x, float y) : Combo(x, y) {
        GLfloat vertex_buffer_data[182*3];
        vertex_buffer_data[0] = 0.0f; vertex_buffer_data[1] = 0.0f; vertex_buffer_data[2] = 0.0f;
        for(int i = 0; i < 181; ++i) {
            vertex_buffer_data[3*i+3] = 3*cos(M_PI*(float(i)/180.0f));
            vertex_buffer_data[3*i+4] = 3*sin(M_PI*(float(i)/180.0f));
            vertex_buffer_data[3*i+5] = 0;
        }
        this->objects.push_back({Object(0, 0, COLOR_RED, vertex_buffer_data, 182, GL_TRIANGLE_FAN), {0, 0, 0}});
        vertex_buffer_data[0] = 0.0f; vertex_buffer_data[1] = 0.0f; vertex_buffer_data[2] = 0.0f;
        for(int i = 0; i < 181; ++i) {
            vertex_buffer_data[3*i+3] = 2.7*cos(M_PI*(float(i)/180.0f));
            vertex_buffer_data[3*i+4] = 2.7*sin(M_PI*(float(i)/180.0f));
            vertex_buffer_data[3*i+5] = 0;
        }
        this->objects.push_back({Object(0, 0, COLOR_BACKGROUND, vertex_buffer_data, 182, GL_TRIANGLE_FAN), {0, 0, 0}});
        this->set_position(x, y, M_PI);
    }
    void tick(Player &p);
};

class Dragon : public Combo {
public:
    Dragon() {}
    Dragon(float x, float y) : Combo(x, y) {
        GLfloat face[] = {
            0, 0, 0,
            0.5, -0.5, 0,
            1, -0.25, 0,
            0, 0, 0,
            1, 0.25, 0,
            0.5, 0.5, 0,
            0.75, -0.375, 0,
            0.75, 0.375, 0,
            0, 0, 0,
        };
        this->objects.push_back({Object(0, 0, COLOR_ORANGE, face, 9), {0, -0.03, 0}});
        GLfloat curve_buffer[272*3];
        curve_buffer[0] = 0.1f; curve_buffer[1] = 0.1f; curve_buffer[2] = 0.0f;
        for(int i = 0; i < 271; ++i) {
            curve_buffer[3*i+3] = 0.1+0.1*cos(M_PI*(float(i)/180.0f));
            curve_buffer[3*i+4] = 0.1+0.1*sin(M_PI*(float(i)/180.0f));
            curve_buffer[3*i+5] = 0;
        }
        this->objects.push_back({Object(0, 0, COLOR_DRAGONITE, curve_buffer, 182, GL_TRIANGLE_FAN), {0.25, -0.05, 0}});
        (this->objects.end()-1)->first.collision_detection = false;
        curve_buffer[0] = 0.1f; curve_buffer[1] = 0.1f; curve_buffer[2] = 0.0f;
        for(int i = 0; i < 271; ++i) {
            curve_buffer[3*i+3] = 0.1+(0.1*cos(M_PI*(float(i)/180.0f)))*0.5;
            curve_buffer[3*i+4] = 0.1+(0.1*sin(M_PI*(float(i)/180.0f)))*0.5;
            curve_buffer[3*i+5] = 0;
        }
        this->objects.push_back({Object(0, 0, COLOR_ORANGE, curve_buffer, 182, GL_TRIANGLE_FAN), {0.25, -0.05, 0}});
        (this->objects.end()-1)->first.collision_detection = false;
        curve_buffer[0] = 0.1f; curve_buffer[1] = -0.1f; curve_buffer[2] = 0.0f;
        for(int i = 0; i < 271; ++i) {
            curve_buffer[3*i+3] = 0.1+0.1*cos(M_PI*(float(i)/180.0f));
            curve_buffer[3*i+4] = -0.1+0.1*sin(M_PI*(float(i)/180.0f));
            curve_buffer[3*i+5] = 0;
        }
        this->objects.push_back({Object(0, 0, COLOR_DRAGONITE, curve_buffer, 182, GL_TRIANGLE_FAN), {0.25, 0, 0}});
        (this->objects.end()-1)->first.collision_detection = false;
        curve_buffer[0] = 0.1f; curve_buffer[1] = -0.1f; curve_buffer[2] = 0.0f;
        for(int i = 0; i < 271; ++i) {
            curve_buffer[3*i+3] = 0.1+(0.1*cos(M_PI*(float(i)/180.0f)))*0.5;
            curve_buffer[3*i+4] = -0.1+(0.1*sin(M_PI*(float(i)/180.0f)))*0.5;
            curve_buffer[3*i+5] = 0;
        }
        this->objects.push_back({Object(0, 0, COLOR_ORANGE, curve_buffer, 182, GL_TRIANGLE_FAN), {0.25, 0, 0}});
        (this->objects.end()-1)->first.collision_detection = false;
        GLfloat eye[] = {
            0.05, -0.05, 0,
            0.05, 0.05, 0,
            -0.05, -0.05, 0,
            0.05, 0.05, 0,
            -0.05, 0.05, 0,
            -0.05, -0.05, 0,
        };
        this->objects.push_back({Object(0, 0, COLOR_DRAGONITE, eye, 6), {0.5, -0.2, 0}});
        this->objects.push_back({Object(0, 0, COLOR_DRAGONITE, eye, 6), {0.55, 0.15, 0}});
        GLfloat body[] = {
            -0.125, 0.125, 0,
            0.125, 0.25, 0,
            -0.125, -0.125, 0,
            0.125, 0.25, 0,
            -0.125, -0.125, 0,
            0.125, -0.25, 0,
        };
        for(int i = 0; i < 25; ++i)
            this->objects.push_back({Object(0, 0, COLOR_ORANGE, body, 6), {1+i*0.3, -0.03, 0}});
        this->life = 5;
    }
    float t; //Parameter
    std::deque<Water> fountain;
    int life;
    void tick();
};

class Display : public Combo {
public:
    Display() {}
    Display(float x, float y, char c) : Combo(x, y) {
        if(c == '1'){
            GLfloat vertex_array_buffer [] {
                0.05, 0, 0,
                0.15, 0.5, 0,
                0.05, 0.5, 0,
                0.05, 0, 0,
                0.15, 0, 0,
                0.15, 0.5, 0,
                0.15, 0.55, 0,
                0.15, 0.5, 0,
                0.025, 0.5, 0,
            };
            this->objects.push_back({Object(0, 0, COLOR_WHITE, vertex_array_buffer, 9), {0, 0, 0}});
        }else if(c == '2') {
            GLfloat curve_buffer[182*3];
            curve_buffer[0] = 0.0f; curve_buffer[1] = 0.0f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 181; ++i) {
                curve_buffer[3*i+3] = 0.4*sin(M_PI*(float(i)/180.0f))*sin(M_PI*(float(i)/180.0f))*sin(M_PI*(float(i)/180.0f));
                curve_buffer[3*i+4] = 0.325*cos(M_PI*(float(i)/180.0f))-0.125*cos(M_PI*(float(i)/90.0f))-0.05*cos(M_PI*(float(i)/60.0f))-0.025*cos(M_PI*(float(i)/45.0f));
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_WHITE, curve_buffer, 182, GL_TRIANGLE_FAN), {0, 0.4, 0}});
            curve_buffer[0] = 0.0f; curve_buffer[1] = 0.0f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 181; ++i) {
                curve_buffer[3*i+3] = (0.4*sin(M_PI*(float(i)/180.0f))*sin(M_PI*(float(i)/180.0f))*sin(M_PI*(float(i)/180.0f)))*0.75;
                curve_buffer[3*i+4] = (0.325*cos(M_PI*(float(i)/180.0f))-0.125*cos(M_PI*(float(i)/90.0f))-0.05*cos(M_PI*(float(i)/60.0f))-0.025*cos(M_PI*(float(i)/45.0f)))*0.75;
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_BACKGROUND, curve_buffer, 182, GL_TRIANGLE_FAN), {0, 0.4, 0}});
            GLfloat vertex_buffer[] = {
                0,0,0,
                0,0.1,0,
                0.4, 0.1, 0,
                0.4, 0.1, 0,
                0.4, 0, 0,
                0, 0, 0,
            };
            this->objects.push_back({Object(0, 0, COLOR_WHITE, vertex_buffer, 6), {0, 0, 0}});
        } else if(c == '3') {
            GLfloat curve_buffer[272*3];
            curve_buffer[0] = 0.15f; curve_buffer[1] = 0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 271; ++i) {
                curve_buffer[3*i+3] = 0.15+0.15*cos(M_PI*(float(i-90)/180.0f));
                curve_buffer[3*i+4] = 0.15+0.15*sin(M_PI*(float(i-90)/180.0f));
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_WHITE, curve_buffer, 272, GL_TRIANGLE_FAN), {0, 0, 0}});
            curve_buffer[0] = 0.15f; curve_buffer[1] = 0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 271; ++i) {
                curve_buffer[3*i+3] = 0.15+(0.15*cos(M_PI*(float(i-90)/180.0f)))*0.5;
                curve_buffer[3*i+4] = 0.15+(0.15*sin(M_PI*(float(i-90)/180.0f)))*0.5;
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_BACKGROUND, curve_buffer, 272, GL_TRIANGLE_FAN), {0, 0, 0}});
            curve_buffer[0] = 0.15f; curve_buffer[1] = -0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 271; ++i) {
                curve_buffer[3*i+3] = 0.15+0.15*cos(M_PI*(float(i-180)/180.0f));
                curve_buffer[3*i+4] = -0.15+0.15*sin(M_PI*(float(i-180)/180.0f));
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_WHITE, curve_buffer, 272, GL_TRIANGLE_FAN), {0, 0.075, 0}});
            curve_buffer[0] = 0.15f; curve_buffer[1] = -0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 271; ++i) {
                curve_buffer[3*i+3] = 0.15+(0.15*cos(M_PI*(float(i-180)/180.0f)))*0.5;
                curve_buffer[3*i+4] = -0.15+(0.15*sin(M_PI*(float(i-180)/180.0f)))*0.5;
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_BACKGROUND, curve_buffer, 272, GL_TRIANGLE_FAN), {0, 0.075, 0}});
            this->set_position(this->x, this->y+0.4);
        } else if(c == '4') {
            GLfloat vertex_buffer_data[] = {
                0,0.2,0,
                0.3, 0.5,0,
                0.3, 0.2, 0,
                0.2, 0.3, 0,
                0.2, 0, 0,
                0.4, 0.3, 0
            };
            this->objects.push_back({Object(0, 0, COLOR_WHITE, vertex_buffer_data, 6), {0, 0, 0}});
            GLfloat vertex_buffer_array[] = {
                0.1,0.25,0,
                0.25, 0.4,0,
                0.25, 0.25, 0,
            };
            this->objects.push_back({Object(0, 0, COLOR_BACKGROUND, vertex_buffer_array, 3), {0.015, 0, 0}});
            GLfloat vertex_buffer_array2[] = {
                0.3,0.2,0,
                0.4, 0.2,0,
                0.3, 0, 0,
            };
            this->objects.push_back({Object(0, 0, COLOR_BACKGROUND, vertex_buffer_array2, 3), {-0.025, 0.04, 0}});
        } else if(c == '5') {
            GLfloat curve_buffer[272*3];
            curve_buffer[0] = 0.15f; curve_buffer[1] = -0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 271; ++i) {
                curve_buffer[3*i+3] = 0.15+0.15*cos(M_PI*(float(i-180)/180.0f));
                curve_buffer[3*i+4] = -0.15+0.15*sin(M_PI*(float(i-180)/180.0f));
                curve_buffer[3*i+5] = 0;
            };
            this->objects.push_back({Object(0, 0, COLOR_WHITE, curve_buffer, 272, GL_TRIANGLE_FAN), {0, 0.3, 0}});
            curve_buffer[0] = 0.15f; curve_buffer[1] = -0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 271; ++i) {
                curve_buffer[3*i+3] = 0.15+(0.15*cos(M_PI*(float(i-180)/180.0f)))*0.5;
                curve_buffer[3*i+4] = -0.15+(0.15*sin(M_PI*(float(i-180)/180.0f)))*0.5;
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_BACKGROUND, curve_buffer, 272, GL_TRIANGLE_FAN), {0, 0.3, 0}});
            GLfloat box_buffer[] = {
                0.15, 0.3, 0,
                0.05, 0.3, 0,
                0.05, 0.6, 0,
                0.15, 0.5, 0,
                0.05, 0.6, 0,
                0.15, 0.3, 0,
                0.05, 0.6, 0,
                0.15, 0.5, 0,
                0.3, 0.5, 0,
                0.05, 0.6, 0,
                0.3, 0.5, 0,
                0.3, 0.6, 0,
            };
            this->objects.push_back({Object(0, 0, COLOR_WHITE, box_buffer, 12), {0, -0.075, 0}});
        } else if(c == '6') {
            GLfloat curve_buffer[362*3];
            curve_buffer[0] = 0.15f; curve_buffer[1] = 0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 271; ++i) {
                curve_buffer[3*i+3] = 0.15+0.15*cos(M_PI*(float(i)/180.0f));
                curve_buffer[3*i+4] = 0.15+0.15*sin(M_PI*(float(i)/180.0f));
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_WHITE, curve_buffer, 272, GL_TRIANGLE_FAN), {0, 0, 0}});
            curve_buffer[0] = 0.15f; curve_buffer[1] = 0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 271; ++i) {
                curve_buffer[3*i+3] = 0.15+(0.15*cos(M_PI*(float(i)/180.0f)))*0.5;
                curve_buffer[3*i+4] = 0.15+(0.15*sin(M_PI*(float(i)/180.0f)))*0.5;
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_BACKGROUND, curve_buffer, 272, GL_TRIANGLE_FAN), {0, 0, 0}});
            curve_buffer[0] = 0.15f; curve_buffer[1] = -0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 361; ++i) {
                curve_buffer[3*i+3] = 0.15+0.15*cos(M_PI*(float(i)/180.0f));
                curve_buffer[3*i+4] = -0.15+0.15*sin(M_PI*(float(i)/180.0f));
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_WHITE, curve_buffer, 362, GL_TRIANGLE_FAN), {0, 0.075, 0}});
            curve_buffer[0] = 0.15f; curve_buffer[1] = -0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 361; ++i) {
                curve_buffer[3*i+3] = 0.15+(0.15*cos(M_PI*(float(i)/180.0f)))*0.5;
                curve_buffer[3*i+4] = -0.15+(0.15*sin(M_PI*(float(i)/180.0f)))*0.5;
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_BACKGROUND, curve_buffer, 362, GL_TRIANGLE_FAN), {0, 0.075, 0}});
            this->set_position(this->x, this->y+0.2);
        } else if(c == '7') {
            GLfloat box_buffer[] = {
                0.05, 0, 0,
                0.3, 0.4, 0,
                0.05, 0.4, 0,
            };
            this->objects.push_back({Object(0, 0, COLOR_WHITE, box_buffer, 3), {0, 0, 0}});
            GLfloat box_buffer2[] = {
                0.05, 0, 0,
                0.2, 0.3, 0,
                0.05, 0.3, 0,
            };
            this->objects.push_back({Object(0, 0, COLOR_BACKGROUND, box_buffer2, 3), {0, 0.05, 0}});
        } else if(c == '8') {
            GLfloat curve_buffer[362*3];
            curve_buffer[0] = 0.15f; curve_buffer[1] = 0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 361; ++i) {
                curve_buffer[3*i+3] = 0.15+0.15*cos(M_PI*(float(i)/180.0f));
                curve_buffer[3*i+4] = 0.15+0.15*sin(M_PI*(float(i)/180.0f));
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_WHITE, curve_buffer, 362, GL_TRIANGLE_FAN), {0, 0, 0}});
            curve_buffer[0] = 0.15f; curve_buffer[1] = 0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 361; ++i) {
                curve_buffer[3*i+3] = 0.15+(0.15*cos(M_PI*(float(i)/180.0f)))*0.5;
                curve_buffer[3*i+4] = 0.15+(0.15*sin(M_PI*(float(i)/180.0f)))*0.5;
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_BACKGROUND, curve_buffer, 362, GL_TRIANGLE_FAN), {0, 0, 0}});
            curve_buffer[0] = 0.15f; curve_buffer[1] = -0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 361; ++i) {
                curve_buffer[3*i+3] = 0.15+0.15*cos(M_PI*(float(i)/180.0f));
                curve_buffer[3*i+4] = -0.15+0.15*sin(M_PI*(float(i)/180.0f));
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_WHITE, curve_buffer, 362, GL_TRIANGLE_FAN), {0, 0.075, 0}});
            curve_buffer[0] = 0.15f; curve_buffer[1] = -0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 361; ++i) {
                curve_buffer[3*i+3] = 0.15+(0.15*cos(M_PI*(float(i)/180.0f)))*0.5;
                curve_buffer[3*i+4] = -0.15+(0.15*sin(M_PI*(float(i)/180.0f)))*0.5;
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_BACKGROUND, curve_buffer, 362, GL_TRIANGLE_FAN), {0, 0.075, 0}});
            this->set_position(this->x, this->y+0.6);
        } else if(c == '9') {
            GLfloat curve_buffer[362*3];
            curve_buffer[0] = 0.15f; curve_buffer[1] = 0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 361; ++i) {
                curve_buffer[3*i+3] = 0.15+0.15*cos(M_PI*(float(i)/180.0f));
                curve_buffer[3*i+4] = 0.15+0.15*sin(M_PI*(float(i)/180.0f));
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_WHITE, curve_buffer, 362, GL_TRIANGLE_FAN), {0, 0, 0}});
            curve_buffer[0] = 0.15f; curve_buffer[1] = 0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 361; ++i) {
                curve_buffer[3*i+3] = 0.15+(0.15*cos(M_PI*(float(i)/180.0f)))*0.5;
                curve_buffer[3*i+4] = 0.15+(0.15*sin(M_PI*(float(i)/180.0f)))*0.5;
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_BACKGROUND, curve_buffer, 362, GL_TRIANGLE_FAN), {0, 0, 0}});
            curve_buffer[0] = 0.15f; curve_buffer[1] = -0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 361; ++i) {
                curve_buffer[3*i+3] = 0.15+0.15*cos(M_PI*(float(i-180)/180.0f));
                curve_buffer[3*i+4] = -0.15+0.15*sin(M_PI*(float(i-180)/180.0f));
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_WHITE, curve_buffer, 272, GL_TRIANGLE_FAN), {0, 0.075, 0}});
            curve_buffer[0] = 0.15f; curve_buffer[1] = -0.15f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 361; ++i) {
                curve_buffer[3*i+3] = 0.15+(0.15*cos(M_PI*(float(i-180)/180.0f)))*0.5;
                curve_buffer[3*i+4] = -0.15+(0.15*sin(M_PI*(float(i-180)/180.0f)))*0.5;
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_BACKGROUND, curve_buffer, 272, GL_TRIANGLE_FAN), {0, 0.075, 0}});
            this->set_position(this->x, this->y+0.2);
        } else if (c == '0') {
            GLfloat curve_buffer[362*3];
            curve_buffer[0] = 0.3f; curve_buffer[1] = -0.3f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 361; ++i) {
                curve_buffer[3*i+3] = 0.3+0.3*cos(M_PI*(float(i)/180.0f));
                curve_buffer[3*i+4] = -0.3+0.3*sin(M_PI*(float(i)/180.0f));
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_WHITE, curve_buffer, 362, GL_TRIANGLE_FAN), {0, 0.6, 0}});
            curve_buffer[0] = 0.3f; curve_buffer[1] = -0.3f; curve_buffer[2] = 0.0f;
            for(int i = 0; i < 361; ++i) {
                curve_buffer[3*i+3] = 0.3+(0.3*cos(M_PI*(float(i)/180.0f)))*0.75;
                curve_buffer[3*i+4] = -0.3+(0.3*sin(M_PI*(float(i)/180.0f)))*0.75;
                curve_buffer[3*i+5] = 0;
            }
            this->objects.push_back({Object(0, 0, COLOR_BACKGROUND, curve_buffer, 362, GL_TRIANGLE_FAN), {0, 0.6, 0}});
        }
    }
};

#endif // BALL_H
