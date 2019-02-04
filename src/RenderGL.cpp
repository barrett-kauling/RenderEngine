/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RenderInit.cpp
 * Author: Barrett 
 * 
 * Created on September 2, 2016, 1:54 PM
 */

#include "RenderGL.h"

RenderGL::RenderGL(RenderGLData* rd) {
    
    if (DebugInit) this->DebugMessage("Default GL Constructor ...\n", NULL);
    
    this->Window = *rd->wd;
    this->Functions = *rd->rf;
    
    //glut init
    glutInit(&rd->argc, rd->argv);
    
    //create window
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, Window.WindowCloseOption);
    glutInitDisplayMode(Window.DisplayMode);
    glutInitWindowSize(Window.Width, Window.Height);
    glutInitWindowPosition(Window.xPosition, Window.yPosition);
    Window.ID = glutCreateWindow(Window.WindowTitle.c_str());
    
    //glew glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) 
    {
        this->DebugMessage("Error: \n'%s'\n", (const GLchar*) glewGetErrorString(res));
        exit(1);
    }    
    if (DebugInit)
        this->DebugMessage("GLEW Initialization Complete ... \n", NULL);
    if (DebugInit) this->DebugMessage("GL Version: %s\n",
            (const GLchar*) glGetString(GL_VERSION));
    if (DebugInit) this->DebugMessage("GL Renderer: %s\n",
            (const GLchar*) glGetString(GL_RENDERER));
    if (DebugInit) this->DebugMessage("GL Vendor: %s\n",
            (const GLchar*) glGetString(GL_VENDOR));
    if (DebugInit) this->DebugMessage("GL Shading Language Version: %s\n", 
            (const GLchar*) glGetString(GL_SHADING_LANGUAGE_VERSION));
//    if (DebugInit) this->DebugMessage("GL Extensions: %s\n",
//            (const GLchar*) glGetString(GL_EXTENSIONS));
    if (DebugInit) this->DebugMessage("\n", NULL);
    
    //register local functions
    if (Functions.render) InitializeRenderCallback(Functions.render);
    if (Functions.idle) InitializeIdleCallback(Functions.idle);
    if (Functions.timer) InitializeTimerCallback(Functions.timer, 30, 1);

    //add default handlers
    if (Functions.keyboard) InitializeKeyboardCallback(Functions.keyboard);
    if (Functions.function) InitializeFunctionCallback(Functions.function);  
    

    
    if (DebugInit) this->DebugMessage("Setting initial default attributes ...\n", NULL);
    
    //gl color initialization
    this->ModifyClearColor(glm::vec4(0.0f, 0.5f, 1.0f, 0.0f));
    this->ModifyDrawColor(glm::vec3(1.0f, 1.0f, 1.0f));
    
    //set render mode attributes
    this->ModifyLineWidth(1);
    this->ModifySize(5);
    this->ModifyFont(GLUT_BITMAP_8_BY_13);
        
    //enable functionality
    this->EnableDepthBuffer(GL_LESS);
    this->EnableBlendFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    this->EnableFeature(GL_TEXTURE_2D);
    this->EnableCullFace(GL_CCW);
    this->ModifyPolyMode(GL_FRONT_AND_BACK, GL_FILL);
    
    if (DebugInit) this->DebugMessage("Default GL Constructor complete ...\n\n", NULL);
    
}

//RenderInit::RenderInit(const RenderInit& orig) {
//}

RenderGL::~RenderGL() {
    
    
    //glutDestroyWindow(Window.ID);

    
    //glDeleteProgram
    glDeleteProgram(ProgramObj);
    
}



GLuint RenderGL::Program(void)
{
    
    return ProgramObj;

}

RenderAttributes RenderGL::GetRenderAttributes(void)
{
    
    return Render;
    
}



WindowAttributes RenderGL::GetWindowAttributes(void)
{
    
    return Window;
    
}

GLuint RenderGL::GetClearMode(void)
{
    
    return Window.ClearMode;
    
}

GLuint RenderGL::GetDrawMode(void)
{
    
    return this->Window.DrawMode;
    
}

GLuint* RenderGL::GetLocation(ShaderVariable sv)
{
    
    switch (sv)
    {
        case Vertex:
            return &VertexLocation;
            break;
            
        case Normal:
            return &NormalLocation;
            break;
            
        case FrontColor:
            return &FrontColorLocation;
            break;
            
        case BackColor:
            return &BackColorLocation;
            break;
            
        case Texture:
            return &TextureLocation;
            break;
            
        case Tangent:
            return &TangentLocation;
            break;
            
        case Sampler:
            return &SamplerLocation;
            break;
            
        case ModelMatrix:
            return &ModelMatrixLocation;
            break;
            
        case ViewMatrix:
            return &ViewMatrixLocation;
            break;
            
        case ProjectionMatrix:
            return &ProjectionMatrixLocation;
            break;
            
        default:
            return 0;
            break;
    }
    
}


