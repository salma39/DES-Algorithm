
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;


#ifdef __GNUC__
# define __rdtsc __builtin_ia32_rdtsc
#else
# include<intrin.h>
#endif


u64 read_u64_hex(const char*data)

    {

        u64 ret=0;
        for(;;++data)//read left-to-right
            {
                unsigned char dec=*data-'0';

                if(dec<10)
                    ret=ret<<4|dec;
                else {

                unsigned char upper=(*data&0xDF)-'A';

                if(upper>5)
                    break;

                ret=ret<<4|upper+10;

                }

            }
        return ret;

    }






vector<u64> keyPreparation(u64 key)
{
    // 0001001100110100010101110111100110011011101111001101111111110001

    u64 permutedKey = 0;
    vector<pair<u64,u64>> keys;
    vector<u64> finalKeys;
   // bitset<64> key(k);
  // u64 key=0;
/*    for(int i=0; k>0; i++)
    {
        key |= k%2;
        key<<1;
        k= k/2;
    }
*/

    int shiftTable[16] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

    int pC1[56] ={
        57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
    };

    int pC2[48] = {
        14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8, 16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
    };




    //first permutation
    for(int i=0;i<56; ++i)
    {
        permutedKey |= (key>>(64-pC1[56-1-i])&1)<<(u64)i;

    }

    //ba2sem el key


    u64 left = permutedKey>>28;

    u64 right = permutedKey & (u64)0xFFFFFFF;
    //cout<<hex<<left<<endl;

    keys.push_back(make_pair(left,right));
    // shift

    for(int i=1;i<17;++i)
    {
        u64 l = keys[i-1].first;
        u64 r = keys[i-1].second;

        int shift = shiftTable[i-1];
        l = ((l << (u64)shift)&(u64)0xFFFFFFF)|(l >> (u64)(28-shift)) ;

        r =((r << (u64)shift)&(u64)0xFFFFFFF)|(r >> (u64)(28-shift));

        keys.push_back(make_pair(l,r));
    }


    for (int i = 0; i < 16; ++i)
        {
            u64 k = keys[i+1].first<<(u64)28 | keys[i+1].second;



            u64   fkey=0;
                 for(int j=0;j<48; j++)
            {
                 fkey |= (k>>(56-pC2[48-1-j])&1)<<(u64)j;

            }


                finalKeys.push_back(fkey);

    }


    return finalKeys;
}







u64 expand(u64 str)
{
    u64 result = 0;

    int ePermutations[48] = {
         32,     1,    2,     3,     4,    5,
          4,     5,    6,     7,     8,    9,
          8,     9,   10,    11,    12,   13,
         12,    13,   14,    15,    16,   17,
         16,    17,   18,    19,    20,   21,
         20,    21,   22,    23,    24,   25,
         24,    25,   26,    27,    28,   29,
         28,    29,   30,    31,    32,    1
    };

    for(int i=0;i<48;++i)
    {

        result |= (str>>(32-ePermutations[47-i])&1)<<i;
    }
     //cout<<str<<"exp   "<<hex<<result<<endl;
    return result;
}


