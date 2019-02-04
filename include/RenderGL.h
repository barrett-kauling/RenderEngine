/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RenderGL.h
 * Author: Barrett 
 *
 * Created on September 2, 2016, 1:54 PM
 */


#ifndef RENDERGL_H
#define RENDERGL_H

#include "RenderGlobal.h"


typedef enum
{
    Vertex,
    Normal,
    FrontColor,
    BackColor,
    Texture,
    Tangent,
    Sampler,
    ModelMatrix,
    ViewMatrix,
    ProjectionMatrix
} ShaderVariable;

typedef struct
{
    float FOV;
    float AspectRatio;
    float Near;
    float Far;
} PerspectiveInfo;

typedef struct
{
    glm::vec3 Eye;
    glm::vec3 Center;
    glm::vec3 Up;
} LookAtInfo;

typedef struct
{
    int ID = 0;
    int Width = 800;
    int Height = 600;
    int xPosition = 1000;
    int yPosition = 100;
    std::string WindowTitle = "OpenGL default constructor!";
    GLenum WindowCloseOption = GLUT_ACTION_GLUTMAINLOOP_RETURNS;
    GLenum DisplayMode = GLUT_RGBA|GLUT_ALPHA|GLUT_DOUBLE|GLUT_DEPTH;
    GLenum ClearMode = GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT;
    GLenum DrawMode = GL_TRIANGLES;
} WindowAttributes;

typedef struct 
{
    GLenum FACEMODE;
    GLenum FILLMODE;
    GLenum DepthFunction;
    GLenum BlendSource;
    GLenum BlendDestination;
    bool CullEnabled;
    GLenum CullMode;
    GLenum CullOrientation;
    glm::vec3 DrawColor;
    glm::vec4 ClearColor;
    GLfloat PointSize;
    GLfloat LineWidth;
    void* Font;
} RenderAttributes;

typedef struct
{
    void(*render)();
    void(*idle)();
    void(*timer)(int);
    void(*keyboard)(unsigned char, int, int);
    void(*function)(int, int, int);    
    void Clear(void)
    {
        render = NULL;
        idle = NULL;
        keyboard = NULL;
        function = NULL;
        timer = NULL;
    }
} RenderFunctions;

typedef struct
{
    int argc;
    char** argv;
    RenderFunctions* rf;
    WindowAttributes* wd;
    void Init(void)
    {
        rf = new RenderFunctions;
        wd = new WindowAttributes;
    }
    void Clear(void)
    {
        this->Init();
        argc = 0;
        argv = NULL;
        rf->Clear();
    }
} RenderGLData;

class RenderGL {
public:
    RenderGL(RenderGLData*);
//    RenderGL(const RenderGL& orig);
    virtual ~RenderGL();
    
    //shader
    void AddPassThroughShaders(void);
    void AddVertexShader(const char*);
    void AddFragmentShader(const char*);    
    void DetachShaders(void);
    void DeleteShaders(void);
     
    //accessors
    GLuint Program(void);
    RenderAttributes GetRenderAttributes(void);
    WindowAttributes GetWindowAttributes(void);
    GLuint GetClearMode(void);
    GLuint GetDrawMode(void);
    GLuint* GetLocation(ShaderVariable);
    
    //initialization
    void InitializeRenderCallback(void (*)());
    void InitializeIdleCallback(void (*)());
    void InitializeKeyboardCallback(void (*)(unsigned char, int, int));
    void InitializeFunctionCallback(void (*)(int, int, int));
    void InitializeTimerCallback(void (*)(int), unsigned int, int);
    void InitializeShaderLocations(void);
    void Compile(void);
    
    //modifiers
    void ModifyDrawMode(GLenum);
    void ModifyPolyMode(GLenum, GLenum);
    void ModifyLineWidth(GLfloat);
    void ModifySize(GLfloat);
    void ModifyFont(void*);
    void ModifyDrawColor(const glm::vec3);
    void ModifyClearColor(const glm::vec4);
    
    //features
    void EnableFeature(GLenum);
    void EnableCullFace(GLenum);
    void EnableDepthBuffer(GLenum);
    void EnableBlendFunction(GLenum, GLenum);
    void SetCullFace(bool);
    
    //
    void ResetTimer(unsigned int, int);

    
private:

    //opengl functions
    bool ReadSource(const char*, std::string&);
    int CheckError(GLuint, GLuint, bool);
    GLuint GetAttrib(GLuint*, const char*);
    GLuint GetUniform(GLuint*, const char*);
    void DebugMessage(const char*, const GLchar*);
    
    //global variables
    bool DebugInit = true;
    bool DebugFunction = false;
    bool DebugRead = false;
    bool DebugData = true;
    
    //global structures
    RenderAttributes Render;
    RenderFunctions Functions;
    WindowAttributes Window;
    
    
    //shader locations
    GLuint VertexLocation;
    GLuint NormalLocation;
    GLuint FrontColorLocation;
    GLuint BackColorLocation;
    GLuint TextureLocation;
    GLuint TangentLocation;
    GLuint SamplerLocation;
    GLuint ModelMatrixLocation;
    GLuint ViewMatrixLocation;
    GLuint ProjectionMatrixLocation;
    
    //program
    GLuint ProgramObj;
    
    //shader
    std::vector<GLuint> VertexShaderObj;
    std::vector<GLuint> FragmentShaderObj;
    
};

#endif /* RENDERGL_H */

