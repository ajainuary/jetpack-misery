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
bool randi;
deque<FireLine> firelines;
deque<FireBeam> firebeams;
deque<FlyingObject> fos;
deque<Magnet> mags;
deque<Ring> rings;
deque<Water> fountain;
deque<Boomerang> booms;
deque<Dragon> chus;
GLfloat coin_vertex_buffer_data[362*3];
float screen_zoom = 1, screen_center_x = 6, screen_center_y = 3;
float top_corner, left_corner, bottom_corner, right_corner;
float cam_y = 0;
float camera_rotation_angle = 0;
float position = 0.0f;
Timer t60(1.0 / 60);
int score = 0;
int timer = -1;
array <array <Display, 10>, 3> score_display;
array <FlyingObject, 13> life_display;
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
    if(p.y < bottom_corner + 1)
        cam_y = p.y - bottom_corner - 1;
    if(p.y > screen_center_y)
        cam_y = p.y - screen_center_y;
    Matrices.view = glm::lookAt(glm::vec3(position, cam_y, 3), glm::vec3(position, cam_y, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    draw_collection(mags.begin(), mags.end(), VP);
    draw_collection(rings.begin(), rings.end(), VP);
    draw_collection(firelines.begin(), firelines.end(), VP);
    draw_collection(firebeams.begin(), firebeams.end(), VP);
    draw_collection(coins.begin(), coins.end(), VP);
    ground.draw(VP);
    p.draw(VP);
    for (int i = 0;i < p.lives; ++i) {
        life_display[i].draw(glm::scale(glm::vec3(1/screen_zoom, 1/screen_zoom, 1))*VP);
    }
    score_display[0][score / 100].draw(glm::scale(glm::vec3(1/screen_zoom, 1/screen_zoom, 1))*VP);
    score_display[1][(score / 10) % 10].draw(glm::scale(glm::vec3(1/screen_zoom, 1/screen_zoom, 1))*VP);
    score_display[2][score % 10].draw(glm::scale(glm::vec3(1/screen_zoom, 1/screen_zoom, 1))*VP);
    draw_collection(fos.begin(), fos.end(), VP);
    draw_collection(booms.begin(), booms.end(), VP);
    draw_collection(fountain.begin(), fountain.end(), VP);
    draw_collection(chus.begin(), chus.end(), VP);
    for(auto it = chus.begin(); it != chus.end(); ++it)
        draw_collection(it->fountain.begin(), it->fountain.end(), VP);
}

void game_over(Player &p) {
    if(timer > 0)
        return;
    if(p.lives > 0 && !p.invincible && timer <= 0)
    {
        --p.lives;
        p.invincible = true;
        timer = 60;
//        cout << "set timer to 360" << endl;
        return;
    }
//   cout << "Game Over called " << score << endl;
   exit(0);
}

void tick_input(GLFWwindow *window) {
    randi = false;
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int anti_g = glfwGetKey(window, GLFW_KEY_G);
    if(up) {
        p.joy = true;
    }
    if(right) {
        p.set_position(p.x+p.speed, p.y);
        randi = true;
    }
    if(left) {
        p.set_position(p.x-p.speed, p.y);
    }
    if(down) {
        p.set_position(p.x, p.y-0.02f);
    }
    if(anti_g)
    {
        p.gravity = !p.gravity;
    }
    if(space && rand() % 10 == 0) {
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
    for(auto it = firebeams.begin(); it != firebeams.end(); ++it)
        it->tick();
    for (auto it = fos.begin(); it != fos.end(); ++it)
        it->tick();
    for(auto it = booms.begin(); it != booms.end(); ++it)
        it->tick();
    position = p.x-2*screen_zoom;

}

void tick_elements() {
    if(timer == 0)
    {
        p.invincible = false;
        timer = -1;
//        cerr << "falsify" << endl;
    }
    else if(timer > 0){
//        cerr <<"Timer: "<< timer << endl;
        --timer;
    }
    p.tick();
    for (auto it = fountain.begin(); it != fountain.end(); (it->tick()) ? it = fountain.erase(it) : ++it);
    //Coin Spawning
    if(randi) {
        int coin_rand = rand();
        if(coin_rand < (RAND_MAX/50)*(p.speed/0.06f))
            coins.push_back(Coin(position + 20, coin_rand%8, (coin_rand%2 == 0) ? 1 : 2, (coin_rand % 2 == 0) ? COLOR_FAWN : COLOR_YELLOW, coin_vertex_buffer_data, 362));
        int fire_rand = rand();
        if(fire_rand < (RAND_MAX/150)*(p.speed/0.06f))
        {
            firelines.push_back(FireLine(position + 20, fire_rand%8, fire_rand%3));
            for (auto it = firebeams.begin(); it != firebeams.end(); ++it)
                if(collides_combo(*firelines.rbegin(), *it))
                {
                    firelines.pop_back();
                }
        }
        else if(fire_rand < (((RAND_MAX/150)+(RAND_MAX/300))*(p.speed/0.06f)))
        {

            firebeams.push_back(FireBeam(position + 20));
            for (auto it = firelines.begin(); it != firelines.end(); ++it)
                if(collides_combo(*firebeams.rbegin(), *it))
                {
                    firebeams.pop_back();
                }
        }
        int fo_rand = rand();
        if(fo_rand < (RAND_MAX/500)*(p.speed/0.06f))
            fos.push_back(FlyingObject(position+20, 3.5, fo_rand%4));
        int boom_rand = rand();
        if(boom_rand < RAND_MAX/500)
            booms.push_back(Boomerang(position+14, 3.5, position+7, 3.5, 7, 3.5));
        int mag_ring_rand = rand();
        if(mag_ring_rand < RAND_MAX/400)
        {
            if(mag_ring_rand % 2 == 0) {
                mags.push_back(Magnet(position+20, 3.5+(mag_ring_rand%3)));
                for (auto it = firelines.begin(); it != firelines.end(); ++it)
                    if(collides_combo(*mags.rbegin(), *it))
                    {
                        mags.pop_back();
                    }
            } else {
                rings.push_back(Ring(position+20, 3.5+(mag_ring_rand%2)));
                for (auto it = firelines.begin(); it != firelines.end(); ++it)
                    if(collides_combo(*rings.rbegin(), *it))
                    {
                        rings.pop_back();
                    }
            }
        }
        int drag_rand = rand();
        if(drag_rand < RAND_MAX/1000)
        {
            chus.push_back(Dragon(position + 20, 1.5));
        }
    }
    //Collision killing
    for(auto i = fountain.begin(); i != fountain.end(); ++i)
    {
        bool kill = true;
        for(auto j = firebeams.begin(); j != firebeams.end(); ++j)
            if(collides(*i, *j))
            {
                 i = fountain.erase(i);
                 --i;
                 kill = false;
                 j = firebeams.erase(j);
                 --j;
                break;
            }
        for(auto j = firelines.begin(); j != firelines.end() && kill; ++j)
            if(collides(*i, *j))
            {
                i = fountain.erase(i);
                --i;
                j = firelines.erase(j);
                --j;
                kill = false;
               break;
            }
        for(auto j = chus.begin(); j != chus.end() && kill; ++j)
            if(collides(*i, *j) && rand() % 25) {
                i = fountain.erase(i);
                --i;
                j = chus.erase(j);
                --j;
                break;
            }

    }
    for(auto j = firebeams.begin(); j != firebeams.end(); ++j)
        if(j->x < position - 5)
        {
             j = firebeams.erase(j);
             --j;
            break;
        }
    for(auto j = firelines.begin(); j != firelines.end(); ++j)
        if(j->x < position - 5)
        {
            j = firelines.erase(j);
            --j;
           break;
        }
    for(auto j = coins.begin(); j != coins.end(); ++j)
        if(j->position.x < position - 5)
        {
            j = coins.erase(j);
            --j;
           break;
        }
    //Player killing
    for(auto j = firebeams.begin(); j != firebeams.end() && !p.invincible; ++j)
        if(collides_combo(p, *j))
            game_over(p);
    for(auto j = firelines.begin(); j != firelines.end() && !p.invincible; ++j)
        if(collides_combo(p, *j))
            game_over(p);
    for(auto j = booms.begin(); j != booms.end() && !p.invincible; ++j)
        if(collides_combo(p, *j))
            game_over(p);
        //Coin collection
    for(auto j = coins.begin(); j != coins.end(); ++j)
    {
        if(collides(*j, p))
        {
            score += j->value;
            j = coins.erase(j);
            --j;
        }
    }
    //fos
    for (auto j = fos.begin(); j != fos.end(); ++j) {
        if(collides_combo(p, *j)) {
            if(j->type == 0)
                p.speed /=2;
            else if(j->type == 1)
                p.speed *= 2;
            else if(j->type == 2)
                ++p.lives;
            else if(j->type == 3)
            {
                p.invincible = true;
                timer = 3600;
            }
            j = fos.erase(j);
            --j;
        }
    }
    for(auto it = mags.begin(); it != mags.end(); ++it)
        it->tick(p);
    for(auto it = rings.begin(); it != rings.end(); ++it)
        it->tick(p);
    for (int i = 0;i < 3; ++i) {
        for (int j = 0;j < 10; ++j) {
            score_display[i][j].set_position(position+11+0.8*i, 6.7+cam_y);
        }
    }
    for(auto j = chus.begin(); j != chus.end(); ++j)
    {
        j->tick();
        for (auto it = j->fountain.begin(); it != j->fountain.end(); (it->tick()) ? it = j->fountain.erase(it) : ++it);
        if(collides_combo(*j, p) && !p.invincible) {
            game_over(p);
        }
        for(auto it = j->fountain.begin(); it != j->fountain.end() && !p.invincible; ++it)
        {
            if(collides(*it, p))
            {
                game_over(p);
                it = j->fountain.erase(it);
                --it;
            }
        }
    }
    for (int i = 0;i < 13; ++i) {
        life_display[i].set_position(position-1+0.9*i, 7+cam_y);
    }


}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    timer = -1;
    cam_y = 0;
    p = Player(2,2, 0, -0.075f/60.0f, 0, 0);
    fos.push_back(FlyingObject(5,4,3));
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
    for (int i = 0;i < 3; ++i) {
        for (int j = 0;j < 10; ++j) {
            score_display[i][j] = Display(8+0.8*i, 7, '0'+j);
        }
    }
    for (int i = 0;i < 13; ++i) {
        life_display[i] = FlyingObject(-1+0.9*i, 7, 2);
    }
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
     top_corner    = screen_center_y + 4.5f / screen_zoom;
     bottom_corner = screen_center_y - 4.5f / screen_zoom;
     left_corner   = screen_center_x - 8 / screen_zoom;
     right_corner  = screen_center_x + 8 / screen_zoom;
    Matrices.projection = glm::ortho(left_corner, right_corner, bottom_corner, top_corner, 0.1f, 500.0f);
}
