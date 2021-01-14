#ifndef SAFEBOX_H
#define SAFEBOX_H

#include <includes.h>
#include <defines.h>
#include <FileSpooler.h>
#include <ShredManager.h>
#include "AVLTree.hpp"
#include <random>
#include <string>
#include <sstream>

class SafeBox{
    private:
    public:
        SafeBox ();
        virtual void process (char * input_file,
                            char * output_file,
                            char * working_dir,
                            char * key_file,
                            uint16_t block_size,
                            uint16_t shreds)=0;
        virtual SafeBox * clone ()=0; 
        virtual ~SafeBox();
};

class SafeBoxImport: public SafeBox{
    private:
    public:
        SafeBoxImport ();
        virtual void process (char * input_file,
                            char * output_file,
                            char * working_dir,
                            char * key_file,
                            uint16_t block_size,
                            uint16_t shreds);
        virtual SafeBox * clone ();
        virtual ~SafeBoxImport();
};

class SafeBoxExport: public SafeBox{
    private:
    public:
        SafeBoxExport ();
        virtual void process (char * input_file,
                            char * output_file,
                            char * working_dir,
                            char * key_file,
                            uint16_t block_size,
                            uint16_t shreds);
        virtual SafeBox * clone ();
        virtual ~SafeBoxExport();
};

class SafeBoxList: public SafeBox {
    private: 
    public: 
        SafeBoxList(); 
        virtual void process (char * input_file,
                            char * output_file,
                            char * working_dir,
                            char * key_file,
                            uint16_t block_size,
                            uint16_t shreds,
                              bool flag);
        virtual SafeBox * clone ();
        virtual~SafeBoxList(); 
};

class SafeBoxDelete: public SafeBox{
    private:
    public:        
        SafeBoxDelete(); 
        virtual void process(char * input_file,
                            char * output_file,
                            char * working_dir,
                            char * key_file,
                            uint16_t block_size,
                            uint16_t shreds,
                              bool flag);
        virtual SafeBox * clone ();
        virtual~SafeBoxDelete(); 
};


#endif