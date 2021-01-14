#include <List.h>
#include <FileSpooler.h>
#include <random>
#include <string>
#include <sstream>


SafeBoxList::SafeBoxList(){
}
        void SafeBoxList::process(char * input_file,
                                    char * output_file,
                                    char * working_dir,
                                    char * key_file,
                                    uint16_t block_size,
                                    uint16_t shreds,
                                      bool flag){
        avl->process([](AVLNode <node_t> * n)-> void{ 
        cout<<n->getItem().filename << "--" << n->getItem().blocksize << "--"<< n-> getItem().blocks << "--" << n-> getItem().bytes<< "--" << n->getItem().shreds << "--" << n->getItem().hashed_name << endl;

         },flag);
        
    }

SafeBox * SafeBoxList::clone (){
    return new SafeBoxList();
}
SafeBoxList::~SafeBoxList(){
}



