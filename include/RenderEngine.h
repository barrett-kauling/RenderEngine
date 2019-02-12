/*
 * To change this license header, choose License Headers in
 * Project Properties. To change this template file, choose
 * Tools | Templates and open the template in the editor.
 */

/* 
 * File:   RenderEngine.h
 * Author: Barrett Kauling
 *
 * Created on January 3, 2016, 7:31 PM
 */

#ifndef RENDERENGINE_H
#define RENDERENGINE_H

//required for glm/gtc/constants
#define GLM_FORCE_RADIANS


#include "RenderObject.h"
#include "RenderReader.h"
#include "RenderGL.h"

class RenderEngine {


public:
    
    //constructors
    RenderEngine(RenderGLData*);
    //RenderEngine(const RenderEngine& orig);
    virtual ~RenderEngine();
    
    void DeleteObjects(void);
    
    //viewport getters
    PerspectiveInfo GetPerspectiveInfo(void);
    LookAtInfo GetLookAtInfo(void);
    
    //setters
    void SetReadPageSize(bool);
    void SetDirectory(const std::string);
    void AddVertexShader(std::string);
    void AddFragmentShader(std::string);
    void Compile(void);

    //init 
    void SetInitialView(glm::mat4, glm::vec3, glm::vec3, glm::vec3);
    void ResetView(void);
    
    //matrices
    void SetProjection(glm::mat4);    
    void SetView(glm::mat4);
    void SetModel(glm::mat4);
    
    //transform
    void TransformTranslate(glm::vec3);
    void TransformRotate(float, glm::vec3);
    void TransformScale(glm::vec3);
    
    //view
    void SetPerspective(float, float, float, float);
    void SetLookAt(glm::vec3, glm::vec3, glm::vec3);
    void ViewRotate(float, glm::vec3);
    void ViewTranslate(glm::vec3);
    
    //data
    void GenerateTextures(char*);
    void GenerateMesh(std::string);
    
    
    //render
    void RenderText(const char*, float, float, float[]);
    void RenderObjects(void);
    void Clear(void);
    void Clear(GLenum);
    void SwapBuffers(void);
    void Redisplay(void);
    void ResetTimer(unsigned int, int);
    void SetDrawMode(GLenum);
    bool ToggleCullFace(void);
    
    //MainLoop
    void MainLoop(void);
    
private:

    void DebugMessage(const char*, const GLchar*);
    
    //global variables
    bool DebugInit = true;
    bool DebugFunction = false;
    bool DebugRead = false;
    bool DebugData = true;
    
    //status
    RenderStatus Status;

    //global structures
    PerspectiveInfo Perspective;
    LookAtInfo LookAt;

    //local class transformation matrices
    glm::mat4 Model;
    glm::mat4 View;
    glm::mat4 Projection;

    //initial view
    glm::mat4 InitialModel;
    LookAtInfo InitialLookAt;
    
    //shader variables
    MeshVariables* Variables;
    GLuint ViewMatrixLocation;
    GLuint ProjectionMatrixLocation;
    
    //render
    std::forward_list<RenderObject> Objects;
    std::forward_list<RenderMaterial> Materials; 
    std::forward_list<RenderImage> Images;
    
    RenderReader* Reader;
    RenderGL* GL;
    
};



#endif /* RENDERENGINE_H */