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
    char * buffer = (char *) calloc (vector<T>::size()*sizeof(T)+1+(vector<T>::size()*sizeof(T)-((vector<T>::size()*sizeof(T))%CryptoPP::AES::BLOCKSIZE)),sizeof(char));

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

    ofstream f; //opens a stream to write to a file 

    f.open(filename,ios::out| ios::trunc);  //opens the queue file to store in it 

    if ( f.is_open())  //check if the file was opened correctly or not 
    {
        f.write(reinterpret_cast<char*>(vector<T>::data()),vector<T>::size()*sizeof(T)); 
        //writes to the file the content of the vector which contains the queue
		
		f.close(); //closing the file after writing to it
    }

    mtx.unlock(); 


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
            buffer = (char *) calloc (vector<T>::size()*sizeof(T)+1+(vector<T>::size()*sizeof(T)-((vector<T>::size()*sizeof(T))%CryptoPP::AES::BLOCKSIZE)),sizeof(char));  
            std::string ciphertext;
            CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
            CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );
            CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink( ciphertext ) );
            stfEncryptor.Put( reinterpret_cast<const unsigned char*>( buffer ), read_size );
            stfEncryptor.MessageEnd();
            memcpy (buffer,ciphertext.c_str(),ciphertext.size());
            read_size = ciphertext.size();

            ofstream fq; 
            fq.open(filename, ios::out | ios::trunc);   //opening the file to write the iv to it
            if(fq.is_open()){   //check if the files was opened correctly or not
                fq.write(buffer,read_size); //overwrites the content of the queue file to the encrypted queue 
                fq.close();  //closing the file 
            }
    
}



void load (char * filename,char * p_key_file,char * p_iv_file)
{
    
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
 
    //read_size =  vector<T>::size()*sizeof(T);
    //buffer = (char *) calloc (vector<T>::size()*sizeof(T)+1+(vector<T>::size()*sizeof(T)-((vector<T>::size()*sizeof(T))%CryptoPP::AES::BLOCKSIZE)),sizeof(char));
    std::string decryptedtext;
    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv);
    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedtext ));//, CryptoPP::StreamTransformationFilter::ZEROS_PADDING);
    //stfDecryptor.Put( reinterpret_cast<const unsigned char*>(reinterpret_cast<char*>(vector<T>::data())), vector<T>::size()*sizeof(T));
    stfDecryptor.Put( reinterpret_cast<const unsigned char*>( buffer ), read_size );
    stfDecryptor.MessageEnd();
    read_size = (uint16_t) decryptedtext.size();
    memcpy (buffer,decryptedtext.c_str(),read_size);

    ofstream fq; 
     fq.open(filename, ios::out | ios::trunc);   //opening the file to write the iv to it

      if(fq.is_open()){   //check if the files was opened correctly or not
        fq.write(buffer,read_size); //overwrites the content of the queue file to the encrypted queue 
        fq.close();  //closing the file 
      }

    mtx.lock();
    
    ifstream f; //opening a stream 
    f.open(filename,ios::in); //openeing the encrypted queue file 
    if ( f.is_open())  //checks if opened or not 
    { 
        //reading the queue to load it 
        f.seekg(0,f.end); 
        long sz = f.tellg();
        f.seekg(0,f.beg);
        T * bufer = (T *) calloc(sz/sizeof(T),sizeof(T));
        f.read (reinterpret_cast<char*>(bufer),sz);
        f.close();
        sz/=sizeof(T);

        for ( int i = 0 ; i < sz ; i++)//for loop to push back the queue elements 
                vector<T>::push_back(bufer[i]);
                free (bufer);   
        }

    mtx.unlock();

     

}


~MultiHeadQueue(){}


};



#endif