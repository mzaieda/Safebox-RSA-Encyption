#include <SafeBox.h>

SafeBox::SafeBox ()
{
    AVLTree avl = new AVLTree <node_t> ();

}
SafeBox::~SafeBox()
{

}

SafeBoxImport::SafeBoxImport (){

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

        AVLprocess("index.txt",key_file, "output/iv.txt"); 

                mt19937 generator{random_device{}()};
                uniform_int_distribution<int> distribution{'a', 'z'};
                auto generate_len = 5; 
                string rand_str(generate_len, '\0');
                for(auto& dis: rand_str)
                dis = distribution(generator); 
                             
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
            MultithreadedShredManager shredManager((char*)rand_str.c_str(), block_size, shreds,true);       
            shredManager.encrypt(&fileSpooler,key_file,"output/iv.txt",outq);  
            Index_process("index.txt",key_file, "output/iv.txt");
        
}
SafeBox * SafeBoxImport::clone (){
    return new SafeBoxImport();
}
SafeBoxImport::~SafeBoxImport(){

}

SafeBoxExport::SafeBoxExport (){

}
void SafeBoxExport::process (char * input_file,
                            char * output_file,
                            char * working_dir,
                            char * key_file,
                            uint16_t block_size,
                            uint16_t shreds){
        printf ("Export file\n");
        char inf [1024];
        char iniv [1024];
        char inq [1024];
        memset (inf,0,1024);
        memset (iniv,0,1024);
        memset (inq,0,1024);
        sprintf (inf,"%s/%s",working_dir,input_file);
        sprintf (iniv,"%s/%s.iv",working_dir,input_file);        
        sprintf (inq,"%s/%s.queue",working_dir,input_file);  

MultithreadedShredManager shredManager(inf, block_size, shreds,false);        
        FileSpooler fileSpooler (output_file,block_size,true);
        shredManager.decrypt(&fileSpooler,key_file,iniv,inq);
        
}
SafeBox * SafeBoxExport::clone (){
    return new SafeBoxExport();
}
SafeBoxExport::~SafeBoxExport(){
}

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
    n.filename = input_file; 
    avl->find(n)-> getItem().flag= false;  
}

SafeBox * SafeBoxDelete::clone(){
    return new SafeBoxDelete();
}
SafeBoxDelete::~SafeBoxDelete() 
{}


