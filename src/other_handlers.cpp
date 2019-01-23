#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "main.h"

using namespace std;

void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void quit(GLFWwindow *window) {
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow(GLFWwindow *window, int width, int height) {
    int fbwidth = width, fbheight = height;
    /* With Retina display on Mac OS X, GLFW's FramebufferSize
       is different from WindowSize */
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);

    // GLfloat fov = 90.0f;

    // sets the viewport of openGL renderer
    glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);

    // set the projection matrix as perspective
    /* glMatrixMode (GL_PROJECTION);
           glLoadIdentity ();
           gluPerspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1, 500.0); */
    // Store the projection matrix in a variable for future use
    // Perspective projection for 3D views
    // Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f);

    // Ortho projection for 2D views
    reset_screen();
}

float max(float x, float y) {
    if(y < x) return x;
    return y;
}

float min(float x, float y) {
    if(x < y) return x;
    return y;
}

void create_ellipse(float a, float b, GLfloat vertex_buffer_data[], float origin_x, float origin_y) {
    /* The standard ellipse contains 361 points on it's boundary and 1 at the origin */
    vertex_buffer_data[0] = origin_x; vertex_buffer_data[1] = origin_y; vertex_buffer_data[2] = 0.0f;
    for(int i = 0; i < 361; ++i) {
        vertex_buffer_data[3*i+3] = origin_x + a*cos(M_PI*(float(i)/180.0f));
        vertex_buffer_data[3*i+4] = origin_y + b*sin(M_PI*(float(i)/180.0f));
        vertex_buffer_data[3*i+5] = 0;
    }
}

void add_shapes(GLfloat first[], int first_size, GLfloat second[], int second_size, GLfloat answer[], float x, float y) {
    int i = 0;
    for(; i < 3*first_size; ++i)
        answer[i] = first[i];
    for(int j = 0; j < second_size; ++j)
    {
        answer[i+3*j] = second[3*j]+x;
        answer[i+3*j+1] = second[3*j+1]+y;
        answer[i+3*j+2] = second[3*j+2];
    }
    return;
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}
