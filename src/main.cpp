#include "main.h"
#include "timer.h"
#include "ball.h"
#include "packman.h"
#include "floor.h"
#include "pond.h"
#include "trampoline.h"
#include "magnet.h"
#include "procupines.h"
#include "packmanslope.h"
#include "bits/stdc++.h"
#include "pthread.h"


using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

vector <pair <int, color_t> > Color;
Ball ball1;
Packman packmans[20];
Floor floor1;
Pond pond1;
Magnet magnet1, magnet2;
Trampoline tramp1;
Procupines proc1, proc2;
PackmanSlope pack1, pack2;
int score = 0;
int period1 = rand() % 80;
int period2 = rand() % 80;
double xpos;
double ypos;
float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;

Timer t60(1.0 / 60);
GLint64 timer;

//void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
//    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
//        break;
//    }
//}

void sleep(unsigned int mseconds) {
    clock_t goal = mseconds + clock();
    while (goal > clock());
}


char * ConvertIntToString(int n) {
    int a = 10, i = 0;
    char c[10] = " ";
    while(n) {
        c[i++] = ((n % 10) + '0');
        a = a * 10;
        n = n / 10;
    }
    int l = strlen(c), k = 0;
    char * s = (char *)calloc(l, sizeof(char));
    for(int j = l - 1; j >= 0; j--) {
        s[k++] = c[j];
    }
    return s;
}

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram (programID);
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane
    glm::mat4 VP = Matrices.projection * Matrices.view;
    glm::mat4 MVP;  // MVP = Projection * View * Model
    // Scene render
    floor1.draw(VP);
    pond1.draw(VP);
    tramp1.draw(VP);
    for(int i = 0; i < 20; i++) {
        packmans[i].draw(VP);
    }
    pack1.draw(VP);
    pack2.draw(VP);
    timer = timer/1000000000;
    if(timer % 100 > period1 && timer % 100 < period1 + 20) {
        magnet1.draw(VP);
        magnet2.draw(VP);
        magnet1.magneticEffect = 1;
    } else {
        magnet1.magneticEffect = 0;
    }
    if(period2 % 3 == 0 && timer % 100 > period2 && timer % 100 < period2 + 10){
        proc1.draw(VP);
    }
    if(period2 % 6 == 0 && timer % 100 > period2 + 5 && timer % 100 < period2 + 15){
        proc2.draw(VP);
    }
    ball1.draw(VP);
}
void draw2() {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram (programID);
    Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane
    glm::mat4 VP = Matrices.projection * Matrices.view;
    glm::mat4 MVP;  // MVP = Projection * View * Model
    // Scene render
    ball1.draw(VP);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up  = glfwGetKey(window, GLFW_KEY_UP);
    int a  = glfwGetKey(window, GLFW_KEY_A);
    int d  = glfwGetKey(window, GLFW_KEY_D);
    int space  = glfwGetKey(window, GLFW_KEY_SPACE);
    if (left || a) {
        if(ball1.position.y < -0.85){
            ball1.position.x -= 0.006;
        } else {
            ball1.position.x -= 0.04;
        }
    }
    if (right || d) {
        if(ball1.position.y < -0.85){
            ball1.position.x += 0.006;
        } else {
            ball1.position.x += 0.04;
        }
    }
    if ((up || space) && !(ball1.position.y > -0.8000)) {
        if(ball1.position.y < -0.85){
            ball1.velocity.y = 0.1;
        } else {
            ball1.velocity.y = 0.2;
        }
        ball1.position.y += ball1.velocity.y;
    }
    glfwSetScrollCallback(window, scroll_callback);
}