//features


void RenderGL::DetachShaders(void)
{

    //glDetachShader    
    for (int i = 0; i < VertexShaderObj.size(); i++ )
        glDetachShader(ProgramObj, VertexShaderObj[i]);
    for (int i = 0; i < FragmentShaderObj.size(); i++ )
        glDetachShader(ProgramObj, FragmentShaderObj[i]);
    
}

void RenderGL::DeleteShaders(void)
{

    //glDeleteShader
    for (int i = 0; i < VertexShaderObj.size(); i++ )
        glDeleteShader(VertexShaderObj[i]);
    for (int i = 0; i < FragmentShaderObj.size(); i++ )
        glDeleteShader(FragmentShaderObj[i]);  
    
}


//public methods



void RenderGL::InitializeRenderCallback(void (*RenderFunction)(void))
{
    
    //glut Render Function registration    
    glutDisplayFunc(*RenderFunction);
    
}

void RenderGL::InitializeIdleCallback(void (*IdleFunction) (void))
{
 
    //glut Idle Function registration    
    glutIdleFunc(*IdleFunction);
    
}

void RenderGL::InitializeKeyboardCallback(void (*KeyboardFunction)(unsigned char, int, int))
{
    
    //glut Keyboard Function registration
    glutKeyboardFunc(*KeyboardFunction);
    
}

void RenderGL::InitializeFunctionCallback(void (*SpecialFunction)(int ,int,int))
{
    
    //glut Keyboard Special Function registration
    glutSpecialFunc(*SpecialFunction);
    
}

void RenderGL::InitializeTimerCallback(void (*TimerFunction)(int),
                                            unsigned int Msec, int Value)
{
    
    //glut Timer Function registration
    glutTimerFunc(Msec, *TimerFunction, Value);
    
}

void RenderGL::ResetTimer(unsigned int Msec, int Value)
{
    
    glutTimerFunc(Msec, this->Functions.timer, Value);
    
}

void RenderGL::InitializeShaderLocations(void)
{
    
    if (DebugInit)
        this->DebugMessage("\nGetting location of variables in shader ...\n", NULL);
    
    //get location of Vertex, FrontColor, BackColor, and Texture
    this->GetAttrib(&VertexLocation, "Vertex");
    this->GetAttrib(&NormalLocation, "Normal");
    this->GetAttrib(&FrontColorLocation, "FrontColor");
    this->GetAttrib(&BackColorLocation, "BackColor");
    this->GetAttrib(&TextureLocation, "Texture");
    
    //get location of Sampler, Projection, View, and Model Matrix
    this->GetUniform(&SamplerLocation,"Sampler");
    this->GetUniform(&ViewMatrixLocation, "ViewMatrix");
    this->GetUniform(&ModelMatrixLocation, "ModelMatrix");
    this->GetUniform(&ProjectionMatrixLocation, "ProjectionMatrix");
    
}



void RenderGL::EnableFeature(GLenum FEATURE)
{
    
    //enable gl features
    glEnable(FEATURE);
    
}

void RenderGL::EnableCullFace(GLenum ORIENTATION)
{
    
    Render.CullEnabled = true;
    Render.CullMode = GL_BACK;
    Render.CullOrientation = ORIENTATION;
    
    //enable face culling
    glEnable(GL_CULL_FACE);
    //specify the orientation
    glCullFace(Render.CullMode);
    glFrontFace(Render.CullOrientation);
    
}

