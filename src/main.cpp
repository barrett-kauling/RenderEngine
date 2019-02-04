/*
 * To change this license header, choose License Headers
 * in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: Barrett Kauling
 *
 * Created on January 18, 2016, 1:10 PM
 */


#include "RenderEngine.h"

#include <GL/glew.h>
#include <GL/freeglut.h>



RenderEngine* display;


void Init_Mesh(void);
void Render_Mesh(void);


//default handlers
void Process_Keyboard(unsigned char, int, int);
void Process_Function(int, int, int);
void Process_Timer(int);


int main(int argc, char** argv) {

    RenderGLData* rd = new RenderGLData;
    rd->Clear();
    rd->argc = argc;
    rd->argv = argv;
    rd->rf->render = *Render_Mesh;
    rd->rf->timer = *Process_Timer;
    rd->rf->keyboard = *Process_Keyboard;
    rd->rf->function = *Process_Function;

    //declare an instance of the graphics class
    display = new RenderEngine(rd);

    display->SetReadPageSize(false);

    display->SetDirectory("");
    display->AddVertexShader("resource/shader.vs");
    display->AddFragmentShader("resource/shader.fs");
    display->Compile();

    display->SetInitialView(glm::mat4(1.0f),
                            glm::vec3(0.0f, 5.0f, 10.0f),
                            glm::vec3(0.0f,  0.0f,  0.0f),
                            glm::vec3(0.0f,  1.0f,  0.0f));
    display->SetPerspective(45.0f, (float)4.0f/(float)3.0f, 0.1f, 100.0f);

    display->Clear();
    display->SwapBuffers();

    Init_Mesh();

    //call our mainloop
    display->MainLoop();

    delete display;
    delete rd;

    return 0;

}

void Init_Mesh(void)
{

//    display->SetDirectory("../Resources/DSN 34/");
//    display->GenerateMesh("34M_17.obj");

//    display->SetDirectory("../Resources/Kepler/");
//    display->GenerateMesh("kepler.obj");

//    display->SetDirectory("../Resources/Aquarius/");
//    display->GenerateMesh("Aquarius_DEP.obj");

//    display->SetDirectory("../Resources/Crawler/");
//    display->GenerateMesh("crawler.obj");

//    display->SetDirectory("../Resources/Z2/");
//    display->GenerateMesh("Z2.obj");

//    display->SetDirectory("../Resources/HDU/");
//    display->GenerateMesh("HDU-L1.obj");

//    display->SetDirectory("../Resources/HDU/");
//    display->GenerateMesh("HDU-L2.obj");

//    display->SetDirectory("../Resources/Blitz/");
//    display->GenerateMesh("BLITZ-A.obj");

//    display->SetDirectory("../Resources/ISS/");
//    display->GenerateMesh("ISS_2016.obj");

    display->SetDirectory("./resource/MMSEV/");
    display->GenerateMesh("MMSEV-C.obj");

//    display->SetDirectory("../Resources/");
//    display->GenerateMesh("untitled3.obj");
//    display->GenerateMesh("untitled2.obj");
//    display->GenerateMesh("untitled.obj");
//    display->GenerateMesh("materials.obj");
//    display->GenerateMesh("wavefront.obj");

}

void Render_Mesh(void)
{

    display->Clear();

    display->RenderObjects();

    display->SwapBuffers();

}

void Process_Timer(int Value)
{

    display->Redisplay();

    display->ResetTimer(10, Value);

}

