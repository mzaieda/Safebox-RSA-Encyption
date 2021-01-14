#include <FileSpooler.h>

Block * FileSpooler::getBlockLockFree()
{
    Block * b = new Block (block_size);
    if (b->load(f)) return b;
    else {
        delete (b);
        return NULL;
    }
}


FileSpooler::FileSpooler(const char * p_file_name, uint16_t p_block_size, bool truncate)
{
    fsize = 0;
    if ( truncate )
        f.open(p_file_name,ios::in|ios::out|ios::trunc);
    else f.open(p_file_name,ios::in|ios::out);
    if ( f.is_open())
    {
        long cur = f.tellg ();
        f.seekg(0,f.end);
        fsize = f.tellg ();
        f.seekg(cur,f.beg);
    }
    block_size = p_block_size;
}
 Block * FileSpooler::operator [] (long index)
 {
    mtx.lock();
    if ( index*block_size >= fsize) 
    {
        mtx.unlock();
        return NULL;
    }
    long cur = f.tellg ();
    f.seekg(index*block_size,f.beg);
    Block * b  = getBlockLockFree();
    f.seekg(cur,f.beg);
    mtx.unlock();
    return b;
 }
long FileSpooler::getBlockCount () 
{
 //This function needed to be implemented by you
//---------------------------------------------------------------------------------------/
long block_count=fsize/block_size; 
if(fsize%block_size!=0) //hadeer
block_count++;
 
return block_count;
//---------------------------------------------------------------------------------------/
}


Block * FileSpooler::getNextBlock()
{
    mtx.lock(); 
    Block * b  = getBlockLockFree();
    mtx.unlock ();
    return b;
}
void FileSpooler::appendBlock (Block * b)
{
    mtx.lock();
    b->store(f);
    mtx.unlock();
}
void FileSpooler::writeBlockAt(Block *b,long block_index) //write and store
{

    //This function needed to be implemented by you
//-----------------------------------------------------------------------------------/
 mtx.lock(); 
f.seekp(block_size*block_index);
b->store(f);
 mtx.unlock();

//-----------------------------------------------------------------------------------/

}
FileSpooler::~FileSpooler()
{
    if ( f.is_open()) f.close();
}
