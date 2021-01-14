#include <Delete.h>
#include<FileSpooler.h>
#include <random>
#include<string>
#include<sstream>

SafeBoxDelete::SafeBoxDelete() 
{
}
void SafeBoxDelete::process(char * input_file,
                            char * output_file,
                            char * working_dir,
                            char * key_file,
                            uint16_t block_size,
                            uint16_t shreds,
                              bool flag){
		node_t n;  
		n.filename=input_file;    //calling the filename and inserting the input_file
		avl->find(n)->getItem().flag= false; 
}
SafeBox * SafeBoxDelete::clone(){   //clone 
    return new SafeBoxDelete();
}
SafeBoxDelete::~SafeBoxDelete() 
{}