u64 F(u64 str1,u64 str2)
{
    // result is 48 bit
    u64 result = str1^ str2;

    int sboxes[512] =
    {

             14,  4,  13,  1,   2, 15,  11,  8,   3, 10,   6, 12,   5,  9,   0,  7 ,
              0, 15,   7,  4,  14,  2,  13,  1,  10,  6,  12, 11,   9,  5,   3,  8 ,
              4,  1,  14,  8,  13,  6,   2, 11,  15, 12,   9,  7,   3, 10,   5,  0 ,
             15, 12,   8,  2,   4,  9,   1,  7,   5, 11,   3, 14,  10,  0,   6, 13,


             15,  1,   8, 14,   6, 11,   3,  4,   9,  7,   2, 13,  12,  0,   5, 10 ,
              3, 13,   4,  7,  15,  2,   8, 14,  12,  0,   1, 10,   6,  9,  11,  5 ,
              0, 14,   7, 11,  10,  4,  13,  1,   5,  8,  12,  6,   9,  3,   2, 15 ,
             13,  8,  10,  1,   3, 15,   4,  2,  11,  6,   7, 12,   0,  5,  14,  9 ,

             10,  0,   9, 14,   6,  3,  15,  5,   1, 13,  12,  7,  11,  4,   2,  8 ,
             13,  7,   0,  9,   3,  4,   6, 10,   2,  8,   5, 14,  12, 11,  15,  1 ,
             13,  6,   4,  9,   8, 15,   3,  0,  11,  1,   2, 12,   5, 10,  14,  7 ,
              1, 10,  13,  0,   6,  9,   8,  7,   4, 15,  14,  3,  11,  5,   2, 12,


              7, 13,  14,  3,   0,  6,   9, 10,   1,  2,   8,  5,  11, 12,   4, 15 ,
             13,  8,  11,  5,   6, 15,   0,  3,   4,  7,   2, 12,   1, 10,  14,  9 ,
             10,  6,   9,  0,  12, 11,   7, 13,  15,  1,   3, 14,   5,  2,   8,  4 ,
              3, 15,   0,  6,  10,  1,  13,  8,   9,  4,   5, 11,  12,  7,   2, 14 ,

            2, 12,   4,  1,   7, 10,  11,  6,   8,  5,   3, 15,  13,  0,  14,  9 ,
             14, 11,   2, 12,   4,  7,  13,  1,   5,  0,  15, 10,   3,  9,   8,  6 ,
              4,  2,   1, 11,  10, 13,   7,  8,  15,  9,  12,  5,   6,  3,   0, 14 ,
             11,  8,  12,  7,   1, 14,   2, 13,   6, 15,   0,  9,  10,  4,   5,  3 ,

             12,  1,  10, 15,   9,  2,   6,  8,   0, 13,   3,  4,  14,  7,   5, 11 ,
             10, 15,   4,  2,   7, 12,   9,  5,   6,  1,  13, 14,   0, 11,   3,  8 ,
              9, 14,  15,  5,   2,  8,  12,  3,   7,  0,   4, 10,   1, 13,  11,  6 ,
              4,  3,   2, 12,   9,  5,  15, 10,  11, 14,   1,  7,   6,  0,   8, 13 ,

              4, 11,   2, 14,  15,  0,   8, 13,   3, 12,   9,  7,   5, 10,   6,  1 ,
             13,  0,  11,  7,   4,  9,   1, 10,  14,  3,   5, 12,   2, 15,   8,  6 ,
              1,  4,  11, 13,  12,  3,   7, 14,  10, 15,   6,  8,   0,  5,   9,  2 ,
              6, 11,  13,  8,   1,  4,  10,  7,   9,  5,   0, 15,  14,  2,   3, 12 ,

             13,  2,   8,  4,   6, 15,  11,  1,  10,  9,   3, 14,   5,  0,  12,  7 ,
              1, 15,  13,  8,  10,  3,   7,  4,  12,  5,   6, 11,   0, 14,   9,  2 ,
              7, 11,   4,  1,   9, 12,  14,  2,   0,  6,  10, 13,  15,  3,   5,  8 ,
              2,  1,  14,  7,   4, 10,   8, 13,  15, 12,   9,  0,   3,  5,   6, 11 };

    u64 output = 0;

    int* box=0;
    int idx=0;

  for(int i =0; i<8;i++)
{

        box = &sboxes[0] + i*64; //fetch the current S-box
        idx = result >> (7-i)*6 & 0x3F ;//get the index
        idx = idx>>1&15|(idx&1)<<4|idx&0x20; //reorder bits
        output |= box[idx] << (7-i)*4;
}




    u64 permutedOutput = 0;

    int permutations[32] = {
        16,  7, 20, 21,
        29, 12, 28, 17,
         1, 15, 23, 26,
         5, 18, 31, 10,
         2,  8, 24, 14,
        32, 27,  3,  9,
        19, 13, 30,  6,
        22, 11,  4, 25
    };

    for(int i=0; i<32; i++)
    {
        permutedOutput |= (output>>(32-permutations[31-i])&1)<<(u64)i;
    }

    return permutedOutput;
}


