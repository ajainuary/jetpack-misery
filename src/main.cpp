#include "main.h"
#include "timer.h"
#include "object.h"
#include "handlers.hpp"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Player p;
Platform ground;
deque <Coin> coins;
FireLine test;
Ring taki;
deque <Water> fountain;
GLfloat coin_vertex_buffer_data[362*3];
Display text, text2;
float screen_zoom = 1, screen_center_x = 6, screen_center_y = 3;
float camera_rotation_angle = 0;
float position = 0.0f;
Timer t60(1.0 / 60);
int score = 0;
/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);
    /* -- 3D Specific Code --
    // Eye - Location of camera. Don't change unless you are sure!!
    glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (0, 0, 0);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);
    */

    // Compute Camera matrix (view)
    //  Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    Matrices.view = glm::lookAt(glm::vec3(position, 0, 3), glm::vec3(position, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    test.draw(VP);
//    ok.draw(VP);
    p.draw(VP);
    ground.draw(VP);
    taki.draw(VP);
//    text.draw(VP);
//    text2.draw(VP);
    draw_collection(coins.begin(), coins.end(), VP);
    draw_collection(fountain.begin(), fountain.end(), VP);
}

void game_over(Player &p) {
   cout << "Game Over" << endl;
   exit(0);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    if(up) {
        p.joy = true;
//        p.set_position(p.x, p.y+0.01f);
    }
    if(right) {
        p.set_position(p.x+0.01f, p.y);
    }
    if(left) {
        p.set_position(p.x-0.01f, p.y);
    }
    if(down) {
        p.set_position(p.x, p.y-0.01f);
    }
    if(space && rand() % 8 == 0) {
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
        fountain.push_back(Water(p.x, p.y+0.5-(float(rand())/(3*float(RAND_MAX))), water_vertex_buffer));
    }
}

void tick_elements() {
    p.tick();
    //Coin collection
//    for (auto it = find_collision(coins.begin(), coins.end(), p, position); it != coins.end();it = find_collision(it, coins.end(), p, position)) {
//        score += it->value;
//        it = coins.erase(it);
//    }
    for (auto it = fountain.begin(); it != fountain.end(); (it->tick()) ? it = fountain.erase(it) : ++it);
    //Combo test
    if(collides(p, test))
        --p.lives;
//    if(test.detect(p.box))
//        game_over(p);
    //Coin Spawning
//    int coin_rand = rand();
//    if(coin_rand < RAND_MAX/50)
//        coins.push_back(Coin(position + 20, coin_rand%8, (coin_rand%2 == 0) ? 1 : 2, (coin_rand % 2 == 0) ? COLOR_FAWN : COLOR_YELLOW, coin_vertex_buffer_data, 362));
//    position += 0.075f;
//    p.position.x += 0.075f;
    //Boomerang test
//    taki.tick();
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    GLfloat player_vertex_buffer_data[] = {
                -1.0f,-1.0f,-1.0f, // triangle 1 : begin
                -1.0f,-1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f, // triangle 1 : end
                1.0f, 1.0f,-1.0f, // triangle 2 : begin
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f,-1.0f, // triangle 2 : end
                1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f,-1.0f,
                1.0f,-1.0f,-1.0f,
                1.0f, 1.0f,-1.0f,
                1.0f,-1.0f,-1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f,-1.0f,
                1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f, 1.0f,
                -1.0f,-1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f,-1.0f, 1.0f,
                1.0f,-1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f,-1.0f,-1.0f,
                1.0f, 1.0f,-1.0f,
                1.0f,-1.0f,-1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f,-1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f,-1.0f,
                -1.0f, 1.0f,-1.0f,
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f,-1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f,-1.0f, 1.0f
    };
    p = Player(2,2, 0, -0.075f/60.0f, 0, 0);
//    ok = Combo(2, 2);
//    ok.objects.push_back({Object(0, 0, COLOR_PINK, player_vertex_buffer_data, 12*3), {0, 0, 0}});
    float width_platform = 5000.0f;
    GLfloat platform_vertex_buffer_data [] = {
        -width_platform,-1,0,
        width_platform,-1,0,
        -width_platform,-2,0,
        width_platform,-1,0,
        width_platform,-2,0,
        -width_platform,-2,0
};
    ground = Platform(COLOR_SECONDARY_PINK, platform_vertex_buffer_data);
    create_ellipse(0.175, 0.25, coin_vertex_buffer_data);
    test = FireLine(3, 4, 1);
    taki = Ring(8,4);
    text = Display(6, 5, '0');
    text2 = Display(6.8, 5, '0');
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");

    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 1600;
    int height = 900;

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

void reset_screen() {
    float top    = screen_center_y + 4.5f / screen_zoom;
    float bottom = screen_center_y - 4.5f / screen_zoom;
    float left   = screen_center_x - 8 / screen_zoom;
    float right  = screen_center_x + 8 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
