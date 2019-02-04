/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RenderTexture.cpp
 * Author: Barrett Kauling
 * 
 * Created on June 12, 2016, 6:21 PM
 */

#include "RenderMaterial.h"


RenderMaterial::RenderMaterial(RenderMaterialStruct* Mtr, RenderStatus* Status)
{
    
    if (DebugInit) std::cout << "RenderTexture Constructor ..." << std::endl;
    
    
    this->Texture = NULL;
    
    this->Material = new RenderMaterialStruct;
    this->Material->Clear();
    
    this->Material = Mtr;
    this->Name = Material->MaterialData.MaterialName;

    
    Status->RE.TEXTURES++;
    
    if (DebugInit) 
    {
        std::cout << std::endl;
        std::cout << "RenderTexture Constructor complete ..." << std::endl;
        std::cout << std::endl;
    }
    
}

//RenderTexture::RenderTexture(const RenderTexture& orig) {
//}

RenderMaterial::~RenderMaterial() {
    
    if (DebugInit)
        std::cout << "RenderTexture " << this->Name << " Deconstructor ... " << std::endl;

}

void RenderMaterial::Clear(RenderStatus* Status)
{
    
    if (DebugInit)
        std::cout << "RenderTexture " << this->Name << " Clear ..." << std::endl;
    

    
    if (DebugInit)
        std::cout << "RenderTexture " << this->Name << " Clear Complete ..." << std::endl;
    
}


//getters


std::string RenderMaterial::GetName(void)
{
    
    return this->Name;
    
}

void RenderMaterial::SetMap(TextureMap Map)
{
    
    switch (Map)
    {
        
        case map_Ka:
            this->Texture = this->Material->map_Ka;
            break;
            
        case map_Kd:
            this->Texture = this->Material->map_Kd;
            break;
            
        case map_Ks:
            this->Texture = this->Material->map_Ks;
            break;
            
        case map_Ke:
            this->Texture = this->Material->map_Ke;
            break;
            
        case map_Ns:
            this->Texture = this->Material->map_Ns;
            break;
            
        case map_d:
            this->Texture = this->Material->map_d;
            break;
            
        case map_Bump:
            this->Texture = this->Material->map_Bump;
            break;
            
        case bump:
            this->Texture = this->Material->bump;
            break;
            
        case disp:
            this->Texture = this->Material->disp;
            break;
            
        case decal:
            this->Texture = this->Material->decal;
            break;
            
        case refl:
            this->Texture = this->Material->refl;
            break;
            
        default:
            this->Texture = NULL;
            return;
            break;
            
    }

}

void RenderMaterial::LinkTexture(RenderImage* Image, RenderStatus* Status)
{
    

    


    
}

GLuint RenderMaterial::IsLinked(void)
{
    
    if (this->Texture)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}

void RenderMaterial::Activate(void)
{
    
    if (this->Texture)
    {
        this->Texture->Activate();
    }
    
}

void RenderMaterial::Bind(void)
{
    
    if (this->Texture)
    {
        this->Texture->Bind();
    }
    
}

void RenderMaterial::Unbind(void)
{
    
    if (this->Texture)
    {
        this->Texture->Unbind();
    }
    
}

void RenderMaterial::UploadTextureParameters(void)
{
    
    if (this->Texture)
    {
        this->Texture->UploadTextureParameters();
    }
    else
    {
        return;
    }
    
}
