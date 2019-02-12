/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   RenderEngine.cpp
 * Author: Barrett Kauling
 *
 * Created on January 3, 2016, 7:31 PM
 */


#include "RenderEngine.h"


using namespace std;

int a = 0;

//constructors

RenderEngine::RenderEngine(RenderGLData* rd)
{

    if (DebugInit) this->DebugMessage("Default RenderEngine Constructor ...\n", NULL);

    //init with default window options from struct
    this->GL = new RenderGL(rd);

    this->Reader = new RenderReader(&Status);

    if (DebugInit) this->DebugMessage("Default RenderEngine Constructor complete ...\n\n", NULL);

}

//RenderEngine::RenderEngine(const RenderEngine& orig) {
//}

RenderEngine::~RenderEngine() {

    if (DebugInit) this->DebugMessage("\nDefault RenderEngine Deconstructor ...", NULL);

    //clean up objects
    this->DeleteObjects();

    delete this->Reader;
    delete this->GL;

    if (DebugInit) this->DebugMessage("\nDefault RenderEngine Deconstructor complete ...\n\n", NULL);

}

void RenderEngine::DeleteObjects(void)
{

    //clear each object
    for (std::forward_list<RenderObject>::iterator o = Objects.begin(); o != Objects.end(); o++)
    {
        o->Clear(&this->Status);
    }

}


//getters


PerspectiveInfo RenderEngine::GetPerspectiveInfo(void)
{

    return Perspective;

}

LookAtInfo RenderEngine::GetLookAtInfo(void)
{

    return LookAt;

}


//setters


void RenderEngine::SetDirectory(const std::string directory)
{

    this->Status.Directory = directory;

}

void RenderEngine::SetReadPageSize(bool ReadPage)
{

    Reader->setReadPageSize(ReadPage, &Status);

}

void RenderEngine::AddVertexShader(std::string Filename)
{

    GL->AddVertexShader(Filename.c_str());

}

void RenderEngine::AddFragmentShader(std::string Filename)
{

    GL->AddFragmentShader(Filename.c_str());

}

void RenderEngine::Compile(void)
{

    GL->Compile();
    GL->InitializeShaderLocations();

    this->Variables = new MeshVariables;
    this->Variables->Clear();
    this->Variables->VertexLocation = GL->GetLocation(Vertex);
    this->Variables->CoordinateLocation = GL->GetLocation(Texture);
    this->Variables->NormalLocation = GL->GetLocation(Normal);
    this->Variables->TangentLocation = GL->GetLocation(Tangent);

    this->ViewMatrixLocation = *GL->GetLocation(ViewMatrix);
    this->ProjectionMatrixLocation = *GL->GetLocation(ProjectionMatrix);

}

//transform


void RenderEngine::SetInitialView(glm::mat4 Model, glm::vec3 Eye, glm::vec3 Center, glm::vec3 Up)
{

    InitialModel = Model;
    InitialLookAt.Eye = Eye;
    InitialLookAt.Center = Center;
    InitialLookAt.Up = Up;

    this->ResetView();

}

void RenderEngine::ResetView(void)
{

    if (DebugFunction)
    {
        std::cout << "ResetView" << std::endl;
    }

    this->SetModel(InitialModel);
    this->SetLookAt(InitialLookAt.Eye, InitialLookAt.Center, InitialLookAt.Up);

}

void RenderEngine::SetLookAt(glm::vec3 Eye, glm::vec3 Center, glm::vec3 Up)
{

    if (DebugFunction)
    {
        std::cout << "SetLookAt" << std::endl;
    }

    //set the lookat
    LookAt.Eye = Eye;
    LookAt.Center = Center;
    LookAt.Up = Up;

    this->SetView(glm::lookAt(LookAt.Eye, LookAt.Center, LookAt.Up));


}

void RenderEngine::SetPerspective(float FOV, float AspectRatio, float Near, float Far)
{

    if (DebugFunction)
    {
        std::cout << "SetPerspective" << std::endl;
    }

    //set perspective from values
    Perspective.FOV = FOV;
    Perspective.AspectRatio = AspectRatio;
    Perspective.Near = Near;
    Perspective.Far = Far;
    this->SetProjection(glm::perspective(Perspective.FOV, Perspective.AspectRatio,
                                        Perspective.Near, Perspective.Far));


}

