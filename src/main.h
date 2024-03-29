#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <array>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct color_t {
    int r;
    int g;
    int b;
};

// nonedit.cpp
GLFWwindow *initGLFW(int width, int height);
GLuint     LoadShaders(const char *vertex_file_path, const char *fragment_file_path);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat *color_buffer_data, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const color_t color, GLenum fill_mode = GL_FILL);
void       draw3DObject(struct VAO *vao);

// input.cpp
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods);
void keyboardChar(GLFWwindow *window, unsigned int key);
void mouseButton(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

// other_handlers.cpp
void error_callback(int error, const char *description);
void quit(GLFWwindow *window);
void reshapeWindow(GLFWwindow *window, int width, int height);

// Types
struct VAO {
    GLuint VertexArrayID; //Unique identifier
    GLuint VertexBuffer; //Associated Vertex Buffer
    GLuint ColorBuffer; //Associated Color Buffer

    GLenum PrimitiveMode; //OpenGL Primitive used to render
    GLenum FillMode;
    int    NumVertices;
};
typedef struct VAO VAO;

struct GLMatrices {
    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    GLuint    MatrixID;
};

extern GLMatrices Matrices;

// ---- Logic ----

enum direction_t { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT };

struct bounding_box_t {
    float x;
    float y;
    float width;
    float height;
};

bool detect_collision(bounding_box_t a, bounding_box_t b);

extern float screen_zoom, screen_center_x, screen_center_y;
void reset_screen();

// ---- Colors ----
extern const color_t COLOR_RED;
extern const color_t COLOR_GREEN;
extern const color_t COLOR_BLACK;
extern const color_t COLOR_BACKGROUND;
extern const color_t COLOR_PINK;
extern const color_t COLOR_SECONDARY_PINK;
extern const color_t COLOR_YELLOW;
extern const color_t COLOR_FAWN;
extern const color_t COLOR_GREY;
extern const color_t COLOR_WOOD;
extern const color_t COLOR_SECONDARY_WOOD;
extern const color_t COLOR_WATER;
extern const color_t COLOR_WHITE;
extern const color_t COLOR_ORANGE;
extern const color_t COLOR_DRAGONITE;
// ---- Helpers ----
extern float min(float x, float y);
extern float max(float x, float y);
extern bool detect_collision(bounding_box_t a, bounding_box_t b);
extern void add_shapes(GLfloat first[], int first_size, GLfloat second[], int second_size, GLfloat answer[], float x, float y); //Primitive Mode: GL_TRIANGLES only
// ---- Shape Creators ----
extern void create_ellipse(float a, float b, GLfloat vertex_buffer_data[], float origin_x = 0, float origin_y = 0);
extern void create_heart(GLfloat vertex_buffer_data[]);
// ---- Collision Detection ----
typedef struct point {
  float x, y;
} point;

typedef struct line {
  float a, b, c;
  // of the form ax+by+c=0
} line;

extern line passing(point p, point b);
extern bool intersect(point a, point b, point c, point d);
extern bool triangle_intersect(point A[], point B[]);
extern bool poly_intersect(const std::vector<GLfloat> &A, int vertices_A, float origin_A_x, float origin_A_y, const std::vector<GLfloat> &B, int vertices_B, float origin_B_x, float origin_B_y);
#endif
