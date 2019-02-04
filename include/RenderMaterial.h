/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RenderTexture.h
 * Author: user
 *
 * Created on June 12, 2016, 6:21 PM
 */


#ifndef RENDERMATERIAL_H
#define RENDERMATERIAL_H


#include "RenderGlobal.h"
#include "RenderImage.h"


typedef enum
{
    map_Ka,
    map_Kd,
    map_Ks,
    map_Ke,
    map_Ns,
    map_d,
    map_Bump,
    bump,
    disp,
    decal,
    refl
} TextureMap;

typedef struct
{
    std::string Directory;
    std::string MaterialName;
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    glm::vec3 Ke;
    GLfloat Ns;
    GLfloat Ni;
    GLfloat d;
    GLuint illum;
    void Clear(void)
    {
        Directory = "";
        MaterialName = "";
        Ka = glm::vec3(0.0f, 0.0f, 0.0f);
        Kd = glm::vec3(0.0f, 0.0f, 0.0f);
        Ks = glm::vec3(0.0f, 0.0f, 0.0f);
        Ke = glm::vec3(0.0f, 0.0f, 0.0f);
        Ns = 0;
        Ni = 0.0f;
        d = 0.0f;
        illum = 0;
    }
} MaterialStruct;

typedef struct
{
    MaterialStruct MaterialData;
    RenderImage* map_Ka;
    RenderImage* map_Kd;
    RenderImage* map_Ks;
    RenderImage* map_Ke;
    RenderImage* map_Ns;
    RenderImage* map_d;
    RenderImage* map_Bump;
    RenderImage* bump;
    RenderImage* disp;
    RenderImage* decal;
    RenderImage* refl;
    void Init()
    {
        map_Ka = NULL;
        map_Kd = NULL;
        map_Ks = NULL;
        map_Ke = NULL;
        map_Ns = NULL;
        map_d = NULL;
        map_Bump = NULL;
        bump = NULL;
        disp = NULL;
        decal = NULL;
        refl = NULL;
    }
    void Clear()
    {
        this->Init();
        MaterialData.Clear();
    }
} RenderMaterialStruct;

    //0. Color on and Ambient off
    //1. Color on and Ambient on
    //2. Highlight on
    //3. Reflection on and Ray trace on
    //4. Transparency: Glass on, Reflection: Ray trace on
    //5. Reflection: Fresnel on and Ray trace on
    //6. Transparency: Refraction on, Reflection: Fresnel off and Ray trace on
    //7. Transparency: Refraction on, Reflection: Fresnel on and Ray trace on
    //8. Reflection on and Ray trace off
    //9. Transparency: Glass on, Reflection: Ray trace off
    //10. Casts shadows onto invisible surfaces

class RenderMaterial
{
    
public:
    
    RenderMaterial(RenderMaterialStruct*, RenderStatus*);
//    RenderTexture(const RenderTexture& orig);
    virtual ~RenderMaterial();
    
    //clear
    void Clear(RenderStatus*);
    
    //getters
    std::string GetName(void);
    
    //texture functions
    void SetMap(TextureMap);
    void LinkTexture(RenderImage*, RenderStatus*);
    
    //render loop
    GLuint IsLinked(void);
    void Activate(void);
    void Bind(void);
    void Unbind(void);
    void UploadTextureParameters(void);
    
private:

    //texture info
    std::string Name;
    
    //texture data
    RenderMaterialStruct* Material;
    
    RenderImage* Texture;
    
    //texture debug
    bool DebugInit = false;
    bool DebugRead = false;
    bool DebugData = false;
        
};

#endif /* RENDERMATERIAL_H */