void RenderGL::SetCullFace(bool Cull)
{
    
    if (Cull)
    {
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
    
}

void RenderGL::EnableDepthBuffer(GLenum FUNCTION)
{
    
    //add the depth buffer bit to the display and clear mode
    this->Window.DisplayMode = this->Window.DisplayMode | GLUT_DEPTH;
    this->Window.ClearMode = this->Window.ClearMode | GL_DEPTH_BUFFER_BIT;
    
    //update the render depth function value
    this->Render.DepthFunction = FUNCTION;
    
    //set the parameter for the depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(this->Render.DepthFunction);
    glDepthRange(0.1, 100.0);
    glClearDepth(1.0);
    
}

void RenderGL::EnableBlendFunction(GLenum SOURCE, GLenum DESTINATION)
{
    
    //enable the blend function feature
    glEnable(GL_BLEND);
    
    //store the values locally
    this->Render.BlendSource = SOURCE;
    this->Render.BlendDestination = DESTINATION;

    //set the parameters for the gl blend function
    glBlendFunc(this->Render.BlendSource, this->Render.BlendDestination);
    
}

void RenderGL::AddPassThroughShaders(void)
{
    
    AddVertexShader("/usr/local/src/shaders/passthrough.vs");
    AddFragmentShader("/usr/local/src/shaders/passthrough.fs");
    
}

void RenderGL::AddVertexShader(const char* filename)
{
    
    VertexShaderObj.push_back(glCreateShader(GL_VERTEX_SHADER));
    
    if (VertexShaderObj.back() == 0)
    {
        this->DebugMessage("Error creating Vertex shader! %d\n", 
                (const GLchar*) VertexShaderObj.size()-1);
        exit(1);
    }
    
    std::string vs;
    const GLchar* p[1];
    GLint length;
    
    //ReadSource as a C++ string
    this->ReadSource(filename, vs);
    
    //convert the C++ string to a C style char* array
    //and calculate it's length so that it can be loaded into the glShaderSource
    p[0] = vs.c_str();
    length = strlen(vs.c_str());
    
    //glShaderSource
    glShaderSource(VertexShaderObj.back(), 1, p, &length);
    
}

void RenderGL::AddFragmentShader(const char* filename)
{
    
    FragmentShaderObj.push_back(glCreateShader(GL_FRAGMENT_SHADER));
    
    if (FragmentShaderObj.back() == 0)
    {
        this->DebugMessage("Error creating Fragment shader! %d\n", 
                (const GLchar*) FragmentShaderObj.size()-1);
        exit(1);
    }
    
    std::string fs;
    const GLchar* p[1];
    GLint length;
    
    //ReadSource as a C++ string
    this->ReadSource(filename, fs);
    
    //convert the C++ string to a C style char* array
    //and calculate it's length so that it can be loaded into the glShaderSource
    p[0] = fs.c_str();
    length = strlen(fs.c_str());
    
    //glShaderSource
    glShaderSource(FragmentShaderObj.back(), 1, p, &length);
    
}

bool RenderGL::ReadSource(const char* Source, std::string& Content)
{
    
    std::ifstream file(Source);
    std::string line;
    
    bool complete = false;
    
    if (file.is_open())
    {
        while (getline(file, line))
        {
            Content.append(line);
            Content.append("\n");
        }
        
        file.close();
        complete = true;
    }
    else {}
    
    //output error to console
    return complete;
    
}

int RenderGL::CheckError(GLuint Obj, GLuint FLAG, bool Program)
{
    
    GLint success = 0;
    GLchar ErrorLog[1024];
    
    if(Program)
    {
        
        //glGetProgramiv
        glGetProgramiv(Obj, FLAG, &success);
        if (success == 0 || DebugInit)
        {
            //glGetProgramInfoLog
            glGetProgramInfoLog(Obj, sizeof(ErrorLog), NULL, ErrorLog);
            this->DebugMessage("Program Info Log:\n%s\n", ErrorLog);
            if (success == 0)
            {
                this->DebugMessage("Error validating program!\n\n", NULL);
                exit(1);
            }
        }
    }
    
    else
    {
        
        //glGetShaderiv
        glGetShaderiv(Obj, FLAG, &success);
        if (success == 0 || DebugInit)
        {
            //glGetShaderInfoLog
            glGetShaderInfoLog(Obj, sizeof(ErrorLog), NULL, ErrorLog);
            this->DebugMessage("Shader Info Log: \n%s\n", ErrorLog);
            if (success == 0)
            {
                this->DebugMessage("Error compiling shader!\n\n", NULL);
                exit(1);
            }
        }
    }
    
    return 0;
    
}

void RenderGL::Compile()
{   
    
    //glCreateProgram
    ProgramObj = glCreateProgram();
    
    if (ProgramObj == 0)
    {
        this->DebugMessage("Error creating program!\n", NULL);
        exit(1);
    }    
    
    //glCompileShader
    //CheckError
    for (int i = 0; i < VertexShaderObj.size(); i++ )
    {
        if (DebugInit) 
            this->DebugMessage("Compiling Vertex Shader %d ...\n", 
                    (const GLchar*) (intptr_t) i);
        glCompileShader(VertexShaderObj[i]);
        this->CheckError(VertexShaderObj[i], GL_COMPILE_STATUS, false);
    }
    for (int i = 0; i < FragmentShaderObj.size(); i++ )
    {
        if (DebugInit) 
            this->DebugMessage("Compiling Fragment Shader %d ...\n", 
                    (const GLchar*) (intptr_t) i);
        glCompileShader(FragmentShaderObj[i]);
        this->CheckError(FragmentShaderObj[i], GL_COMPILE_STATUS, false);
    }
    
    //glAttachShader
    for (int i = 0; i < VertexShaderObj.size(); i++ )
        glAttachShader(ProgramObj, VertexShaderObj[i]);
    for (int i = 0; i < FragmentShaderObj.size(); i++ )
        glAttachShader(ProgramObj, FragmentShaderObj[i]);
    
    //glBindAttribLocation for vertex input
    glBindAttribLocation(ProgramObj, 0, "Vertex");
    
    if (DebugInit) this->DebugMessage("Linking ...\n", NULL);
    
    //glLinkProgram
    glLinkProgram(ProgramObj);
    //CheckError
    this->CheckError(ProgramObj, GL_LINK_STATUS, true);
    
    if (DebugInit) this->DebugMessage("Validating ...\n", NULL);
    
    //glValidateProgram
    glValidateProgram(ProgramObj);
    //CheckError
    this->CheckError(ProgramObj, GL_VALIDATE_STATUS, true);
    
    //glUseProgram
    glUseProgram(ProgramObj);
    
    if (DebugInit) this->DebugMessage("Compilation complete ...\n\n", NULL);
    
}


//modifiers


void RenderGL::ModifyDrawMode(GLenum DRAWMODE)
{
    
    this->Window.DrawMode = DRAWMODE;
    
}

void RenderGL::ModifyPolyMode(GLenum FACEMODE, GLenum FILLMODE)
{
    
    Render.FACEMODE = FACEMODE ;
    Render.FILLMODE = FILLMODE;
    glPolygonMode(Render.FACEMODE, Render.FILLMODE);
    
    if (DebugFunction)
    {
        std::cout << "\t" << "PolyMode:" << "\t";  
        std::cout << Render.FACEMODE << "\t";
        std::cout << Render.FILLMODE << std::endl;
    }
    
}

void RenderGL::ModifyLineWidth(float Width)
{
    
    Render.LineWidth = Width;
    glLineWidth(Render.LineWidth);
    
    if (DebugFunction)
    {
        std::cout << "\t" << "LineWidth:" << "\t";
        std::cout << Render.LineWidth << std::endl;
    }

}

void RenderGL::ModifySize(float Size)
{
    
    Render.PointSize = Size;
    glPointSize(Render.PointSize);
    
    if (DebugFunction)
    {   
        std::cout << "\t" << "Size: " << "\t" << "\t";
        std::cout << Render.PointSize << std::endl;
    }

}

void RenderGL::ModifyFont(void* Font)
{
    
    Render.Font = Font;
    
    if (DebugFunction)
    {
        std::cout << "\t" << "Font:" << "\t\t";
        std::cout << Render.Font << std::endl;
    }

}

void RenderGL::ModifyDrawColor(glm::vec3 Color)
{
    
    Render.DrawColor = Color;
    glColor3fv(glm::value_ptr(Render.DrawColor));

    if (DebugFunction)
    {
        std::cout << "\t" << "DrawColor:" << "\t";
        std::cout << Render.DrawColor.r << "\t";
        std::cout << Render.DrawColor.g << "\t";
        std::cout << Render.DrawColor.b << std::endl;
    }
}

void RenderGL::ModifyClearColor(glm::vec4 Color)
{
    
    Render.ClearColor = Color;
    glClearColor(Render.ClearColor.r, Render.ClearColor.g, 
                Render.ClearColor.b,  Render.ClearColor.a);
    
    if (DebugFunction)
    {
        std::cout << "\t" << "\tClearColor: " << "\t";
        std::cout << Render.ClearColor.r << "\t";
        std::cout << Render.ClearColor.g << "\t";
        std::cout << Render.ClearColor.b << std::endl;
    }
    
}

GLuint RenderGL::GetAttrib(GLuint* Location, const GLchar* Name)
{
    
    //get location of attribute
    *Location = glGetAttribLocation(ProgramObj, Name);
    fprintf(stderr, "GetAttrib %s: ", Name);
    if(*Location == -1)
            fprintf(stderr, " Failed\n");
    else
    {
        if (DebugInit) 
            this->DebugMessage(" Succeeded: %d\n", (const GLchar*) (intptr_t) *Location);
    }
    
    return (GLuint) 0;
    
}

GLuint RenderGL::GetUniform(GLuint* Location, const char* Name)
{
    
    //get location of attribute
    *Location = glGetUniformLocation(ProgramObj, Name);
    fprintf(stderr, "GetUniform %s: ", Name);
    if(*Location == -1)
            fprintf(stderr, " Failed\n");
    else
    {
        if (DebugInit) 
            this->DebugMessage(" Succeeded: %d\n", (const GLchar*) (intptr_t) *Location);            
    }
    
    return (GLuint) 0;
    
}

void RenderGL::DebugMessage(const char* Message, const GLchar* Log)
{
    
    fprintf(stderr, Message, Log);
    
}

