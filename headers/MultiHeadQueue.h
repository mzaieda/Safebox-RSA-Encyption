#ifndef MULTIHEADQUEUE_H
#define MULTIHEADQUEUE_H

#include <includes.h>
#include <defines.h>
#include <vector>
#include <thread>
#include <Shred.h>
#include <stdio.h>


using namespace std;

template <typename T>
class MultiHeadQueue: private std::vector<T> {
    
private:

    mutex mtx;
    uint32_t read_size;
    char * buffer;

public:

MultiHeadQueue ():vector<T>(){}

void enqueue(T & t)
{
    mtx.lock();
    vector<T>::push_back(t);
    mtx.unlock();
}

bool dequeue(T & t,std::function <bool (T&,T&)> check)
{
    mtx.lock();

    T e;

    for (auto x = vector<T>::begin() ;x != vector<T>::end();x++)
    {

        if(check(x[0],t))//x[0] is the block index , t is the cur block index
        {
                t=x[0];// give the wanted block index to cur block_index

            vector<T>::erase(x);//erase the block_index
                mtx.unlock();
            return true;
        }
    }

    mtx.unlock();
    
    return false;
}

void dump (char * filename,char * p_key_file,char * p_iv_file)

{

    mtx.lock();    

    CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];  //declaring the iv array
    CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];  //declaring the key array 

    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );

    ifstream fk; //opening a stream to read the key from it a store it in a variable 

    fk.open(p_key_file,ios::in); //opening the key file 

    if ( fk.is_open()) //check if the key file was opened successfully or not 
    {
       fk.read (reinterpret_cast<char*>(key),sizeof(key));  //storing the content of the key file into a variable 
       fk.close(); //closing the key file 
    }

    fk.open(p_iv_file,ios::in); //opening the iv file 
    if ( fk.is_open()) //checks if the file was opened or not 
    {
       fk.read (reinterpret_cast<char*>(iv),sizeof(iv)); //storing the content of the iv file into a variable
       fk.close(); //closing the file 
    }
 
            read_size =  vector<T>::size()*sizeof(T);
            std::string ciphertext;
            CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
            CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );
            CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( ciphertext ) );
            stfEncryptor.Put( reinterpret_cast<const unsigned char*>(  vector<T>::data() ), read_size );
            stfEncryptor.MessageEnd();
            memcpy ( vector<T>::data() ,ciphertext.c_str(),ciphertext.size());

            ofstream fq; 
            fq.open(filename, ios::out | ios::trunc);   //opening the file to write the iv to it
            if(fq.is_open()){   //check if the files was opened correctly or not
                fq.write(reinterpret_cast<char*>(vector<T>::data()),read_size); //overwrites the content of the queue file to the encrypted queue 
                fq.close();  //closing the file 
            }

}

        mtx.unlock();



void load (char * filename,char * p_key_file,char * p_iv_file)
{
    mtx.lock();

    CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];  //declaring the iv array
    CryptoPP::byte key[CryptoPP::AES::DEFAULT_KEYLENGTH];  //declaring the key array 

    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );

    ifstream fk; //opening a stream to read the key from it a store it in a variable 
  
    fk.open(p_key_file,ios::in); //opening the key file 
    if ( fk.is_open()) //check if the key file was opened successfully or not 
    {
       fk.read (reinterpret_cast<char*>(key),sizeof(key));  //storing the content of the key file into a variable 
       fk.close(); //closing the key file 
    }

    fk.open(p_iv_file,ios::in); //opening the iv file 
    if ( fk.is_open()) //checks if the file was opened or not 
    {
       fk.read (reinterpret_cast<char*>(iv),sizeof(iv)); //storing the content of the iv file into a variable
       fk.close(); //closing the file 
    }
    
    ifstream f; //opening a stream 
    f.open(filename,ios::in); //openeing the encrypted queue file 
    if ( f.is_open())  //checks if opened or not 
    { 
        f.seekg(0,f.end); 
        long sz = f.tellg();
        f.seekg(0,f.beg);
        T * buffer = (T*) calloc(sz/sizeof(T),sizeof(T));
        f.read (reinterpret_cast<char*>(buffer),sz);
        f.close();
        sz/=sizeof(T);
        
    read_size =  vector<T>::size()*sizeof(T);
    std::string decryptedtext;
    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv);
    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedtext ), CryptoPP::StreamTransformationFilter::ZEROS_PADDING);
    stfDecryptor.Put(reinterpret_cast<const unsigned char*>(buffer), sz );
    stfDecryptor.MessageEnd();
    read_size = (uint16_t) decryptedtext.size();
    memcpy (buffer,decryptedtext.c_str(),sz);

    for ( int i = 0 ; i < sz ; i++) 
            vector<T>::push_back(buffer[i]);
            free (buffer);
    }

        mtx.unlock();

}

     


    ~MultiHeadQueue(){}


};



#endif