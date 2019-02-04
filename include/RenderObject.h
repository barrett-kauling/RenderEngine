/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RenderObject.h
 * Author: user
 *
 * Created on June 12, 2016, 6:21 PM
 */

#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H


#include "RenderMesh.h"


typedef struct
{
    std::vector<glm::vec3>* Vertex;
    std::vector<glm::vec2>* Coordinate;
    std::vector<glm::vec3>* Normal;
    void Init(void)
    {
        Vertex = new std::vector<glm::vec3>;
        Coordinate = new std::vector<glm::vec2>;
        Normal = new std::vector<glm::vec3>;       
    }
    void Clear(void)
    {
        this->Init();
        Vertex->clear();
        Coordinate->clear();
        Normal->clear();        
    }
} Data;

typedef struct
{
    int Smoothing;
    std::string Material;
    std::vector<Face>* FaceList;
    void Init()
    {
        FaceList = new std::vector<Face>;
    }
    void Clear(void)
    {
        this->Init();
        Smoothing = 0;
        Material = "";
    }
} Faces;

typedef struct
{
    std::string Name;
    std::vector<Faces>* MaterialFaces;
    MeshVariables* ShaderLocations;
    void Init(void)
    {
        MaterialFaces = new std::vector<Faces>;
        ShaderLocations = new MeshVariables;
    }
    void Clear(void)
    {
        this->Init();
        Name = "";
        MaterialFaces->clear();
        ShaderLocations->Clear();
    }
} RenderMaterials;


class RenderObject {
    
public:
    
    RenderObject(RenderMaterials*, Data*, RenderStatus*);
//    RenderObject(const RenderObject& orig);
    virtual ~RenderObject();
    
    //clear
    void Clear(RenderStatus*);

    //render
    void UploadModelMatrix(GLuint);
    void DrawObject(GLenum);
    
    //accessors
    void SetName(std::string);
    void SetModel(glm::mat4);
    void SetTextures(std::forward_list<RenderMaterial>*);
    
    //getters
    glm::mat4 GetModel(void);
    std::string GetName(void);
    std::forward_list<RenderMesh>* GetMeshes(void);
    
    //transform
    void TransformTranslate(glm::vec3);
    void TransformRotate(float, glm::vec3);
    void TransformScale(glm::vec3);
    
private:
    
    //
    void AddMesh(RenderMesh*);
    void UpdateTransform(void);
    
    //debug
    bool DebugInit = false;
    bool DebugFunction = false;
    
    //object properties
    std::string Name;
    
    //mesh model transformation matrix
    glm::mat4 Model;
    
    //mesh data
    std::forward_list<RenderMesh> Meshes;
    
};

#endif /* RENDEROBJECT_H */

