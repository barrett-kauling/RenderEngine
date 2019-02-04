/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RenderMesh.cpp
 * Author: Barrett Kauling
 * 
 * Created on February 6, 2016, 7:56 AM
 */


#include <vector>

#include "RenderMesh.h"


RenderMesh::RenderMesh(RenderMeshData* MeshData, RenderStatus* Status) {
    
    if (DebugInit) std::cout << "RenderMesh Constructor ..." << std::endl;
    
    this->Mesh = *MeshData;
    this->Material = NULL;
    
    this->vb.LOCATION = this->Mesh.Vertex;
    this->vb.SIZE = this->Mesh.Vertex->size() * sizeof(glm::vec3);
    this->vb.COUNT = this->Mesh.Vertex->size();
    this->vb.TYPE = GL_ARRAY_BUFFER;
    this->vb.DRAWSTYLE = GL_STATIC_DRAW;
    this->vb.SHADER_VARIABLE = this->Mesh.ShaderLocations->VertexLocation;
    
    this->cb.LOCATION = this->Mesh.Coordinate;
    this->cb.SIZE = this->Mesh.Coordinate->size() * sizeof(glm::vec2);
    this->cb.COUNT = this->Mesh.Coordinate->size();
    this->cb.TYPE = GL_ARRAY_BUFFER;
    this->cb.DRAWSTYLE = GL_STATIC_DRAW;
    this->cb.SHADER_VARIABLE = this->Mesh.ShaderLocations->CoordinateLocation;
    
    this->nb.LOCATION = this->Mesh.Normal;
    this->nb.SIZE = this->Mesh.Normal->size() * sizeof(glm::vec3);
    this->nb.COUNT = this->Mesh.Normal->size();
    this->nb.TYPE = GL_ARRAY_BUFFER;
    this->nb.DRAWSTYLE = GL_STATIC_DRAW;
    this->nb.SHADER_VARIABLE = this->Mesh.ShaderLocations->NormalLocation;
    
    this->tb.LOCATION = this->Mesh.Tangent;
    this->tb.SIZE = this->Mesh.Tangent->size() * sizeof(glm::vec3);
    this->tb.COUNT = this->Mesh.Tangent->size();
    this->tb.TYPE = GL_ARRAY_BUFFER;
    this->tb.DRAWSTYLE = GL_STATIC_DRAW;
    this->tb.SHADER_VARIABLE = this->Mesh.ShaderLocations->TangentLocation;
    
    //glGenBuffers VBO
    glGenBuffers(1, &vb.VBO);
    glBindBuffer(vb.TYPE, vb.VBO);
    glBufferData(vb.TYPE, vb.SIZE, glm::value_ptr(vb.LOCATION->at(0)), vb.DRAWSTYLE);
    Status->GL.VBO ++;
    Status->GL.VSIZE += vb.SIZE;
    
    //glGenBuffers CBO
    if (cb.SIZE)
    {
        glGenBuffers(1, &cb.CBO);
        glBindBuffer(cb.TYPE, cb.CBO);
        glBufferData(cb.TYPE, cb.SIZE, glm::value_ptr(cb.LOCATION->at(0)), cb.DRAWSTYLE);
        Status->GL.CBO ++;
        Status->GL.CSIZE += cb.SIZE;
    }
    
    //glGenBuffers NBO
    glGenBuffers(1, &nb.VBO);
    glBindBuffer(nb.TYPE, nb.VBO);
    glBufferData(nb.TYPE, nb.SIZE, glm::value_ptr(nb.LOCATION->at(0)), nb.DRAWSTYLE);
    Status->GL.NBO ++;
    Status->GL.NSIZE += nb.SIZE;
    
    //glGenBuffers TBO
    if (tb.SIZE)
    {
        glGenBuffers(1, &tb.VBO);
        glBindBuffer(tb.TYPE, tb.VBO);
        glBufferData(tb.TYPE, tb.SIZE, glm::value_ptr(tb.LOCATION->at(0)), tb.DRAWSTYLE);
        Status->GL.TBO ++;
        Status->GL.TSIZE += tb.SIZE;
    }
    
    //update mesh count
    Status->RE.MESHES ++;
    
    if (DebugInit)
    {
        std::cout << "Vertex Location: " << glm::value_ptr(vb.LOCATION->at(0)) << std::endl;
        std::cout << "Vertex Count:    " << vb.COUNT << std::endl;
        std::cout << "Vertex Size:     " << vb.SIZE << std::endl;
        std::cout << "Texture Location: " << glm::value_ptr(cb.LOCATION->at(0)) << std::endl;
        std::cout << "Texture Count:    " << cb.COUNT << std::endl;
        std::cout << "Texture Size:     " << cb.SIZE << std::endl;
        std::cout << "Normal Location: " << glm::value_ptr(nb.LOCATION->at(0)) << std::endl;
        std::cout << "Normal Count:    " << nb.COUNT << std::endl;
        std::cout << "Normal Size:     " << nb.SIZE << std::endl;
        std::cout << "VBO: " << vb.VBO << std::endl;
        std::cout << "TBO: " << cb.CBO << std::endl;
        std::cout << "NBO: " << nb.VBO << std::endl;
    }
    
    if (DebugInit) std::cout << "RenderMesh Constructor complete..." << std::endl << std::endl;
    
}

//RenderMesh::RenderMesh(const RenderMesh& orig) {
//}