void RenderEngine::SetView(glm::mat4 View)
{

    if (DebugFunction)
    {
        std::cout << "SetView" << std::endl;
    }

    //set the view matrix
    this->View = View;

    glUniformMatrix4fv(ViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(this->View));

}

void RenderEngine::SetProjection(glm::mat4 Projection)
{

    if (DebugFunction)
    {
        std::cout << "SetProjection" << std::endl;
    }

    //set the projection matrix
    this->Projection = Projection;

    glUniformMatrix4fv(ProjectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(this->Projection));

}

void RenderEngine::SetModel(glm::mat4 Model)
{

    if (DebugFunction)
    {
        std::cout << "SetModel" << std::endl;
    }

    //set the model matrix
    this->Model = Model;

    //set the model matrix for each object
    for (std::forward_list<RenderObject>::iterator i = Objects.begin(); i != Objects.end(); i++)
    {
        i->SetModel(Model);
    }

}


//translate


void RenderEngine::TransformTranslate(glm::vec3 Coordinates)
{

    //translate model matrix
    for (std::forward_list<RenderObject>::iterator i = Objects.begin(); i != Objects.end(); i++)
    {
        i->TransformTranslate(Coordinates);
    }

    if (DebugFunction)
    {
        std::cout << "\t" << "Transform Translate:" << "\t";
        std::cout << Coordinates.x << "\t";
        std::cout << Coordinates.y << "\t";
        std::cout << Coordinates.z << "\t";
        std::cout << std::endl;
    }

}

void RenderEngine::TransformRotate(float Degrees, glm::vec3 Axis)
{

    //rotate object model matrix
    for (std::forward_list<RenderObject>::iterator i = Objects.begin(); i != Objects.end(); ++i)
    {
        i->TransformRotate(Degrees, Axis);
    }

    if (DebugFunction)
    {
        std::cout << "\t" << "Transform Rotate:" << "\t\t";
        std::cout << Axis.x << "\t";
        std::cout << Axis.y << "\t";
        std::cout << Axis.z << "\t";
        std::cout << std::endl;
    }

}

void RenderEngine::TransformScale(glm::vec3 Scale)
{

    //scale object matrix
    for (std::forward_list<RenderObject>::iterator i = Objects.begin(); i != Objects.end(); ++i)
    {
        i->TransformScale(Scale);
    }


    if (DebugFunction)
    {
        std::cout << "\t" << "Transform Scale:" << "\t\t";
        std::cout << Scale.x << "\t";
        std::cout << Scale.y << "\t";
        std::cout << Scale.z << "\t";
        std::cout << std::endl;
    }

}


//view


void RenderEngine::ViewRotate(float Degrees, glm::vec3 Axis)
{

    //rotate the view matrix
    this->View = glm::rotate(this->View, glm::radians(Degrees), Axis);

    if (DebugFunction)
    {
        std::cout << "\t" << "View Rotate:" << "\t\t";
        std::cout << Axis.x * Degrees << "\t";
        std::cout << Axis.y * Degrees << "\t";
        std::cout << Axis.z * Degrees << "\t";
        std::cout << std::endl;
    }

}

void RenderEngine::ViewTranslate(glm::vec3 Coordinates)
{

    //translate the view matrix
    this->View = glm::translate(this->View, Coordinates);

    if (DebugFunction)
    {
        std::cout << "\t" << "View Translate:" << "\t\t";
        std::cout << Coordinates.x << "\t";
        std::cout << Coordinates.y << "\t";
        std::cout << Coordinates.z << "\t";
        std::cout << std::endl;
    }

}


//render


void RenderEngine::RenderObjects(void)
{

    //iterate each object
    for(std::forward_list<RenderObject>::iterator EachObject = Objects.begin();
                                            EachObject != Objects.end();
                                            EachObject++)
    {

        EachObject->UploadModelMatrix(*this->GL->GetLocation(ModelMatrix));
        EachObject->DrawObject(this->GL->GetDrawMode());

    }

}

