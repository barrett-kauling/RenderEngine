/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RenderImage.cpp
 * Author: user
 * 
 * Created on August 29, 2016, 8:32 AM
 */

#include "RenderImage.h"

RenderImage::RenderImage(ImageTextureStruct* ImageData, RenderStatus* Status)
{
    
    this->tc = new TextureCoordinateStruct;
    this->tc->Clear();
    this->tc->Image = ImageData;
    
    
    //glGenTextures
    glGenTextures(1, &tc->TCO);
    glActiveTexture(tc->ACTIVE);
    glBindTexture(tc->TARGET, tc->TCO);

    //status
    Status->GL.TCO++;
    
    
    if (DebugInit) std::cout << "TCO: " << tc->TCO << std::endl;
    
    //glTexImage2D
    glTexImage2D(tc->TARGET, 0, tc->Image->INTERNALFORMAT, tc->Image->Width, tc->Image->Height,
                    0, tc->Image->FORMAT, tc->Image->TYPE,  tc->Image->Data);
    
    //glBindTexture to 0
    glBindTexture(tc->TARGET, 0);
    
    Status->GL.TCSIZE += tc->Image->SIZE;
    
    
    
    if (DebugInit)
        std::cout << "Texture location: " << tc->Image << std::endl;
    
}

//RenderImage::RenderImage(const RenderImage& orig)
//{
//}

RenderImage::~RenderImage()
{
}

std::string RenderImage::GetName(void)
{
    
    return this->tc->Path;
    
}

void RenderImage::Clear(RenderStatus* Status)
{
    
    if (this->tc->Image->Data != NULL)
    {
        delete this->tc->Image->Data;
        this->tc->Image->Data = NULL;
    }
    
    //clean buffers
    glDeleteTextures(1, &tc->TCO);
    
    Status->GL.TCO --;
        
    Status->GL.TCSIZE -= tc->Image->SIZE;
    
}

void RenderImage::Activate(void)
{
    
    if (tc)
    {
        glActiveTexture(tc->ACTIVE);
    }
    
}

void RenderImage::Bind(void)
{
    
    if (tc)
    {
        glBindTexture(tc->TARGET, tc->TCO);
    }
    
}

void RenderImage::Unbind(void)
{
    
    if (tc)
    {
        glBindTexture(tc->TARGET, tc->TCO);
    }
    
}

void RenderImage::UploadTextureParameters(void)
{
    
    if (tc)
    {
        //set parameters of the current texture in the texture unit
        glTexParameteri(tc->TARGET, GL_TEXTURE_WRAP_S, tc->WRAP_S);
        glTexParameteri(tc->TARGET, GL_TEXTURE_WRAP_T, tc->WRAP_T);
        glTexParameteri(tc->TARGET, GL_TEXTURE_MIN_FILTER, tc->MIN_FILTER);
        glTexParameteri(tc->TARGET, GL_TEXTURE_MAG_FILTER, tc->MAG_FILTER);
    }
    else
    {
        return;
    }
    
}

TextureCoordinateStruct* RenderImage::GetTexture(void)
{
    
    return this->tc;
    
}