void Process_Keyboard(unsigned char Key, int x, int y)
{

    bool debug = true;

    switch (Key)
    {

        //leave main loop
        case 'q':
            glutLeaveMainLoop();
            break;

        //rotate about axis

        case '5': //x axis left
            if (debug) std::cout << "5 : X - CCW" << std::endl;
            display->TransformRotate(15.0f, glm::vec3(  1.00f,  0.00f,  0.00f));
            break;
        case '8': //x axis right
            if (debug) std::cout << "8 : X - CW" << std::endl;
            display->TransformRotate(15.0f, glm::vec3( -1.00f,  0.00f,  0.00f));
            break;

        case '7': //y axis counterclockwise
            if (debug) std::cout << "7 : Y - CCW" << std::endl;
            display->TransformRotate(15.0f, glm::vec3(  0.00f,  1.00f,  0.00f));
            break;

        case '9': //y axis clockwise
            if (debug) std::cout << "9 : Y - CW" << std::endl;
            display->TransformRotate(15.0f, glm::vec3(  0.00f, -1.00f,  0.00f));
            break;

        case '4': //z axis up
            if (debug) std::cout << "4 : Z - CCW" << std::endl;
            display->TransformRotate(15.0f, glm::vec3(  0.00f,  0.00f,  1.00f));
            break;

        case '6': //z axis down
            if (debug) std::cout << "6 : Z - CW" << std::endl;
            display->TransformRotate(15.0f, glm::vec3(  0.00f,  0.00f, -1.00f));
            break;

        case '0': //reset view to initial transform
            if (debug) std::cout << "0 : RESET" << std::endl;
            display->ResetView();
            break;

        //scale
        case '+':
            if (debug) std::cout << "+ : Zoom In" << std::endl;
            display->TransformScale(glm::vec3(  1.25f,  1.25f,  1.25f));
            break;

        case '-':
            if (debug) std::cout << "- : Zoom Out" << std::endl;
            display->TransformScale(glm::vec3(  0.75f,  0.75f,  0.75f));
            break;

    }

    display->Redisplay();

}

void Process_Function(int Key, int x, int y)
{

    bool debug_function = true;
    bool debug_key = true;

    switch (Key)
    {

        //modify draw color
        case GLUT_KEY_F1:
            if (debug_function) std::cout << "F1 - Draw Mode: " << F1 << std::endl;
            display->SetDrawMode(F1);
            break;

        case GLUT_KEY_F2:
            if (debug_function) std::cout << "F2 - Draw Mode: " << F2 << std::endl;
            display->SetDrawMode(F2);
            break;

        case GLUT_KEY_F3:
            if (debug_function) std::cout << "F3 - Draw Mode: " << F3 << std::endl;
            display->SetDrawMode(F3);
            break;

        case GLUT_KEY_F4:
            if (debug_function) std::cout << "F4 - Draw Mode: " << F4 << std::endl;
            display->SetDrawMode(F4);
            break;

        case GLUT_KEY_F5:
            if (debug_function) std::cout << "F5 - Draw Mode: " << F5 << std::endl;
            display->SetDrawMode(F5);
            break;

        case GLUT_KEY_F6:
            if (debug_function) std::cout << "F6 - Draw Mode: " << F6 << std::endl;
            display->SetDrawMode(F6);
            break;

        case GLUT_KEY_F7:
            if (debug_function) std::cout << "F7 - Draw Mode: " << F7 << std::endl;
            display->SetDrawMode(F7);
            break;

        case GLUT_KEY_F8:
            if (debug_function) std::cout << "F8 - Draw Mode: " << F8 << std::endl;
            display->SetDrawMode(F8);
            break;

        case GLUT_KEY_F9:
            if (debug_function) std::cout << "F9 - Draw Mode: " << F9 << std::endl;
            display->SetDrawMode(F9);
            break;

        case GLUT_KEY_F10:
            if (debug_function) std::cout << "F10 - Draw Mode: " << F10 << std::endl;
            display->SetDrawMode(F10);
            break;

        case GLUT_KEY_F11:
            if (debug_function) std::cout << "F11 - Draw Mode: " << F11 << std::endl;
            display->SetDrawMode(F11);
            break;

        case GLUT_KEY_F12:
            if (debug_function) std::cout << "F12 - ";
            if (display->ToggleCullFace())
            {
                if (debug_function) std::cout << "Cull Face: True" << std::endl;
            }
            else
            {
                if(debug_function) std::cout << "Cull Face: False" << std::endl;
            }
            break;

        case GLUT_KEY_INSERT:


            break;

        //translate
        case GLUT_KEY_UP:
            if (debug_key) std::cout << "UP " << std::endl;
            display->ViewTranslate(glm::vec3(  0.00f,  0.00f, -0.25f));
            break;

        case GLUT_KEY_DOWN:
            if (debug_key) std::cout << "DOWN " << std::endl;
            display->ViewTranslate(glm::vec3(  0.00f,  0.00f,  0.25f));
            break;

        case GLUT_KEY_RIGHT:
            if (debug_key) std::cout << "RIGHT " << std::endl;
            display->ViewTranslate(glm::vec3(  0.25f,  0.00f,  0.00f));
            break;

        case GLUT_KEY_LEFT:
            if (debug_key) std::cout << "LEFT " << std::endl;
            display->ViewTranslate(glm::vec3( -0.25f,  0.00f,  0.00f));
            break;

    }

    display->Redisplay();

}