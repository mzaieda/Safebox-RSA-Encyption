#ifndef EXPORT_H
#define EXPORT_H

#include <defines.h>
#include <FileSpooler.h>
#include <ShredManager.h>
#include"AVLTree.hpp"


class SafeBoxExport: public SafeBox{
    private:
    public:
        SafeBoxExport ();
        virtual void process (char * input_file,
                            char * output_file,
                            char * working_dir,
                            char * key_file,
                            uint16_t block_size,
                            uint16_t shreds,
                              bool flag);
        virtual SafeBox * clone ();
        virtual ~SafeBoxExport();
};


#endif 

