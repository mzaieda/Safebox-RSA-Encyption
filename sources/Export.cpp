#include <Export.h>
#include <FileSpooler.h>
#include <random>
#include <string>
#include <sstream>


SafeBoxExport::SafeBoxExport (){

}

void SafeBoxImport::process (char * input_file,
                            char * output_file,
                            char * working_dir,
                            char * key_file,
                            uint16_t block_size,
                            uint16_t shreds){
        printf ("Import file\n");
        char outf [1024];
        char outiv [1024];
        char outq [1024];
        memset (outf,0,1024);
        memset (outiv,0,1024);
        memset (outq,0,1024);
        sprintf (outf,"%s/%s",working_dir,output_file);
        sprintf (outiv,"%s/%s.iv",working_dir,output_file);
        sprintf (outq,"%s/%s.queue",working_dir,output_file);

        MultithreadedShredManager shredManager(outf, block_size, shreds,true);       
        FileSpooler fileSpooler (input_file,block_size,false);
        shredManager.encrypt(&fileSpooler,key_file,outiv,outq); 
                             
            FileSpooler fileSpooler (input_file,block_size,false);
            node_t n;
            n.filename = input_file;
            n.blocksize = block_size; 
            n.blocks = fileSpooler.getBlockCount();
            n.bytes = fileSpooler.fsize;
            n.shreds = shreds; 
            n.hashed_name=rand_str;
            n.flag = false;
            avl-> insert(n);

                // The same as import but just decrypting the blocks. 
                // The more details will include in the other functions, but generally, the safe box is just calling the constructor 
              //  and its preporities  : decrypted function.
}
SafeBox * SafeBoxExport::clone (){
    return new SafeBoxExport();
}
SafeBoxExport::~SafeBoxExport(){
}
