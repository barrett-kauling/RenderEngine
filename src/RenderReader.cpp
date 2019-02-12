/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   RenderReader.cpp
 * Author: Barrett Kauling
 *
 * Created on August 20, 2016, 2:10 PM
 */


#include "RenderReader.h"

RenderReader::RenderReader(RenderStatus* Status) {

    if (DebugInit) this->DebugMessage("Default Reader Constructor ...\n", NULL);

    l_ro.clear();
    ro.Clear();
    rs.Clear();
    ps.Clear();
    rd.Clear();

    this->Status = Status;
    
    if (DebugInit) this->DebugMessage("Default Reader Constructor complete ...\n\n", NULL);

}

//RenderReader::RenderReader(const RenderReader& orig) {
//}

RenderReader::~RenderReader() {
}

void RenderReader::setReadPageSize(bool setReadPage, RenderStatus* Status)
{
    
    //update the local ReadPage boolean and update the RenderEngine status
    ReadPage = setReadPage;
    Status->RD.ReadPageSize = ReadPage;
    
}

RenderGenerateData* RenderReader::ReadMaterial(char* Filename)
{

    if (DebugInit)
    {
        std::cout << std::endl;
        std::cout << "read texture ... " << Filename << std::endl;
    }
    
    std::cout << "initializing and clearing RenderGenerateData ... " << std::endl;
    //initialize and clear local render generate data for materials
    RenderGenerateData* mat = new RenderGenerateData;
    mat->Init();
    mat->Clear();

    std::cout << "clearing local structs and states ... " << std::endl;
    //clear local structs and states
    rs.Clear();
    rd.Clear();
    ps.Clear();

    //start C based system memory mapping
    
    std::cout << "open file descriptor ... " << std::endl;
    //open file descriptor
    std::string resource(Filename);
    resource.insert(0,this->Status->Directory);
    Filename = (char*) resource.c_str();
    rd.fd = open(Filename, rd.OPEN_FLAGS);
    if (!rd.fd)
    {
        std::cout << std::endl;
        std::cout << "Unable to open a file descriptor for: ";
        std::cout << Filename << std::endl;
        return mat;
    }

    //retrieve file status
    rd.rc = fstat(rd.fd, rd.stat_b);
    if (rd.rc == -1)
    {
        std::cout << std::endl;
        std::cout << "Unable to retrieve file status for file: ";
        std::cout << Filename << std::endl;
        return mat;
    }
    else
    {
        rd.file_sz = rd.stat_b->st_size;
    }

    if (DebugInit)
    {
        std::cout << std::endl;
        std::cout << "file open and rc complete ... " << std::endl;
        std::cout << "filename: " << Filename << std::endl;
        std::cout << "pagesize: ";
        std::cout.flags(std::ios::right) << std::cout.width(12);
        std::cout << num_sep(rd.page_sz) << std::endl;
        std::cout << "filesize: ";
        std::cout.flags(std::ios::right) << std::cout.width(12);
        std::cout << num_sep(rd.file_sz) << std::endl;
        std::cout << "begin mmap ..." << std::endl;
    }

    //~ if (ReadPage)
    //~ {

    //~ }
    if (false) {}
    else
    {
        
        std::cout << "Materials MMAP by file size ... " << std::endl;
        rd.data = (char*) mmap(NULL, rd.file_sz, rd.MM_PROT, rd.MM_MAP, rd.fd, 0);

        ps.start = 0;
        ps.end = ps.start-2;

        std::cout << "Materials reading data ... " << std::endl;
        //determine file information
        std::cout << "Materials read file size ... " << rd.file_sz << std::endl;
        for (unsigned long n = 0; n < rd.file_sz; n++)
        {

            if (rd.data[n] == '\n')
            {
                
                if ( rd.data[n-1] == '\n' ) //blank newline
                {
                    //parse state initialization from \n\n
                    ps.start = ps.end+1;
                    ps.end = n;
                }
                else
                {
                    //parse state initialization from \n
                    ps.start = ps.end+2;
                    ps.end = n;
                }
                //local variable
                unsigned long i = ps.start;

                //printable characters detection
                //~ if ( (ps.end - ps.start) <= 1 )
                if (false)
                {
                    //~ n += 0;
                    //~ ps.end = n-1;
                    //~ std::cout << "n == " << ps.end - ps.start << " ps.end: " << ps.end << " ps.start: " << ps.start << std::endl;
                }
                else
                {
                    for (unsigned long c = ps.end; c > ps.start; c--)
                    {
                        if (rd.data[c] < 32) ps.end--;
                        else break;
                    }

                    //delimiter clear
                    unsigned long pt = 0;

                    std::cout << "Materials reading values ... rd.data[" << i << "] " << rd.data[i] << std::endl;
                    //values
                    if ((rd.data[i] == 'n') && (rd.data[i+6] == ' ')) //newmtl
                    {
                        std::cout << "Materials newmtl ... " << std::endl;
                        RenderTextureData m;
                        char n[128];
                        memset(n, 0, 128);
                        for (int c = 0; c < 128; c++)
                            (rd.data[i+8+c] == '\n') ? c = 128 : n[c] = rd.data[i+8+c];
                        m.Materials.MaterialName = n;
                        mat->Materials->push_back(m);
                    }
                    if ((rd.data[i] == 'K') && (rd.data[i+1] == 'a'))
                    {
                        pt = 0;
                        float f[3];
                        for (unsigned long t = i+3; t < ps.end; t++)
                        {
                            if (rd.data[t] == ' ')
                            {
                                f[pt++] = atof(&rd.data[t]);
                                t += 9;
                            }
                        }
                        mat->Materials->back().Materials.Ka = glm::vec3(f[0],f[1],f[2]);
                    }
                    if ((rd.data[i] == 'K') && (rd.data[i+1] == 'd'))
                    {
                        pt = 0;
                        float f[3];
                        for (unsigned long t = i+3; t < ps.end; t++)
                        {
                            if (rd.data[t] == ' ')
                            {
                                f[pt++] = atof(&rd.data[t]);
                                t += 9;
                            }
                        }
                        mat->Materials->back().Materials.Kd = glm::vec3(f[0],f[1],f[2]);
                    }
                    if ((rd.data[i] == 'K') && (rd.data[i+1] == 's'))
                    {
                        pt = 0;
                        float f[3];
                        for (unsigned long t = i+3; t < ps.end; t++)
                        {
                            if (rd.data[t] == ' ')
                            {
                                f[pt++] = atof(&rd.data[t]);
                                t += 9;
                            }
                        }
                        mat->Materials->back().Materials.Ks = glm::vec3(f[0],f[1],f[2]);
                    }
                    if ((rd.data[i] == 'K') && (rd.data[i+1] == 'e'))
                    {
                        pt = 0;
                        float f[3];
                        for (unsigned long t = i+3; t < ps.end; t++)
                        {
                            if (rd.data[t] == ' ')
                            {
                                f[pt++] = atof(&rd.data[t]);
                                t += 9;
                            }
                        }
                        mat->Materials->back().Materials.Ke = glm::vec3(f[0],f[1],f[2]);
                    }
                    if ((rd.data[i] == 'N') && (rd.data[i+1] == 's'))
                    {
                        GLfloat f = atof(&rd.data[i+3]);
                        mat->Materials->back().Materials.Ns = f;
                    }
                    if ((rd.data[i] == 'N') && (rd.data[i+1] == 'i'))
                    {
                        GLfloat f = atof(&rd.data[i+3]);
                        mat->Materials->back().Materials.Ni = f;
                    }
                    if ((rd.data[i] == 'd') && (rd.data[i+1] == ' '))
                    {
                        GLfloat f = atof(&rd.data[i+2]);
                        mat->Materials->back().Materials.d = f;
                    }
                    if ((rd.data[i] == 'i') && (rd.data[i+6] == ' '))
                    {
                        GLuint f = atoi(&rd.data[i+8]);
                        mat->Materials->back().Materials.illum = f;
                    }
                    
                    std::cout << "Materials reading bitmaps ... " << std::endl;
                    //bitmaps
                    if ((rd.data[i] == 'm') && (rd.data[i+4] == 'K') && (rd.data[i+5] == 'a'))
                    {
                        char m[128];
                        memset(m, 0, 128);
                        for (int c = 0; c < 128; c++)
                            (rd.data[i+6+c] == '\n') ? c = 128 : m[c] = rd.data[i+6+c];
                        mat->Materials->back().map_Ka = m;
                    }
                    if ((rd.data[i] == 'm') && (rd.data[i+4] == 'K') && (rd.data[i+5] == 'd'))
                    {
                        char m[128];
                        memset(m, 0, 128);
                        for (int c = 0; c < 128; c++)
                            (rd.data[i+6+c] == '\n') ? c = 128 : m[c] = rd.data[i+6+c];
                        mat->Materials->back().map_Kd = m;
                    }
                    if ((rd.data[i] == 'm') && (rd.data[i+4] == 'K') && (rd.data[i+5] == 's'))
                    {
                        char m[128];
                        memset(m, 0, 128);
                        for (int c = 0; c < 128; c++)
                            (rd.data[i+6+c] == '\n') ? c = 128 : m[c] = rd.data[i+6+c];
                        mat->Materials->back().map_Ks = m;
                    }
                    if ((rd.data[i] == 'm') && (rd.data[i+4] == 'K') && (rd.data[i+5] == 'e'))
                    {
                        char m[128];
                        memset(m, 0, 128);
                        for (int c = 0; c < 128; c++)
                            (rd.data[i+6+c] == '\n') ? c = 128 : m[c] = rd.data[i+6+c];
                        mat->Materials->back().map_Ke = m;
                    }
                    if ((rd.data[i] == 'm') && (rd.data[i+4] == 'N') && (rd.data[i+5] == 's'))
                    {
                        char m[128];
                        memset(m, 0, 128);
                        for (int c = 0; c < 128; c++)
                            (rd.data[i+6+c] == '\n') ? c = 128 : m[c] = rd.data[i+6+c];
                        mat->Materials->back().map_Ns = m;
                    }
                    if ((rd.data[i] == 'm') && (rd.data[i+4] == 'd'))
                    {
                        char m[128];
                        memset(m, 0, 128);
                        for (int c = 0; c < 128; c++)
                            (rd.data[i+6+c] == '\n') ? c = 128 : m[c] = rd.data[i+6+c];
                        mat->Materials->back().map_d = m;
                    }
                    if ((rd.data[i] == 'm') && (rd.data[i+4] == 'B'))
                    {
                        char m[128];
                        memset(m, 0, 128);
                        for (int c = 0; c < 128; c++)
                            (rd.data[i+10+c] == '\n') ? c = 128 : m[c] = rd.data[i+10+c];
                        mat->Materials->back().map_Bump = m;
                    }
                    if ((rd.data[i] == 'b') && (rd.data[i+1] == 'u'))
                    {
                        char m[128];
                        memset(m, 0, 128);
                        for (int c = 0; c < 128; c++)
                            (rd.data[i+5+c] == '\n') ? c = 128 : m[c] = rd.data[i+5+c];
                        mat->Materials->back().bump = m;
                    }
                    if ((rd.data[i] == 'd') && (rd.data[i+1] == 'i'))
                    {
                        char m[128];
                        memset(m, 0, 128);
                        for (int c = 0; c < 128; c++)
                            (rd.data[i+5+c] == '\n') ? c = 128 : m[c] = rd.data[i+5+c];
                        mat->Materials->back().disp = m;
                    }
                    if ((rd.data[i] == 'd') && (rd.data[i+1] == 'e'))
                    {
                        char m[128];
                        memset(m, 0, 128);
                        for (int c = 0; c < 128; c++)
                            (rd.data[i+6+c] == '\n') ? c = 128 : m[c] = rd.data[i+6+c];
                        mat->Materials->back().decal = m;
                    }
                    if ((rd.data[i] == 'r') && (rd.data[i+1] == 'e'))
                    {
                        char m[128];
                        memset(m, 0, 128);
                        for (int c = 0; c < 128; c++)
                            (rd.data[i+5+c] == '\n') ? c = 128 : m[c] = rd.data[i+5+c];
                        mat->Materials->back().refl = m;
                    }
                }
            }

        }

    }
    
    std::cout << "Materials read complete ..." << std::endl;
    std::cout << "Materials #: " << mat->Materials->size() << std::endl;
    
    for (std::vector<RenderTextureData>::iterator i = mat->Materials->begin(); i != mat->Materials->end(); i++)
    {
        
        std::cout << "iterating materials for textures ... " << std::endl;
        
        if ( i->map_Ka != "" )
            
        if ( i->map_Kd != "" ) this->ReadImage(&mat->Images->back(), (char*) i->map_Kd.c_str() );
            
        if ( i->map_Ks != "" )
            
        if ( i->map_Ke != "" )
            
        if ( i->map_Ns != "" )
            
        if ( i->map_d != "" )
            
        if ( i->map_Bump != "" ) this->ReadImage(&mat->Images->back(), (char*) i->map_Bump.c_str() );
            
        if ( i->bump != "" )
            
        if ( i->disp != "" )
            
        if ( i->decal != "" )
            
        if ( i->refl != "" )
            
        ;
    }
    
    return (RenderGenerateData*) mat;

}