void RenderEngine::RenderText(const char* Text, float x, float y, float Color[])
{



}

void RenderEngine::Clear(void)
{

    //clear screen using defaults
    glClear(this->GL->GetClearMode());

}

void RenderEngine::Clear(GLenum OPTION)
{

    //clear screen
    glClear(OPTION);

}

void RenderEngine::SwapBuffers(void)
{

    //swap buffers
    glutSwapBuffers();

}

void RenderEngine::Redisplay(void)
{

    //refresh
    glutPostRedisplay();

}

void RenderEngine::ResetTimer(unsigned int Msec, int Value)
{

    this->GL->ResetTimer(Msec, Value);

}

void RenderEngine::SetDrawMode(GLenum DrawMode)
{

    this->GL->ModifyDrawMode(DrawMode);

}

bool RenderEngine::ToggleCullFace(void)
{
    if (glIsEnabled(GL_CULL_FACE))
    {
        this->GL->SetCullFace(false);
        return false;
    }
    else
    {
        this->GL->SetCullFace(true);
        return true;
    }
}

void RenderEngine::MainLoop(void)
{

    //add work later to stop program if there are errors
    glutMainLoop();

}


//data


void RenderEngine::GenerateTextures(char* Filename)
{
    
    std::string File = std::string(Filename);
    if (File.find("./") != std::string::npos)
        File = File.substr(2);
    File.insert(0, this->Status.Directory);
    
    RenderGenerateData* mat = this->Reader->ReadMaterial((char*)File.c_str());

    for(std::vector<ImageTextureStruct>::iterator i = mat->Images->begin(); i != mat->Images->end(); i++)
    {
        this->Images.push_front(RenderImage(&*i, &Status));
    }

    for(std::vector<RenderTextureData>::iterator r = mat->Materials->begin(); r != mat->Materials->end(); r++)
    {

        RenderMaterialStruct* rm;
        rm->Clear();

        for(std::forward_list<RenderImage>::iterator i = this->Images.begin(); i != this->Images.end(); i++)
        {
            if(i->GetName() == r->map_Ka)
            {
                rm->map_Ka = &*i;
            }
            if(i->GetName() == r->map_Kd)
            {
                rm->map_Kd = &*i;
            }
            if(i->GetName() == r->map_Ks)
            {
                rm->map_Ks = &*i;
            }
            if(i->GetName() == r->map_Ke)
            {
                rm->map_Ke = &*i;
            }
            if(i->GetName() == r->map_Ns)
            {
                rm->map_Ns = &*i;
            }
            if(i->GetName() == r->map_d)
            {
                rm->map_d = &*i;
            }
            if(i->GetName() == r->map_Bump)
            {
                rm->map_Bump = &*i;
            }
            if(i->GetName() == r->bump)
            {
                rm->bump = &*i;
            }
            if(i->GetName() == r->disp)
            {
                rm->disp = &*i;
            }
            if(i->GetName() == r->decal)
            {
                rm->decal = &*i;
            }
            if(i->GetName() == r->refl)
            {
                rm->refl = &*i;
            }
        }

        this->Materials.push_front(RenderMaterial(rm, &Status));

    }

    delete mat;

}

