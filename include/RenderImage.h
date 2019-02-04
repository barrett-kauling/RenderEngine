/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RenderImage.h
 * Author: user
 *
 * Created on August 29, 2016, 8:32 AM
 */

#ifndef RENDERIMAGE_H
#define RENDERIMAGE_H

#include "RenderGlobal.h"

typedef struct
{
    GLuint SIZE;
    GLuint Height;
    GLuint Width;
    GLint Level;
    GLenum FORMAT;
    GLenum INTERNALFORMAT;
    GLenum TYPE;
    GLubyte* Data;
    void Init(void)
    {
        Data = new GLubyte;
    }
    void Clear(void)
    {
        this->Init();
        SIZE = 0;
        Height = 0;
        Width = 0;
        Level = 0;
        FORMAT = 0;
        INTERNALFORMAT = 0;
        TYPE = 0;
        Data = NULL;
    }
} ImageTextureStruct;

typedef struct
{
    GLuint TCO;
    GLenum ACTIVE;
    GLenum TARGET;
    GLenum WRAP_S;
    GLenum WRAP_T;
    GLenum MIN_FILTER;
    GLenum MAG_FILTER;
    std::string Path;
    ImageTextureStruct* Image;
    void Init(void)
    {
        Image = new ImageTextureStruct;
    }
    void Clear(void)
    {
        this->Init();
        TCO = 0;
        ACTIVE = GL_TEXTURE0;
        TARGET = GL_TEXTURE_2D;
        WRAP_S = GL_CLAMP_TO_BORDER;
        WRAP_T = GL_CLAMP_TO_BORDER;
        MIN_FILTER = GL_LINEAR;
        MAG_FILTER = GL_LINEAR;
    }
} TextureCoordinateStruct;


class RenderImage {
public:
    
    RenderImage(ImageTextureStruct*, RenderStatus*);
//    RenderImage(const RenderImage& orig);
    virtual ~RenderImage();
    
    std::string GetName(void);
    
    //gpu functions
    void Clear(RenderStatus*);
    void Activate(void);
    void Bind(void);
    void Unbind(void);
    void UploadTextureParameters(void);
    
    //render functions
    TextureCoordinateStruct* GetTexture(void);
    
private:

    bool DebugInit = false;
    
    TextureCoordinateStruct* tc;
    
};

#endif /* RENDERIMAGE_H */

