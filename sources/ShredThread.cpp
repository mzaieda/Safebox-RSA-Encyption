#include <ShredThread.h>

ShredThread::ShredThread (FileSpooler * p_fileSpooler,  char * p_key_file_name,  char * p_iv_file_name, const char * p_file_name,uint16_t block_size,MultiHeadQueue  <sb_block_index_t> * p_multiHeadQueue,char p_shred_name,bool truncate):Thread(), Shred(p_file_name,block_size,truncate)
{
        srcFileSpooler = p_fileSpooler;
        key_file_name = p_key_file_name;
        iv_file_name = p_iv_file_name;
        multiHeadQueue = p_multiHeadQueue;
        shred_name = p_shred_name;
}
void ShredThread::mainThreadBody(){}
ShredThread::~ShredThread(){}

EncryptShredThread::EncryptShredThread (FileSpooler * p_fileSpooler,  char * p_key_file_name,  char * p_iv_file_name,const char * p_file_name,uint16_t block_size,Lottery * p_lottery,MultiHeadQueue  <sb_block_index_t> * p_multiHeadQueue,char p_shred_name,bool truncate) : ShredThread(p_fileSpooler, p_key_file_name, p_iv_file_name,p_file_name,block_size,p_multiHeadQueue,p_shred_name,truncate)
{
    lottery = p_lottery;
}
void EncryptShredThread::mainThreadBody()
{
    AutoSeededRandomPool prng;
    CryptoPP::byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );
    
    ifstream kf;
    kf.open(key_file_name,ios::in);
    if ( kf.is_open())
    {
        kf.read (reinterpret_cast<char*>(key),sizeof(key));
        kf.close();
    }

    kf.open(iv_file_name,ios::in);
    if ( kf.is_open())
    {
        kf.read (reinterpret_cast<char*>(iv),sizeof(iv));
        kf.close();
    }

    int i=0;

    sb_block_index_t BlockIndex;
    BlockIndex.shred=shred_name;
    long index=lottery->withdraw();

    do{
        BlockIndex.block=index;

        Block *b=(*srcFileSpooler)[index];
        b->encrypt(key,iv);

        *(this)<<*b;

        BlockIndex.shred_block=i;
        multiHeadQueue->enqueue(BlockIndex);
        i++;
        delete(b);
        index=lottery->withdraw();
    }while(index != -1);
   
}
EncryptShredThread::~EncryptShredThread()
{

}

DecryptShredThread::DecryptShredThread (FileSpooler * p_fileSpooler,  char * p_key_file_name,  char * p_iv_file_name,const char * p_file_name,uint16_t block_size,MultiHeadQueue  <sb_block_index_t> * p_multiHeadQueue,char p_shred_name,bool truncate): ShredThread(p_fileSpooler, p_key_file_name, p_iv_file_name,p_file_name,block_size,p_multiHeadQueue,p_shred_name,truncate)
{
    

}
Block * DecryptShredThread::operator [] (int index)
{
    return (*fileSpooler)[index];
}

void DecryptShredThread::mainThreadBody()
{
    AutoSeededRandomPool prng;
    CryptoPP::byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );
    ifstream f;
    f.open(key_file_name,ios::in);
    if ( f.is_open())
    {
        f.read (reinterpret_cast<char*>(key),sizeof(key));
        f.close();
    }
    
    f.open(iv_file_name.c_str(),ios::in);
    if ( f.is_open())
    {
        f.read (reinterpret_cast<char*>(iv),sizeof(iv));
        f.close();
    }


sb_block_index_t BlockIndex;
BlockIndex.shred=shred_name;

bool check = multiHeadQueue->dequeue(BlockIndex,[](sb_block_index_t &a,sb_block_index_t &b)->bool{
    if(a.shred == b.shred) return true;
    else return false;
});

int i=0;

do{
    long index = BlockIndex.shred_block;
cout<<index<<endl;
    Block *b=(*this)[index];
    //b->print();
    b->decrypt(key,iv);
    srcFileSpooler->writeBlockAt(b,BlockIndex.block);
    check = multiHeadQueue->dequeue(BlockIndex,[](sb_block_index_t &a,sb_block_index_t &b)->bool{
        if(a.shred == b.shred) return true;
        else return false;
    });
    i++;
    delete(b);
    }while(check==true);

}

DecryptShredThread::~DecryptShredThread()
{
    
}