void tick_elements() {
    srand(time(0));
    ball1.tick();
    for(int i = 0; i < 20; i++) {
        packmans[i].tick();
    }
    pack1.tick();
    pack2.tick();
    //inside pond
    if(pond1.inside_pond(ball1.bounding_box()) && ball1.position.y < -0.76) {
        ball1.position.y = -sqrt(0.6 * 0.6 - (ball1.position.x - pond1.position.x) * (ball1.position.x - pond1.position.x)) + pond1.position.y;
    }

    if(!pond1.inside_pond(ball1.bounding_box()) && ball1.position.y < -0.8) {
        ball1.position.y = -0.8;
    }
    //on procupines
    if(proc1.on_procupines(ball1.position.x, ball1.position.y) && period2 % 3 == 0 && timer % 100 > period2 && timer % 100 < period2 + 10) {
        score = score - 1;
    }
    if(proc2.on_procupines(ball1.position.x, ball1.position.y) && period2 % 6 == 0 && timer % 100 > period2 + 5 && timer % 100 < period2 + 15) {
        score = score - 1;
    }

    if(tramp1.on_trampoline(ball1.bounding_box())) {
        ball1.velocity.y = 0.3;
    }

    //under magnetic field
    if(magnet1.magneticEffect) {
        glm::vec3 acc = magnet1.Force_due_to_magnet(ball1.position.x, ball1.position.y);
        ball1.acceleration.x = acc.x;
        ball1.acceleration.y = acc.y;
    }

    for(int i = 0; i < 20; i++) {
        if (detect_collision(ball1.bounding_box(), packmans[i].bounding_box())) {
            ball1.velocity.y = 0.2;
            packmans[i].position.x = (float)(rand() % 70 + 40)/10;
            packmans[i].position.y =  (float)(rand() % 100 + 40) / 100;
            score = score + 10;
        }
    }
    if(pack1.collision(ball1.position.x, ball1.position.y, ball1.radius)) {
        ball1.velocity.x = -0.05;
        ball1.velocity.y = 0.15;
        pack1.set_position(-9, 0.8);
        score = score + 15;
    }
    if(pack2.collision(ball1.position.x, ball1.position.y, ball1.radius)) {
        ball1.velocity.x = -0.05;
        ball1.velocity.y = 0.15;
        pack2.set_position(-5, 1.6);
        score = score + 15;
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */

void initGL(GLFWwindow *window, int width, int height) {
    srand(time(NULL));
    /* Objects should be created before any other gl function and shaders */
    // Create the models
    ball1 = Ball(-2.6, -0.8, COLOR_RED, 0.2);
    floor1 = Floor(0, 0, COLOR_GREEN);
    pond1 = Pond(-1.5, -1, COLOR_BLUE);
    tramp1 = Trampoline(2, 0, COLOR_ORANGE);
    for(int i = 0; i < 20; i++) {
        float y = (float)(rand() % 100 + 40) / 100;
        float x = (float)(rand() % 70 + 40)/10;
        int c = rand() % 9;
        packmans[i] = Packman(-x, y, Color[c].second, 0.15);
    }
    pack1 = PackmanSlope(1, 0.8, COLOR_PINK, 0.15);
    pack2 = PackmanSlope(-5, 1.6, COLOR_BLUE, 0.15);
    int s = rand();
    if(s % 2 == 0) {
        magnet1 = Magnet(-2, 2, COLOR_BLACK, 0.5, -6);
        magnet2 = Magnet(-2, 2, COLOR_BACKGROUND, 0.3, -6);
    } else {
        magnet1 = Magnet(2, 2, COLOR_BLACK, 0.5, 6);
        magnet2 = Magnet(2, 2, COLOR_BACKGROUND, 0.3, 6);
    }
    proc1 = Procupines(-0.4, -1, COLOR_VIOLET, 3);
    proc2 = Procupines(-3.5, -1, COLOR_VIOLET, 2);
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
    Color.push_back(make_pair(0, COLOR_RED)); Color.push_back(make_pair(1, COLOR_BLACK));
    Color.push_back(make_pair(2, COLOR_BLUE)); Color.push_back(make_pair(3, COLOR_GREEN));
    Color.push_back(make_pair(4, COLOR_PINK)); Color.push_back(make_pair(5, COLOR_ORANGE));
    Color.push_back(make_pair(6, COLOR_DBLUE)); Color.push_back(make_pair(7, COLOR_BGREEN));
    Color.push_back(make_pair(8, COLOR_VIOLET));
    char * s = ConvertIntToString(score);
    int width  = 900;
    int height = 900;

    window = initGLFW(width, height);

    initGL (window, width, height);
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
        // Process timers
        if (t60.processTick()) {
            glGetInteger64v(GL_TIMESTAMP, &timer);
            draw();
            glfwSetWindowTitle(window, ConvertIntToString(score));

            if(score < 0) {
                glfwSetWindowTitle(window, "Game Over!");
                sleep(900000);
                break;
            }
//            glfwGetCursorPos(window, &xpos, &ypos);
//            glfwSetMouseButtonCallback(window, mouse_button_callback);
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

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) < 0.1) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height)) && (a.y - b.y >= 0.2);
}

void reset_screen() {
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}