RenderObjectData* RenderReader::ReadWavefront(std::string Filename)
{
    if (DebugInit)
    {
        std::cout << std::endl;
        std::cout << "read wavefront ... " << std::endl;
    }

    Object = new RenderObjectData;
    Object->Clear();

    //initialize local struct
    l_ro.clear();
    ro.Clear();
    rs.Clear();
    rd.Clear();

    //start C based system memory mapping

    //open file descriptor
    rd.fd = open(Filename.c_str(), rd.OPEN_FLAGS);
    if (!rd.fd)
    {
        std::cout << std::endl;
        std::cout << "Unable to open a file descriptor for: ";
        std::cout << Filename.c_str() << std::endl;
        return Object;
    }

    //retrieve file status
    rd.rc = fstat(rd.fd, rd.stat_b);
    if (rd.rc == -1)
    {
        std::cout << std::endl;
        std::cout << "Unable to retrieve file status for file: ";
        std::cout << Filename.c_str() << std::endl;
        return Object;
    }
    else
    {
        rd.file_sz = rd.stat_b->st_size;
    }

    if (DebugInit)
    {
        std::cout << std::endl;
        std::cout << "file open and rc complete ... " << std::endl;
        std::cout << "filename: " << Filename.c_str() << std::endl;
        std::cout << "pagesize: ";
        std::cout.flags(std::ios::right) << std::cout.width(12);
        std::cout << num_sep(rd.page_sz) << std::endl;
        std::cout << "filesize: ";
        std::cout.flags(std::ios::right) << std::cout.width(12);
        std::cout << num_sep(rd.file_sz) << std::endl;
        std::cout << "begin mmap ..." << std::endl;
    }

    clock_t start = clock();

    std::cout << "ReadPage: " << ReadPage << std::endl;
    std::cin;
    
    //begin mmap
    if (ReadPage) //read iteratively by page size
    {
        std::cout << "read page iteratively by page size ..." << std::endl;
        std::cin;
        //iterate pages until file size is reached
        for (unsigned long offset = 0; offset < rd.file_sz; offset += rd.page_sz)
        {
            //read a page into RAM
            rd.data = (char*) mmap(NULL, rd.page_sz, rd.MM_PROT, rd.MM_MAP, rd.fd, offset);

            //iterate the bytes read
            for (int i = 0; i < rd.page_sz; i++)
            {
                //read the parsed mtllib file declarations
                //~ if ( rd.data[i] == 'm' )
                //~ {
                    //~ std::cout << std::endl << "rd.data[" << i << "] == 'm'" << std::endl;
                    //~ std::cout << "&rd.data " << (void*) (&rd.data + i*sizeof(char*) + w*sizeof(char*)) << std::endl;
                    //~ int n = 15;
                    //~ std::cout << "(size_t) " << (size_t) n-w << std::endl;
                //~ }
                const int w = 6;
                if ( (rd.data[i] == 'm' ) && (rd.data[i+w] == ' ') )
                {
                    std::cout << "mtllib reached, parsing ... " << std::endl;
                    int n = w+1;
                    while ( n < rd.page_sz && rd.data[i+n] != '\n' )
                    {
                        n++;
                    }
                    
                    char* mtlfile = new char[n-w];
                    memset(mtlfile,0,n-w);
                    memcpy((void*) mtlfile, (void*) (rd.data + i*sizeof(char) + (w+1)*sizeof(char)), (size_t) n-(w+1));
                    
                    std::cout << "mtllib parsed: '" << mtlfile << "'" << std::endl;
                    
                    this->ReadMaterial(mtlfile);
                    
                }

                //parse newline characters
                if ( (rd.data[i] == 'o') && (rd.data[i+1] == ' ' ) )
                {
                    //begin file
                    rs.o++;
                }
            }
            munmap(rd.data, rd.page_sz);
        }
        
        exit(0);

    }
    
    else //read entire file
    {
        
        std::cout << "read entire file ..." << std::endl;

        rd.data = (char*) mmap(NULL, rd.file_sz, rd.MM_PROT, rd.MM_MAP, rd.fd, 0);

        //determine file information
        for (unsigned long n = 0; n < rd.file_sz; n++)
        {

            if (rd.data[n] == char(10))
            {

                rs.start = ++rs.end;
                rs.end = n;

                //determine characteristics
                for (unsigned long i = rs.start; i < rs.end; i++)
                {

                    if ((rd.data[i] == 'o') && (rd.data[i+1] == ' '))
                    {
                        if (ro.v)
                        {
                            ro.end = i-1;
                            rs.o++;
                            rs.rso.v += ro.v;
                            rs.rso.vt += ro.vt;
                            rs.rso.vn += ro.vn;
                            rs.rso.s += ro.s;
                            rs.rso.f += ro.f;
                            rs.rso.ft += ro.ft;
                            rs.rso.fq += ro.fq;
                            rs.rso.fp += ro.fp;
                            l_ro.emplace_back(ro);
                            ro.Clear();
                            ro.start = i;
                            memset(ro.n, 0, 128);
                            for (int c = 0; c < 128; c++)
                                (rd.data[i+2+c] == '\n') ? c = 128 : ro.n[c] = rd.data[i+2+c];
                        }
                        else
                        {
                            ro.start = i;
                            memset(ro.n, 0, 128);
                            for (int c = 0; c < 128; c++)
                                (rd.data[i+2+c] == '\n') ? c = 128 : ro.n[c] = rd.data[i+2+c];
                        }
                    }

                    if ((rd.data[i] == 'v') && (rd.data[i+1] == ' '))
                        ro.v++;
                    if ((rd.data[i] == 'v') && (rd.data[i+1] == 't') && (rd.data[i+2] == ' '))
                        ro.vt++;
                    if ((rd.data[i] == 'v') && (rd.data[i+1] == 'n') && (rd.data[i+2] == ' '))
                        ro.vn++;
                    if ((rd.data[i] == 's') && (rd.data[i+1] == ' '))
                        ro.s++;
                    if ((rd.data[i] == 'f') && (rd.data[i+1] == ' '))
                    {
                        int faces_this_line = 0;
                        for (unsigned long ft = rs.start; ft < rs.end; ft++)
                            if (rd.data[ft] == ' ')
                                faces_this_line++;

                        switch(faces_this_line)
                        {
                            case 1:
                            case 2:
                                break;
                            case 3:
                                ro.f += 3;
                                ro.ft++;
                                break;
                            case 4:
                                ro.f += 4;
                                ro.fq++;
                                break;
                            default:  //n sided polygon
                                ro.f += faces_this_line;
                                ro.fp++;
                        }

                    }
                }
            }//end newline identification
        } //read file complete

        //append current upon completion
        if (ro.v)
        {
            ro.end = rd.file_sz;
            rs.o++;
            rs.rso.v += ro.v;
            rs.rso.vt += ro.vt;
            rs.rso.vn += ro.vn;
            rs.rso.s += ro.s;
            rs.rso.f += ro.f;
            rs.rso.ft += ro.ft;
            rs.rso.fq += ro.fq;
            rs.rso.fp += ro.fp;
            l_ro.emplace_back(ro);
            ro.Clear();
        }


        if (DebugRead)
        {
            std::cout << std::endl;
            std::cout << "Reserved size ... " << std::endl;
            std::cout << "Object vertex size:     ";
            std::cout.flags(std::ios::right) << std::cout.width(9);
            std::cout << num_sep(Object->MaterialData->Vertex->size()) << std::endl;
            std::cout << "Object coordinate size: ";
            std::cout.flags(std::ios::right) << std::cout.width(9);
            std::cout << num_sep(Object->MaterialData->Coordinate->size()) << std::endl;
            std::cout << "Object normal size:     ";
            std::cout.flags(std::ios::right) << std::cout.width(9);
            std::cout << num_sep(Object->MaterialData->Normal->size()) << std::endl;
            std::cout << "Object size:            ";
            std::cout.flags(std::ios::right) << std::cout.width(9);
            std::cout << num_sep(Object->MaterialObjects->size()) << std::endl;
        }



        //parse each identified object before unmapping
        ps.Clear();
        if (DebugData) std::cout << std::endl;

        for (std::list<ReadObject>::iterator r = l_ro.begin(); r != l_ro.end(); r++)
        {

            RenderMaterials* ob = new RenderMaterials;
            ob->Clear();

            ob->Name.assign(r->n,128);

            Faces* rf = new Faces;
            rf->Clear();

            if (DebugData)
            {
                 std::cout.flags(std::ios::right) << std::cout.width(2);
                 std::cout << ++ps.o;
                 std::cout.flags(std::ios::right) << std::cout.width(10);
                 std::cout << r->start;
                 std::cout.flags(std::ios::right) << std::cout.width(10);
                 std::cout << r->end;
                 std::cout << std::endl;
            }

            ps.start = r->start;
            ps.end = ps.start-2;

            for (unsigned long n = r->start; n <= r->end; n++)
            {

                if (rd.data[n] == '\n')
                {

                    //parse state initialization
                    ps.start = ps.end+2;
                    ps.end = n;

                    //local variable
                    unsigned long i = ps.start;

                    //printable characters detection
                    for (unsigned long c = ps.end; c > ps.start; c--)
                    {
                        if (rd.data[c] < 32) ps.end--;
                        else break;
                    }

                    //delimiter clear
                    unsigned long pt = 0;

                    //determine characteristics
                    //vertices
                    if ((rd.data[i] == 'v') && (rd.data[i+1] == ' '))
                    {
                        pt = 0;
                        float f[3];
                        for (unsigned long t = i+1; t < ps.end; t++)
                        {
                            if (rd.data[t] == ' ')
                            {
                                f[pt++] = atof(&rd.data[t]);
                                t += 8;
                            }
                        }
                        Object->MaterialData->Vertex->push_back(glm::vec3(f[0],f[1],f[2]));
                    }
                    //coordinates
                    if ((rd.data[i] == 'v') && (rd.data[i+1] == 't') && (rd.data[i+2] == ' '))
                    {
                        pt = 0;
                        float f[2];
                        for (unsigned long t = i+2; t < ps.end; t++)
                        {
                            if (rd.data[t] == ' ')
                            {
                                f[pt++] = atof(&rd.data[t]);
                                t += 8;
                            }
                        }
                        Object->MaterialData->Coordinate->push_back(glm::vec2(f[0],f[1]));
                    }
                    //normals
                    if ((rd.data[i] == 'v') && (rd.data[i+1] == 'n') && (rd.data[i+2] == ' '))
                    {
                        pt = 0;
                        float f[3];
                        for (unsigned long t = i+2; t < ps.end; t++)
                        {
                            if (rd.data[t] == ' ')
                            {
                                f[pt++] = atof(&rd.data[t]);
                                t += 8;
                            }
                        }
                        Object->MaterialData->Normal->push_back(glm::vec3(f[0],f[1],f[2]));
                    }
                    //materials
                    if ((rd.data[i] == 'u') && (rd.data[i+6] == ' '))
                    {
                        memset(rs.u, 0, 128);
                        for (int c = 0; c < 128; c++)
                        {
                            if (rd.data[i+7+c] == '\n')
                            {
                                c = 128; //exit
                            }
                            else
                            {
                                rs.u[c] = rd.data[i+7+c];
                            }
                        }
                        if (rf->FaceList->size() != 0)
                        {
                            ob->MaterialFaces->push_back(*rf);
                            delete rf;
                            rf = new Faces;
                            rf->Clear();
                        }
                        rf->Material = rs.u;
                    }
                    //smoothing
                    if ((rd.data[i] == 's') && (rd.data[i+1] == ' '))
                    {
                        if (rd.data[i+2] == 'o')
                        {
                            rs.s_val = 0;
                        }
                        else
                        {
                            rs.s_val = atoi(&rd.data[i+1]);
                        }
                        if (rf->FaceList->size() != 0)
                        {
                            ob->MaterialFaces->push_back(*rf);
                            delete rf;
                            rf = new Faces;
                            rf->Init();
                        }
                        rf->Smoothing = rs.s_val;
                        rf->Material = rs.u;
                    }
                    //faces
                    if ((rd.data[i] == 'f') && (rd.data[i+1] == ' '))
                    {

                        Face* f;
                        int f_c = 0;
                        bool h_f = false;
                        unsigned long h_pt = 0;

                        int faces_this_line = 0;
                        for (unsigned long ft = ps.start; ft < ps.end; ft++)
                            if (rd.data[ft] == ' ')
                                faces_this_line++;

                        switch(faces_this_line)
                        {
                            case 1:
                            case 2:
                                break;
                            case 3: //triangle
                                f = new Face[3];
                                pt = 0;
                                f_c = 0;
                                for (unsigned long l = ps.start; l < ps.end; l++)
                                {
                                    if (rd.data[l] == ' ')
                                    {
                                        pt = l;
                                    }
                                    if (rd.data[l] == '/')
                                    {
                                        if (!h_f)
                                        {
                                            f[f_c].Vertex = atoi(&rd.data[pt+1]);
                                            h_pt = l;
                                            h_f = true;
                                        }
                                        else
                                        {
                                            f[f_c].Coordinate = atoi(&rd.data[h_pt+1]);
                                            f[f_c].Normal = atoi(&rd.data[l+1]);
                                            f_c++;
                                            h_pt = 0;
                                            h_f = false;
                                        }
                                    }
                                }
                                rf->FaceList->push_back(f[0]);
                                rf->FaceList->push_back(f[1]);
                                rf->FaceList->push_back(f[2]);
                                delete[] f;
                                f = NULL;
                                break;
                            case 4: //quad
                                f = new Face[4];
                                pt = 0;
                                f_c = 0;
                                for (unsigned long l = ps.start; l < ps.end; l++)
                                {
                                    if (rd.data[l] == ' ')
                                    {
                                        pt = l;
                                    }
                                    if (rd.data[l] == '/')
                                    {
                                        if (!h_f)
                                        {
                                            f[f_c].Vertex = atoi(&rd.data[pt+1]);
                                            h_pt = l;
                                            h_f = true;
                                        }
                                        else
                                        {
                                            f[f_c].Coordinate = atoi(&rd.data[h_pt+1]);
                                            f[f_c].Normal = atoi(&rd.data[l+1]);
                                            f_c++;
                                            h_pt = 0;
                                            h_f = false;
                                        }
                                    }
                                }
                                rf->FaceList->push_back(f[0]);
                                rf->FaceList->push_back(f[1]);
                                rf->FaceList->push_back(f[2]);
                                rf->FaceList->push_back(f[2]);
                                rf->FaceList->push_back(f[3]);
                                rf->FaceList->push_back(f[0]);
                                delete[] f;
                                f= NULL;
                                break;
                            default: //n sided polygon
                                f = new Face[faces_this_line];
                                pt = 0;
                                f_c = 0;
                                for (unsigned long l = ps.start; l < ps.end; l++)
                                {
                                    if (rd.data[l] == ' ')
                                    {
                                        pt = l;
                                    }
                                    if (rd.data[l] == '/')
                                    {
                                        if (!h_f)
                                        {
                                            f[f_c].Vertex = atoi(&rd.data[pt+1]);
                                            h_pt = l;
                                            h_f = true;
                                        }
                                        else
                                        {
                                            f[f_c].Coordinate = atoi(&rd.data[h_pt+1]);
                                            f[f_c].Normal = atoi(&rd.data[l+1]);
                                            f_c++;
                                            h_pt = 0;
                                            h_f = false;
                                        }
                                    }
                                }
                                for (int t = 2; t < f_c; t++)
                                {
                                    rf->FaceList->push_back(f[0]);
                                    rf->FaceList->push_back(f[t-1]);
                                    rf->FaceList->push_back(f[t]);
                                }
                                delete[] f;
                                f = NULL;
                                break;
                        }
                    }

                    if (DebugInit)
                    for (unsigned long i = ps.start; i <= n; i++)
                        if (DebugData) std::cout << rd.data[i];

                }
            }

            ob->MaterialFaces->push_back(*rf);
            Object->MaterialObjects->push_back(*ob);
        }

        if (DebugRead)
        {
            std::cout << std::endl;
            std::cout << "Object vertex size:     ";
            std::cout.flags(std::ios::right) << std::cout.width(9);
            std::cout << num_sep(Object->MaterialData->Vertex->size()) << std::endl;
            std::cout << "Object coordinate size: ";
            std::cout.flags(std::ios::right) << std::cout.width(9);
            std::cout << num_sep(Object->MaterialData->Coordinate->size()) << std::endl;
            std::cout << "Object normal size:     ";
            std::cout.flags(std::ios::right) << std::cout.width(9);
            std::cout << num_sep(Object->MaterialData->Normal->size()) << std::endl;
            std::cout << "Object size:            ";
            std::cout.flags(std::ios::right) << std::cout.width(9);
            std::cout << num_sep(Object->MaterialObjects->size()) << std::endl;
        }

        munmap(rd.data, rd.file_sz);

    }

    clock_t end = clock();


    if (DebugInit)
    {
        float n_avg = 0;
        if (rs.rso.fp)
            n_avg = (float) (rs.rso.f - rs.rso.ft*3 - rs.rso.fq*4) / (float) rs.rso.fp;
        float n_t = (float) (rs.rso.f - rs.rso.fq*4 - rs.rso.fp*n_avg) / (float) rs.rso.f * 100;
        float n_q = (float) (rs.rso.f - rs.rso.ft*3 - rs.rso.fp*n_avg) / (float) rs.rso.f * 100;
        float n_p = (float) (rs.rso.f - rs.rso.ft*3 - rs.rso.fq*4) / (float) rs.rso.f * 100;

        std::cout << std::endl;
        std::cout << "o: " << num_sep(rs.o) << std::endl;
        int nu = 0;
        for (std::list<ReadObject>::iterator r = l_ro.begin(); r != l_ro.end(); r++)
        {
            std::cout.flags(std::ios::right) << std::cout.width(2);
            std::cout << ++nu;
            std::cout.flags(std::ios::right) << std::cout.width(12);
            std::cout << num_sep(r->start);
            std::cout.flags(std::ios::right) << std::cout.width(12);
            std::cout << num_sep(r->end);
            std::cout << std::endl;
        }

        std::cout << std::endl;
        std::cout << "v:  ";
        std::cout.flags(std::ios::right) << std::cout.width(11);
        std::cout << num_sep(rs.rso.v) << std::endl;
        std::cout << "vt: ";
        std::cout.flags(std::ios::right) << std::cout.width(11);
        std::cout << num_sep(rs.rso.vt) << std::endl;
        std::cout << "vn: ";
        std::cout.flags(std::ios::right) << std::cout.width(11);
        std::cout << num_sep(rs.rso.vn) << std::endl;
        std::cout << "s:  ";
        std::cout.flags(std::ios::right) << std::cout.width(11);
        std::cout << num_sep(rs.rso.s) << std::endl;

        std::cout << std::endl;
        std::cout << "f:  ";
        std::cout.flags(std::ios::right) << std::cout.width(28);
        std::cout << num_sep(rs.rso.f) << std::endl;

        std::cout << "ft: ";
        std::cout.flags(std::ios::right) << std::cout.width(11);
        std::cout << num_sep(rs.rso.ft);
        std::cout.flags(std::ios::right | std::ios::fixed) << std::cout.width(6);
        std::cout << "3";
        std::cout.flags(std::ios::right) << std::cout.width(11);
        std::cout << num_sep(rs.rso.ft*3);
        std::cout.flags(std::ios::right | std::ios::fixed) << std::cout.width(6);
        std::cout.precision(1);
        std::cout << n_t << "%";
        std::cout << std::endl;

        std::cout << "fq: ";
        std::cout.flags(std::ios::right) << std::cout.width(11);
        std::cout << num_sep(rs.rso.fq);
        std::cout.flags(std::ios::right | std::ios::fixed) << std::cout.width(6);
        std::cout << "4";
        std::cout.flags(std::ios::right) << std::cout.width(11);
        std::cout << num_sep(rs.rso.fq*4);
        std::cout.flags(std::ios::right | std::ios::fixed) << std::cout.width(6);
        std::cout.precision(1);
        std::cout << n_q << "%";
        std::cout << std::endl;

        std::cout << "fp: ";
        std::cout.flags(std::ios::right) << std::cout.width(11);
        std::cout << num_sep(rs.rso.fp);
        std::cout.flags(std::ios::right | std::ios::fixed) << std::cout.width(6);
        std::cout.precision(1);
        std::cout << n_avg;
        std::cout.flags(std::ios::right) << std::cout.width(11);
        std::cout << num_sep(rs.rso.fp*n_avg);
        std::cout.flags(std::ios::right | std::ios::fixed) << std::cout.width(6);
        std::cout.precision(1);
        std::cout << n_p << "%";
        std::cout << std::endl;

        std::cout << std::endl <<  "elapsed: ";
        std::cout.setf(std::ios::fixed, std::ios::floatfield) << std::cout.precision(3);
        std::cout << ((double) (end - start)/ (double) CLOCKS_PER_SEC) << std::endl;

    }

    return Object;

}

