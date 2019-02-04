/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RenderObject.cpp
 * Author: Barrett Kauling
 * 
 * Created on June 12, 2016, 6:21 PM
 */

#include "RenderObject.h"

RenderObject::RenderObject(RenderMaterials* Materials, Data* Datas, RenderStatus* Status) {
    
    if (DebugInit) std::cout << "RenderObject Constructor ..." << std::endl;
    
    for (std::vector<Faces>::iterator m = Materials->MaterialFaces->begin();
                                      m != Materials->MaterialFaces->end();
                                      m++)
    {
        
        ///////////////////////////////////
        //expand mesh into RenderMeshData//
        ///////////////////////////////////
        
        RenderMeshData* MeshData = new RenderMeshData;
        MeshData->Clear();
        MeshData->ShaderLocations = Materials->ShaderLocations;

        for (std::vector<Face>::iterator f = m->FaceList->begin(); f != m->FaceList->end(); f++)
        {
            MeshData->Vertex->push_back(Datas->Vertex->at(f->Vertex-1));
            MeshData->Coordinate->push_back(Datas->Coordinate->at(f->Coordinate-1));
            MeshData->Normal->push_back(Datas->Normal->at(f->Normal-1));
        }

        Meshes.push_front(RenderMesh(MeshData, Status));
        Meshes.front().SetName(m->Material);
        
        delete MeshData;
        
    }
    
    Status->RE.OBJECTS ++;
    
    if (DebugInit) std::cout << "RenderObject Constructor complete..." << std::endl << std::endl;
    
}

//RenderObject::RenderObject(const RenderObject& orig) {
//}

RenderObject::~RenderObject() {
    
    if (DebugInit)
        std::cout << "RenderObject " << this->Name << " Deconstructor ..." << std::endl;
    
}

//accessors

void RenderObject::Clear(RenderStatus* Status)
{
    
    for (std::forward_list<RenderMesh>::iterator m = Meshes.begin(); m != Meshes.end(); m++)
        m->Clear(Status);
    
    Status->RE.OBJECTS --;
    
}

void RenderObject::UploadModelMatrix(GLuint ModelMatrixLocation)
{
    
    glUniformMatrix4fv(ModelMatrixLocation, 1, GL_FALSE, glm::value_ptr(this->Model));
    
}

void RenderObject::DrawObject(GLenum DrawMode)
{
    
    //iterate the meshes
    for(std::forward_list<RenderMesh>::iterator CurrentMesh = Meshes.begin(); 
                                                CurrentMesh != Meshes.end(); 
                                                CurrentMesh++)
    {

        CurrentMesh->DrawMesh(DrawMode);

    }
    
}


void RenderObject::SetName(std::string NewName)
{
    
    this->Name = NewName;
    
}

void RenderObject::SetModel(glm::mat4 Model)
{
    
    //set the model matrix
    this->Model = Model;
    
}

void RenderObject::SetTextures(std::forward_list<RenderMaterial>* Textures)
{
    

    for (std::forward_list<RenderMaterial>::iterator t = Textures->begin(); t != Textures->end(); t++)
    {
        for (std::forward_list<RenderMesh>::iterator m = Meshes.begin(); m != Meshes.end(); m++)
        {
            if (DebugFunction)
            {
                std::cout << "Mesh: " << m->GetName();
                std::cout << "\t\tTexture: " << t->GetName() << std::endl;
            }
            if (m->GetName() == t->GetName())
                m->SetTexture(&*t);
        }
    }    
    
}


//getters


std::string RenderObject::GetName(void)
{
    
    return this->Name;
    
}

glm::mat4 RenderObject::GetModel(void)
{
    
    return this->Model;
    
}


std::forward_list<RenderMesh>* RenderObject::GetMeshes(void)
{
    
    return &this->Meshes;
    
}


//transform


void RenderObject::TransformTranslate(glm::vec3 Coordinates)
{
    
    //translate the model matrix
    this->Model = glm::translate(this->Model, Coordinates);

    if (DebugFunction)
    {
        std::cout << this->Name;
        std::cout << " Transform Translate:" << "\t";
        std::cout << Coordinates.x << "\t";
        std::cout << Coordinates.y << "\t";
        std::cout << Coordinates.z << "\t";
        std::cout << std::endl;
    }

}

void RenderObject::TransformRotate(float Degrees, glm::vec3 Axis)
{
    
    this->Model = glm::rotate(this->Model, glm::radians(Degrees), Axis);

    if (DebugFunction)
    {
        std::cout << this->Name;
        std::cout << " Transform Rotate:" << "\t\t";
        std::cout << Axis.x << "\t";
        std::cout << Axis.y << "\t";
        std::cout << Axis.z << "\t";
        std::cout << std::endl;
    }
    
}

void RenderObject::TransformScale(glm::vec3 Scale)
{
    
    //scale our model matrix
    this->Model = glm::scale(this->Model, Scale);

    if (DebugFunction)
    {
        std::cout << this->Name;
        std::cout << " Transform Scale:" << "\t\t";
        std::cout << Scale.x << "\t";
        std::cout << Scale.y << "\t";
        std::cout << Scale.z << "\t";
        std::cout << std::endl;
    }
    
}


//private


void RenderObject::AddMesh(RenderMesh* NewMesh)
{
    
    this->Meshes.push_front(*NewMesh);
    
}

void RenderObject::UpdateTransform(void)
{
    
    
}