u64 DESEncryption(u64 dataBlock, vector<u64> keys)
{
    // 0000000100100011010001010110011110001001101010111100110111101111

   /* for(int i=0;i< keys.size();i++)
    {
        cout<<keys[i]<<endl;
    }*/
    u64 permutedBlock = 0;

    int IP[64] = {
        58,    50,   42,    34,    26,   18,    10,    2,
        60,    52,   44,    36,    28,   20,    12,    4,
        62,    54,   46,    38,    30,   22,    14,    6,
        64,    56,   48,    40,    32,   24,    16,    8,
        57,    49,   41,    33,    25,   17,     9,    1,
        59,    51,   43,    35,    27,   19,    11,    3,
        61,    53,   45,    37,    29,   21,    13,    5,
        63,    55,   47,    39,    31,   23,    15,    7
    };

    for(int i=0 ; i<64 ; i++)

    {

        permutedBlock |= (dataBlock>>(64-IP[63-i])&1)<<(u64)i;
    }

    u64 L0 = permutedBlock>>(u64)32;
    u64 R0 = permutedBlock& (u64)0xFFFFFFFF;


    vector< pair<u64, u64> > data;
    data.push_back(make_pair(L0, R0));

    for(int i=1;i<17;++i)
    {
        //left = right
        u64 L = data[i-1].second;
        //r = left xor f(r,keyn)
       // cout<<hex<<data[i-1].second<<endl;
        u64 R =
            data[i-1].first ^ F(expand(data[i-1].second), keys[i-1]);
       // cout<<F(expand(data[i-1].second), keys[i-1])<<endl;
        data.push_back(make_pair(L, R));

    }

    u64 encryptedDataReversedKey = 0;
    encryptedDataReversedKey |= data[data.size()-1].second<<(u64)32;
    encryptedDataReversedKey |= data[data.size()-1].first&(u64) 0xFFFFFFFF;
   // cout<<hex<<encryptedDataReversedKey<<endl;
    u64 finalPermutedBlock = 0;

    int finalPermutation[64] = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41,  9, 49, 17, 57, 25
    };

    for(int i=0 ; i<64 ; ++i)
    {
        finalPermutedBlock |= (encryptedDataReversedKey>>(64-finalPermutation[63-i])&1)<<(u64)i;

    }


    return finalPermutedBlock;
}


u64 DESDecrption(u64 encryptedData,  vector<u64> keys)
{
   vector<u64 > keyss = keys;
    reverse(keyss.begin(), keyss.end());


    u64 allPlainText = DESEncryption(encryptedData,keyss);


    return allPlainText;
}





int main(int argc, char *argv[])
 {
     u64 key = read_u64_hex(argv[3]);

    // cout<<key<<endl;
     u64 data = read_u64_hex(argv[2]);




     vector<u64> keys =keyPreparation(key) ;
     string op = argv[1];



     if( op == "encrypt")
     {
        long long t1=__rdtsc();
        u64 encryptedData= DESEncryption(data,keys);
        long long t2=__rdtsc();
       printf("cipher: %016llX\n", encryptedData);
       printf("Cycles: %lld\n", t2-t1);
     }

    else if(op=="decrypt")

    {
        long long t1=__rdtsc();
        u64 plainT = DESDecrption(data,keys);
         long long t2=__rdtsc();
      printf("plain: %016llX\n", plainT);
       printf("Cycles: %lld\n", t2-t1);
    }


     return 0;
 }
