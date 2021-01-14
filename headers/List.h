#ifndef LIST_H
#define LIST_H
#include <defines.h>
#include <FileSpooler.h>
#include <ShredManager.h>
#include"AVLTree.hpp"

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
                              bool flag );
        virtual SafeBox * clone ();
        virtual~SafeBoxList(); 
};

#endif 