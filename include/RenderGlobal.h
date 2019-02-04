/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RenderGlobal.h
 * Author: user
 *
 * Created on August 12, 2016, 8:18 PM
 */


#ifndef RENDERGLOBAL_H
#define RENDERGLOBAL_H


#include <cstdlib>
#include <cstdarg>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

#include <vector>
#include <list>
#include <forward_list>

#include <iterator>
#include <algorithm>

#include <locale>

#include <time.h>

//#include <math.h>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/transform.hpp>


#define F1 GL_POINTS
#define F2 GL_LINES
#define F3 GL_LINE_LOOP
#define F4 GL_LINE_STRIP
#define F5 GL_TRIANGLES 
#define F6 GL_TRIANGLE_STRIP
#define F7 GL_TRIANGLE_FAN
#define F8 GL_PATCHES
#define F9 GL_POLYGON
#define F10 GL_QUADS
#define F11 GL_QUAD_STRIP

#define MEM_C_SIZE 13
#define BUF_C_SIZE 5
#define STA_C_SIZE 11

typedef struct
{
    GLuint COUNT;
    GLulong SIZE;
    GLuint VBO;
    GLulong VSIZE;
    GLuint CBO;
    GLulong CSIZE;
    GLuint NBO;
    GLulong NSIZE;
    GLuint TBO;
    GLulong TSIZE;
    GLuint TCO;
    GLulong TCSIZE;
} gl;

typedef struct
{
    GLuint OBJECTS;
    GLuint MESHES;
    GLuint TEXTURES;
} re;

typedef struct
{
    bool ReadPageSize;
    int file_size;
    int page_size;
    double elapsed;
} rd;

typedef struct
{
    gl GL;
    re RE;
    rd RD;
    std::string Directory;
} RenderStatus;


template<class T>
std::string num_sep(T value){
    struct Numpunct: public std::numpunct<char>{
    protected:
        virtual char do_thousands_sep() const{return ',';}
        virtual std::string do_grouping() const{return "\03";}
    };
    std::stringstream ss;
    ss.imbue({std::locale(), new Numpunct});
    std::cout.flags(std::ios::right);
    ss << value;
    return ss.str();
}

#endif /* RENDERGLOBAL_H */

