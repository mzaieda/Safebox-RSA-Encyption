#include <ShredManager.h>
#include <ThreadManager.h>
#include <MultiHeadQueue.h>
ShredManager::ShredManager () 
{

}

ShredManager::ShredManager(char * p_file_name, uint16_t p_block_size, uint16_t p_shred_count,bool truncate)
{
    shred_count = p_shred_count;
    shreds = (Shred ** ) calloc (shred_count,sizeof(Shred*));
    for ( char i = 0 ; i  < shred_count; i++)
    {
        string fname = p_file_name;
        fname.insert(fname.find('.'),1,i+'A');
        cout << fname << endl;
        if (truncate)
           shreds[i] = new Shred(fname.c_str(),p_block_size,truncate);
        else shreds[i] = new Shred(fname.c_str(),(p_block_size+16)&~15,truncate);
    }
}
bool ShredManager::encrypt (FileSpooler * fileSpooler, const char * key_file_name, const char * iv_file_name)
{
    AutoSeededRandomPool rnd;  //declaring an object of type AutoSeededRandomPool to generate iv
    CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];  //declaring the iv array
    memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);  //resetting the iv 

    rnd.GenerateBlock(iv, AES::BLOCKSIZE);//applying the method to generate the iv of predefined size = BLOCKSIZE
    CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];  //declaring the key array 
    memset(key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH);   //resetting the key

    ofstream fp; //opening a stream to store the iv 
    fp.open(iv_file_name, ios::out);   //opening the file to write the iv to it
    if(fp.is_open()){   //check if the files was opened correctly or not
        fp.write(reinterpret_cast<char*>(iv), sizeof(iv));   //writing to the file
        fp.close();    //closing the file after using it 
    }
    ifstream f;  //opening a stream to read the key 
    f.open(key_file_name, ios::in);  //opening the file to read the key to the buffer
    if(f.is_open()){  //checking if opened   
        f.read(reinterpret_cast<char*>(key), sizeof(key));   //reading the data in key_file and storing it in the buffer key
        f.close();   //closing the stream file
    }
    
    Block *b = fileSpooler->getNextBlock();   //instantiating an object of type block 
  
   
    int i = 0;  //initialize i = 0 
    do{      //starting an infinite loop
        b->encrypt(key, iv);  //encrypting using the key and the iv
        *(shreds[i%shred_count]) << *b;    //storing the data of the block into the shreds in round robin
        delete(b);   //delete the object b
        b = fileSpooler->getNextBlock();    //equating b to the filespooler getting the next block
        i++;
    }while(b!= NULL);  //delimeter of infinite loop
    return true;
}
bool ShredManager::decrypt (FileSpooler * fileSpooler, const char * key_file_name, const char * iv_file_name)
{
    AutoSeededRandomPool prng;
    CryptoPP::byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );

    ifstream f;
    f.open(key_file_name,ios::in);      //to read the key 
    if (f.is_open())
    {
        f.read (reinterpret_cast<char*>(key),sizeof(key));
        f.close();
    }

    f.open(iv_file_name,ios::in);
    if (f.is_open())
    {
        f.read (reinterpret_cast<char*>(iv),sizeof(iv));
        f.close();
    }

    Block * block = NULL;
    for (int i = 0 ; i == 0 || block != NULL; i ++)
    {
        block = shreds[i%shred_count]->getNextBlock();
        if ( block == NULL) break;
        block->decrypt(key,iv);
        //block->print();
        fileSpooler->appendBlock(block);
        delete (block);
    }
    return false;
}
ShredManager::~ShredManager()
{
    for ( int i = 0 ; i  < shred_count; i++)
        delete(shreds[i]);
    free(shreds);
}
//*************

MultithreadedShredManager::MultithreadedShredManager(char * p_file_name, uint16_t p_block_size, uint16_t p_shred_count,bool p_truncate) : ShredManager()
{
    file_name = p_file_name;
    shred_count = p_shred_count;
    block_size = p_block_size;
    truncate = p_truncate;
    shreds = (Shred ** ) calloc (shred_count,sizeof(Shred*));
}
bool MultithreadedShredManager::encrypt (FileSpooler * p_fileSpooler, char * key_file_name,  char * iv_file_name, char * q_file_name)
{
    CryptoPP::byte  iv[ CryptoPP::AES::BLOCKSIZE ];
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );
     AutoSeededRandomPool prng;
     prng.GenerateBlock(iv,sizeof(iv));
     
    
    ofstream kf;
    kf.open(iv_file_name,ios::out);
    if ( kf.is_open())
    {
        kf.write (reinterpret_cast<char*>(iv),sizeof(iv));
        kf.close();
    }
    //This function needed to be implemented by you
//------------------------------------------------------------//
    ThreadManager threadmanger ;
    Lottery p_lottery(p_fileSpooler->getBlockCount());
    MultiHeadQueue  <sb_block_index_t> p_multiHeadQueue;
    int i=0;

    string ivname = iv_file_name;
    cout << ivname << endl;
    do{
        string fname = file_name;
        fname.insert(fname.find('.'),1,i+'A');
                cout << fname << endl;
        if(truncate)
        shreds[i]=new EncryptShredThread (p_fileSpooler,  key_file_name, (char*)ivname.c_str() ,fname.c_str(), block_size,&p_lottery, &p_multiHeadQueue,i+'A', truncate) ;
        else
        shreds[i]=new EncryptShredThread (p_fileSpooler,  key_file_name, (char*)ivname.c_str() ,fname.c_str(), (block_size+16)&~15,&p_lottery, &p_multiHeadQueue,i+'A', truncate) ;
        threadmanger+=dynamic_cast<Thread*>(shreds[i]);
        i++;
    }while(i<shred_count);

    threadmanger.start();
    threadmanger.barrier();
    p_multiHeadQueue.dump(q_file_name,key_file_name,iv_file_name);
//------------------------------------------------------------//
    return true;
}

bool MultithreadedShredManager::decrypt (FileSpooler * p_fileSpooler, char * key_file_name,  char * iv_file_name, char * q_file_name)
{
    //This function needed to be implemented by you
//------------------------------------------------------------//
    ThreadManager threadmanger;
    MultiHeadQueue  <sb_block_index_t>  p_multiHeadQueue;
    
    string ivname = iv_file_name;
    cout << ivname << endl;
    p_multiHeadQueue.load(q_file_name,key_file_name,iv_file_name); //hadeer
    int i=0;
    do{
        string fname = file_name;
        fname.insert(fname.find('.'),1,i+'A');
                cout << fname << endl;
        if(truncate)
            shreds[i]=new DecryptShredThread (p_fileSpooler,  key_file_name, (char*)ivname.c_str() ,(char*)fname.c_str(), block_size, &p_multiHeadQueue,i+'A', truncate) ;
        else
            shreds[i]=new DecryptShredThread (p_fileSpooler,  key_file_name, (char*)ivname.c_str() ,(char*)fname.c_str(), (block_size+16)&~15, &p_multiHeadQueue,i+'A', truncate) ;
        
        threadmanger+=dynamic_cast<Thread*>(shreds[i]);
        i++;
    }while(i<shred_count);

    threadmanger.start();
    threadmanger.barrier();
   

    //------------------------------------------------------------//
        return true;
    }

MultithreadedShredManager::~MultithreadedShredManager()
{

}
