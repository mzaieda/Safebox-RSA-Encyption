#include <Import.h>
#include <FileSpooler.h>
#include <random>
#include <string>
#include <sstream>


SafeBoxImport::SafeBoxImport(){

}
void SafeBoxImport::process (char * input_file,
                            char * output_file,
                            char * working_dir,
                            char * key_file,
                            uint16_t block_size,
                            uint16_t shreds,
                             bool flag){
                                
                    printf ("Import file\n");
                    char outf [1024];
                    char outiv [1024];
                    char outq [1024];
                    memset (outf,0,1024);
                    memset (outq,0,1024);

                    sprintf (outf,"%s/%s",working_dir,output_file);
                    sprintf (outiv,"%s/%s.iv",working_dir,output_file);
                    sprintf (outq,"%s/%s.queue",working_dir,output_file); 
                    
                    
                             tree_process("treeindex.txt",key_file, "output/iv.txt");       //sending to the tree process the following parameter
                        cout << " A"<< endl; 

                    mt19937 generator{random_device{}()};       //random generator to generate the random string for the hidden name
                  uniform_int_distribution<int> distribution{'a', 'z'};     //distribute the selection 
                     auto generate_len = 5; 
                 std::string rand_str(generate_len, '\0');       //generate it here
              for(auto& dis: rand_str)      
               dis = distribution(generator);       //store it in this buffer
                               //     sprintf ((char*)rand_str.c_str(),"%s/%s",working_dir,output_file); 

 /*  char rand_str[8];
    for (int i = 0; i < 8; ++i)
        rand_str[i] = ((double)rand() / ((double)RAND_MAX + 1.0)) * ('z' - 'a' + 1) + 'a';
    rand_str[8] = '\0';*/
    // Import will get the strct of AV_NODE and insert it in the tree
            FileSpooler fileSpooler (input_file,block_size,false);
            AVL_Node_index_t strct;
            strct.filename= input_file;
            strct.blocksize=block_size; 
            strct.size_in_blocks= fileSpooler.getBlockCount();
            strct.size_in_bytes = fileSpooler.fsize;
            strct.shreds = shreds; 
            strct.hidden_name=rand_str;
            strct.flag=false;
            avl-> insert(strct);
            MultithreadedShredManager shredManager((char*)rand_str.c_str(), block_size, shreds,true);       
            shredManager.encrypt(&fileSpooler,key_file,"output/iv.txt",outq);  // To get one iv.
            Index_process("treeindex.txt",key_file, "output/iv.txt");       //sending the following parqamter to the index process

}
SafeBox * SafeBoxImport::clone (){
    return new SafeBoxImport();
}
SafeBoxImport::~SafeBoxImport(){

}