RenderMesh::~RenderMesh() {
    
    if (DebugInit)
        std::cout << "RenderMesh " << this->Name << " Deconstructor ... " << std::endl;
    
}

void RenderMesh::Clear(RenderStatus* Status)
{
    
    if (DebugFunction)
    {
        std::cout << "RenderMesh " << this->Name << " - ";
        std::cout << this->Name << " Clear ..." << std::endl;
    }
    
    //clear buffers
    glDeleteBuffers(1, &vb.VBO); 
    glDeleteBuffers(1, &nb.VBO);
    glDeleteBuffers(1, &cb.CBO);
    
    //update status
    Status->GL.VBO --;
    Status->GL.VSIZE -= vb.SIZE;
    Status->GL.TBO --;
    Status->GL.TSIZE -= cb.SIZE;
    Status->GL.NBO --;
    Status->GL.NSIZE -= nb.SIZE;
    
    Status->RE.MESHES --;
    
    //clear mesh
    this->Mesh.Clear();
    
    if (DebugFunction)
    {
        std::cout << "RenderMesh " << this->Name << " - ";
        std::cout << this->Name << " Clear Complete ..." << std::endl;
    }
    
}

void RenderMesh::SetName(std::string SetName)
{
    
    this->Name = SetName;
    
}

void RenderMesh::SetTexture(RenderMaterial* NewTexture)
{
    
    this->Material = NewTexture;
    this->Name = Material->GetName();
    if (DebugFunction) std::cout << "RenderMesh SetTexture: " << this->Name << std::endl;

}

GLuint RenderMesh::GetVBO(void)
{
    
    return this->vb.VBO;
    
}

GLuint RenderMesh::GetCBO(void)
{
    
    return this->cb.CBO;
    
}


GLuint RenderMesh::GetNBO(void)
{
    
    return this->nb.VBO;
    
}

GLuint RenderMesh::GetTBO(void)
{
    
    return this->tb.VBO;
    
}

void RenderMesh::Bind(MeshBuffer Buffer)
{
    
    switch (Buffer)
    {
        
        case VBO:
            //enable the vertex location and bind the attribute buffer
            glEnableVertexAttribArray(*vb.SHADER_VARIABLE);
            glBindBuffer(vb.TYPE, vb.VBO);
            glVertexAttribPointer(*vb.SHADER_VARIABLE, 3, GL_FLOAT, GL_FALSE, 0, 0);            
            break;
            
        case CBO:
            //enable the coordinate location and bind the attribute buffer
            glEnableVertexAttribArray(*cb.SHADER_VARIABLE);
            glBindBuffer(cb.TYPE, cb.CBO);
            glVertexAttribPointer(*cb.SHADER_VARIABLE, 2, GL_FLOAT, GL_FALSE, 0, 0);            
            break;
            
        case NBO:
            //enable the normal location and bind the attribute buffer
            glEnableVertexAttribArray(*nb.SHADER_VARIABLE);
            glBindBuffer(nb.TYPE, nb.VBO);
            glVertexAttribPointer(*nb.SHADER_VARIABLE, 3, GL_FLOAT, GL_FALSE, 0, 0);            
            break;
            
        case TBO:
            //enable the tangent location and bind the attribute buffer
            glEnableVertexAttribArray(*tb.SHADER_VARIABLE);
            glBindBuffer(tb.TYPE, tb.VBO);
            glVertexAttribPointer(*tb.SHADER_VARIABLE, 3, GL_FLOAT, GL_FALSE, 0, 0);            
            break;
            
        default:
            break;
        
    }
    
}

void RenderMesh::Unbind(MeshBuffer Buffer)
{
    
    switch (Buffer)
    {
        
        case VBO:
            //disable the vertex attribute buffer
            glDisableVertexAttribArray(*vb.SHADER_VARIABLE);           
            break;
            
        case CBO:
            //disable the coordinate attribute buffer
            glDisableVertexAttribArray(*cb.SHADER_VARIABLE);           
            break;
            
        case NBO:
            //disable the normal attribute buffer
            glDisableVertexAttribArray(*nb.SHADER_VARIABLE);           
            break;
            
        case TBO:
            //disable the tangent attribute buffer
            glDisableVertexAttribArray(*tb.SHADER_VARIABLE);           
            break;
            
        default:
            break;
            
    }
    
}

void RenderMesh::DrawMesh(GLenum DrawMode)
{
    
    //enable the color vertex location and bind the attribute buffer
    this->Bind(VBO);
    this->Bind(CBO);
    this->Bind(NBO);

    //enable the texture attribute location and bind the texture buffer
    if (Material != NULL)
    {
        Material->SetMap(map_Kd);
        if (Material->IsLinked())
        {
            Material->Activate();
            Material->Bind();
            Material->UploadTextureParameters();
        }
    }
    
    //draw the corresponding elements
    glDrawArrays(DrawMode, 0, vb.COUNT);
    
    //disable the bound texture
    if (Material != NULL)
    {
        Material->SetMap(map_Kd);
        if (Material->IsLinked())
        {
            Material->Activate();
            Material->Unbind();
        }
    }

    //disable the attributes
    this->Unbind(VBO);
    this->Unbind(CBO);
    this->Unbind(NBO);
    
}


//getters


std::string RenderMesh::GetName(void)
{
    
    return this->Name;
    
}

RenderMaterial* RenderMesh::GetTexture(void)
{
    
    return this->Material;
    
}

std::string RenderMesh::GetMaterialName(void)
{
    
    return this->Material->GetName();
    
}


//private methods




