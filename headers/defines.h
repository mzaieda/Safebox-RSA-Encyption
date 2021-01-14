#ifndef DEFINES_H
#define DEFINES_H

#include <cstring>
#include <string>
#include <string.h>

typedef unsigned __int128 uint128_t;

#define COMMAND_LINE_ARGUMENT_MAX_SIZE 128
#define ERROR_STRING_MAX_SIZE 128



/*      UINT64_MAX 18446744073709551615ULL */
#define P10_UINT64 10000000000000000000ULL   /* 19 zeroes */
#define E10_UINT64 19

#define STRINGIZER(x)   # x
#define TO_STRING(x)    STRINGIZER(x)

#define PHASE1     1
#define PHASE2     2
#define PHASE3     3

namespace CryptoPP{
    typedef unsigned char byte;
};


typedef struct sb_block_index{ 
    long block;
    long shred_block;
    char shred;
} sb_block_index_t;


typedef struct node {

        long blocksize;
        long blocks;
        long bytes;
        long shreds;
        string filename;
        string hashed_name;
        bool flag;

       
        ostream & operator << (std::ostream & os, node * n) {
            os << n-> filename << "--" << n-> blocksize << "--"<<n-> blocks << "--" << n-> bytes << "--" << n-> shreds << "--" << n-> hashed_name << endl; 
            
            return os;
        }

} node_t;



#endif
