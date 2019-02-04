/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RenderMesh.h
 * Author: user
 *
 * Created on February 6, 2016, 7:56 AM
 */

#ifndef RENDERMESH_H
#define RENDERMESH_H


#include "RenderMaterial.h"


typedef enum
{
    VBO,
    CBO,
    NBO,
    TBO
} MeshBuffer;

typedef struct
{
    GLuint Vertex;
    GLuint Coordinate;
    GLuint Normal;
} Face;

typedef struct
{
    std::vector<glm::vec3>* LOCATION;
    GLuint SIZE;
    GLuint COUNT;
    GLuint VBO;
    GLuint TYPE;
    GLuint DRAWSTYLE;
    GLuint* SHADER_VARIABLE;
} VectorBufferStruct;

typedef struct
{
    std::vector<glm::vec2>* LOCATION;
    GLuint SIZE;
    GLuint COUNT;
    GLuint CBO;
    GLuint TYPE;
    GLuint DRAWSTYLE;
    GLuint* SHADER_VARIABLE;
} CoordinateBufferStruct;

typedef struct
{
    GLuint* VertexLocation;
    GLuint* CoordinateLocation;
    GLuint* NormalLocation;
    GLuint* TangentLocation;
    void Clear(void)
    {
        VertexLocation = NULL;
        CoordinateLocation = NULL;
        NormalLocation = NULL;
        TangentLocation = NULL;        
    }
} MeshVariables;

typedef struct
{
    std::vector<glm::vec3>* Vertex;
    std::vector<glm::vec2>* Coordinate;
    std::vector<glm::vec3>* Normal;
    std::vector<glm::vec3>* Tangent;
    MeshVariables* ShaderLocations;
    RenderMaterial* TextureMap;
    int SmoothShading = 0;
    void Init(void)
    {
        Vertex = new std::vector<glm::vec3>;
        Coordinate = new std::vector<glm::vec2>;
        Normal = new std::vector<glm::vec3>;
        Tangent = new std::vector<glm::vec3>;
        ShaderLocations = new MeshVariables;
    }
    void Clear(void)
    {
        this->Init();
        Vertex->clear();
        Coordinate->clear();
        Normal->clear();
        Tangent->clear();
        SmoothShading = 0;
        ShaderLocations->Clear();
    }
} RenderMeshData;


class RenderMesh {
public:
    
    RenderMesh(RenderMeshData*, RenderStatus*);
//    RenderMesh(const RenderMesh& orig);
    virtual ~RenderMesh();

    //set parameters
    void SetName(std::string);
    void SetTexture(RenderMaterial*);
    
    //getters for render
    GLuint GetVBO(void);
    GLuint GetCBO(void);
    GLuint GetNBO(void);
    GLuint GetTBO(void);
    
    void Bind(MeshBuffer);
    void Unbind(MeshBuffer);
    void DrawMesh(GLenum);
    
    //getter
    std::string GetName(void);
    RenderMaterial* GetTexture(void);
    std::string GetMaterialName(void);
    
    //methods
    void Clear(RenderStatus*); //clear data
    
    
private:

    //debug
    bool DebugInit = false;
    bool DebugFunction = false;
    
    //mesh information
    std::string Name;
    
    //transformation data
    glm::mat4 Model;
    
    //mesh data
    RenderMeshData Mesh;
    
    //Material info
    VectorBufferStruct vb;
    CoordinateBufferStruct cb;
    VectorBufferStruct nb;
    VectorBufferStruct tb;
    
    //texture reference
    RenderMaterial* Material;
    
};

#endif /* RENDERMESH_H */