int RenderReader::ReadImage(ImageTextureStruct* Image, char* file)
{

    std::cout << "Reading texture image from Portable Network Graphic ... " << std::endl;
    
    //declare a local information structure
    ImageInfo PNG;
    
    std::string resource(file);
    resource.insert(0,this->Status->Directory);
    file = (char*) resource.c_str();
    
    FILE *fp = fopen(file, "r");
    if (!fp)
    {
        fprintf(stderr, "\nCould not open PNG Image %s!\n", file);
        return 1;
    };

    if (DebugRead) std::cout << "ReadTexture: " << file << std::endl;

    //read the header
    fread(PNG.Header, 1, 8, fp);

    //PNG test
    if (png_sig_cmp(PNG.Header, 0, 8))
    {
        fclose(fp);
        fprintf(stderr, "\n%s is not a valid PNG image!\n", file);
    }

    //create PNG info structure and pointer
    PNG.Pointer = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!PNG.Pointer)
    {
        fclose(fp);
        fprintf(stderr, "\nFailed to create read structure!\n", NULL);
    }

    //create PNG info struct
    PNG.Info = png_create_info_struct(PNG.Pointer);
    if (!PNG.Info)
    {
        png_destroy_read_struct(&PNG.Pointer, (png_infopp) NULL, (png_infopp) NULL);
        fclose(fp);
        fprintf(stderr, "\nFailed to create info structure!\n", NULL);
    }

    //create PNG end info struct
    PNG.EndInfo = png_create_info_struct(PNG.Pointer);
    if (!PNG.EndInfo)
    {
        png_destroy_read_struct(&PNG.Pointer, &PNG.Info, (png_infopp) NULL);
        fclose(fp);
        fprintf(stderr, "\nFailed to create end info structure!\n", NULL);
    }

    //PNG error
    if (setjmp(png_jmpbuf(PNG.Pointer)))
    {
        png_destroy_read_struct(&PNG.Pointer, &PNG.Info, &PNG.EndInfo);
        fclose(fp);
        fprintf(stderr, "\nFailed png_jmpbuf!\n", NULL);
    }

    //initialize PNG reading
    png_init_io(PNG.Pointer, fp);

    //update 8 byte header read
    png_set_sig_bytes(PNG.Pointer, 8);

    //read all info up to the image data
    png_read_info(PNG.Pointer, PNG.Info);

    //get PNG IHDR
    png_get_IHDR(PNG.Pointer, PNG.Info, (png_uint_32*)&Image->Width,
            (png_uint_32*)&Image->Height, &PNG.BitDepth, &PNG.ColorType, NULL, NULL, NULL);

    if (DebugRead) std::cout << "Bit Depth:  " << PNG.BitDepth << std::endl;

    switch(PNG.BitDepth)
    {
        case 8:
            Image->TYPE = GL_UNSIGNED_BYTE;
            break;
        default:
            fprintf(stderr, "\nUnsupported PNG bit depth!\n", NULL);
            break;
    }

    switch(PNG.ColorType)
    {
        case PNG_COLOR_TYPE_RGB:
            Image->INTERNALFORMAT = GL_RGB;
            Image->FORMAT = GL_RGB;
            if (DebugRead) std::cout << "Color Type: GL_RGB" << std::endl;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            Image->INTERNALFORMAT = GL_RGBA;
            Image->FORMAT = GL_RGBA;
            if (DebugRead) std::cout << "Color Type: GL_RGBA" << std::endl;
            break;
        default:
            fprintf(stderr, "\nUnsupported PNG color type!\n", NULL);
            break;
    }

    //update the PNG info struct
    png_read_update_info(PNG.Pointer, PNG.Info);

    //get row size in bytes
    PNG.BytesPerRow = png_get_rowbytes(PNG.Pointer, PNG.Info);
    Image->SIZE = PNG.BytesPerRow * Image->Height;

    //allocate the image as a block, to be given to opengl
    Image->Data = new png_byte[Image->SIZE];
    if (Image->Data == NULL)
    {
        png_destroy_read_struct(&PNG.Pointer, &PNG.Info, &PNG.EndInfo);
        fclose(fp);
        fprintf(stderr, "\nFailed to allocate image data!\n", NULL);
    }

    //allocate the row pointers to create the PNG with libpng
    PNG.RowPointers = new png_bytep[Image->Height];
    if (PNG.RowPointers == NULL)
    {
        png_destroy_read_struct(&PNG.Pointer, &PNG.Info, &PNG.EndInfo);
        delete[] (png_bytep)Image->Data;
        fclose(fp);
        fprintf(stderr, "\nFailed to allocate row pointers!\n", NULL);
    }

    //set the individual row pointers to the location in memory of Image.Data
    for (unsigned int i = 0; i < Image->Height; i++)
        PNG.RowPointers[Image->Height - 1 - i] =
                (png_bytep) Image->Data + i * PNG.BytesPerRow;


    //read PNG image and pass data through pointers to Image.Data
    png_read_image(PNG.Pointer, PNG.RowPointers);


    //clean up memory and close
    png_destroy_read_struct(&PNG.Pointer, &PNG.Info, &PNG.EndInfo);
    delete[] PNG.RowPointers;
    fclose(fp);

    return 0;

}

void RenderReader::DebugMessage(const char* Message, const GLchar* Log)
{

    fprintf(stderr, Message, Log);

}