void RenderEngine::GenerateMesh(std::string Filename)
{

    if (Filename.find("./") != std::string::npos)
        Filename = Filename.substr(2);

    RenderObjectData* obj = this->Reader->ReadWavefront(this->Status.Directory + Filename);

    std::cout << std::endl;
    std::cout << "Read complete: " << obj->MaterialObjects->size() << " meshes" << std::endl;
    for(std::vector<RenderMaterials>::iterator r = obj->MaterialObjects->begin(); r != obj->MaterialObjects->end(); r++)
    {
        r->ShaderLocations = Variables;
        std::cout << r->Name << ": " << r->MaterialFaces->size() << std::endl;
        for (std::vector<Faces>::iterator f = r->MaterialFaces->begin(); f != r->MaterialFaces->end(); f++)
        {
            std::cout << "\t" << f->Material << ": " << f->Smoothing << " - " << f->FaceList->size() << std::endl;
            Objects.push_front(RenderObject(&*r, obj->MaterialData, &Status));
        }
    }

    delete obj;



    if (DebugInit)
    {

//        Status.GL.VBO =    78;
//        Status.GL.CBO =    95;
//        Status.GL.NBO =  1997;
//        Status.GL.TBO =     9;
//        Status.GL.TCO =   898;

        Status.GL.COUNT = Status.GL.VBO + Status.GL.CBO + Status.GL.NBO + Status.GL.TBO + Status.GL.TCO;

//        Status.GL.VSIZE =   1192000;
//        Status.GL.CSIZE =    192000;
//        Status.GL.NSIZE =  43840000;
//        Status.GL.TSIZE =     51200;
//        Status.GL.TCSIZE =  1128000;

        Status.GL.SIZE = Status.GL.VSIZE + Status.GL.CSIZE + Status.GL.NSIZE + Status.GL.TSIZE + Status.GL.TCSIZE;

//        Status.RE.OBJECTS = 3;
//        Status.RE.MESHES = 40;
//        Status.RE.TEXTURES = 500;


        std::cout << std::endl;
        std::cout << "///////////////////////////////" << std::endl;
        std::cout << "GPU              Gb  Mb  Kb   " << std::endl;

        std::cout << "#VBO: ";
        std::cout.width(BUF_C_SIZE);
        std::cout << num_sep(Status.GL.VBO);
        std::cout << " size: ";
        std::cout.width(MEM_C_SIZE);
        std::cout << num_sep(Status.GL.VSIZE);
        std::cout << std::endl;

        std::cout << "#CBO: ";
        std::cout.width(BUF_C_SIZE);
        std::cout << num_sep(Status.GL.CBO);
        std::cout << " size: ";
        std::cout.width(MEM_C_SIZE);
        std::cout << num_sep(Status.GL.CSIZE);
        std::cout << std::endl;

        std::cout << "#NBO: ";
        std::cout.width(BUF_C_SIZE);
        std::cout << num_sep(Status.GL.NBO);
        std::cout << " size: ";
        std::cout.width(MEM_C_SIZE);
        std::cout << num_sep(Status.GL.NSIZE);
        std::cout << std::endl;

        std::cout << "#TBO: ";
        std::cout.width(BUF_C_SIZE);
        std::cout << num_sep(Status.GL.TBO);
        std::cout << " size: ";
        std::cout.width(MEM_C_SIZE);
        std::cout << num_sep(Status.GL.TSIZE);
        std::cout << std::endl;

        std::cout << "#TCO: ";
        std::cout.width(BUF_C_SIZE);
        std::cout << num_sep(Status.GL.TCO);
        std::cout << " size: ";
        std::cout.width(MEM_C_SIZE);
        std::cout << num_sep(Status.GL.TCSIZE);
        std::cout << std::endl;

        std::cout << "     ";
        std::cout.width(BUF_C_SIZE +1);
        std::cout << num_sep(Status.GL.COUNT);
        std::cout << "       ";
        std::cout.width(MEM_C_SIZE);
        std::cout << num_sep(Status.GL.SIZE);
        std::cout << std::endl;

        std::cout << "CPU" << std::endl;
        std::cout << "#OBJECTS:  ";
        std::cout.flags(std::ios::right) << std::cout.width(4);
        std::cout << Status.RE.OBJECTS;
        std::cout << std::endl;

        std::cout << "#MESHES:   ";
        std::cout.flags(std::ios::right) << std::cout.width(4);
        std::cout << Status.RE.MESHES;
        std::cout << std::endl;

        std::cout << "#TEXTURES: ";
        std::cout.flags(std::ios::right) << std::cout.width(4);
        std::cout << Status.RE.TEXTURES;
        std::cout << std::endl;

        std::cout << "///////////////////////////////" << std::endl;
        std::cout << std::endl;

    }

}

void RenderEngine::DebugMessage(const char* Message, const GLchar* Log)
{

    fprintf(stderr, Message, Log);

}