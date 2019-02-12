/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   RenderReader.h
 * Author: user
 *
 * Created on August 20, 2016, 2:10 PM
 */

#ifndef RENDERREADER_H
#define RENDERREADER_H

#include "RenderObject.h"
#include "RenderMaterial.h"

#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>
#include <sys/stat.h>

#include <png.h>

typedef struct
{
    png_byte Header[8];
    png_structp Pointer;
    png_infop Info;
    png_infop EndInfo;
    png_int_32 BytesPerRow;
    png_int_32 BitDepth;
    png_int_32 ColorType;
    png_bytep* RowPointers;
} ImageInfo;


typedef struct
{
    int OPEN_FLAGS;
    int MM_PROT;
    int MM_MAP;
    int fd;
    int rc;
    int page_sz;
    unsigned long file_sz;
    struct stat* stat_b; //UNIX fstat file info
    char* data;
    void Init(void)
    {
        data = new char;
        stat_b = new struct stat;
    }
    void Clear(void)
    {
        this->Init();
        OPEN_FLAGS = O_RDONLY;
        MM_PROT = PROT_READ;
        MM_MAP = MAP_PRIVATE;
        fd = 0;
        rc = -1;
        page_sz = getpagesize(); //POSIX function
        file_sz = 0;
    }
} ReadStruct;

typedef struct
{
    std::string Name;
    GLuint FaceCount;
    GLuint FileSize;
    Face BaseIndex;
    Data* MaterialData;
    std::vector<RenderMaterials>* MaterialObjects;
    void Init()
    {
        MaterialData = new Data;
        MaterialObjects = new std::vector<RenderMaterials>;
    }
    void Clear()
    {
        this->Init();
        MaterialObjects->clear();
        MaterialData->Clear();
        Name = "";
        FaceCount = 0;
        FileSize = 0;
    }
} RenderObjectData;

typedef struct
{
    MaterialStruct Materials;
    std::string map_Ka;
    std::string map_Kd;
    std::string map_Ks;
    std::string map_Ke;
    std::string map_Ns;
    std::string map_d;
    std::string map_Bump;
    std::string bump;
    std::string disp;
    std::string decal;
    std::string refl;
    void Clear(void)
    {
        map_Ka = "";
        map_Kd = "";
        map_Ks = "";
        map_Ke = "";
        map_Ns = "";
        map_d = "";
        map_Bump = "";
        bump = "";
        disp = "";
        decal = "";
        refl = "";
    }
} RenderTextureData;

typedef struct
{
    std::vector<ImageTextureStruct>* Images;
    std::vector<RenderTextureData>* Materials;
    void Init(void)
    {
        Images = new std::vector<ImageTextureStruct>;
        Materials = new std::vector<RenderTextureData>;        
    }
    void Clear(void)
    {
        Images->clear();
        Materials->clear();
    }
} RenderGenerateData;



class RenderReader {
public:
    
    RenderReader(RenderStatus*);
//    RenderReader(const RenderReader& orig);
    virtual ~RenderReader();

    //setters
    void setReadPageSize(bool, RenderStatus*);

    //read file functions
    RenderGenerateData* ReadMaterial(char* Filename);
    RenderObjectData* ReadWavefront(std::string Filename);
    int ReadImage(ImageTextureStruct*, char*);

    
private:
    
    void DebugMessage(const char* Message, const GLchar* Log);
    
    typedef struct
    {
        char n[128];
        unsigned long start;
        unsigned long end;
        unsigned long v;
        unsigned long vt;
        unsigned long vn;
        unsigned long s;
        unsigned long f;
        unsigned long ft;
        unsigned long fq;
        unsigned long fp;
        void Clear(void)
        {
            memset(n, 0, 128);
            v = 0;
            vt = 0;
            vn = 0;
            s = 0;
            f = 0;
            ft = 0;
            fq = 0;
            fp = 0;
        }
    } ReadObject;
    
    typedef struct
    {
        unsigned long start;
        unsigned long end;
        int o;
        char u[128];
        int s_val;
        ReadObject rso;
        void Clear(void)
        {
            start = 0;
            end = 0;
            s_val = 0;
            o = 0;
            memset(u, 0, 128);
            rso.Clear();
        }
    } ReadState;
    
    typedef struct
    {
        unsigned long start;
        unsigned long end;
        int o;
        void Clear(void)
        {
            start = 0;
            end = 0;
            o = 0;
        }
    } ParseState;
    
    bool DebugInit = true;
    bool DebugFunction = false;
    bool DebugRead = false;
    bool DebugData = false;
    
    bool ReadPage = true;
    
    std::list<ReadObject> l_ro;
    ReadObject ro;
    ReadState rs;
    ParseState ps;
    ReadStruct rd;
    
    RenderObjectData* Object;
    
    RenderStatus* Status;
    
};


#endif /* RENDERREADER_H */

