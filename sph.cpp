#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <utility>
#include <algorithm>
#include <iostream>
#include <functional>
#include <vector>
#include <random>
#include <fstream>
#include <iterator>
#include <sstream>

using namespace std;

// stochasticPerfectHash
// orthopteroid@gmail.com

// credit to http://web.mit.edu/freebsd/head/sys/libkern/crc32.c
// credit to http://cs.mwsu.edu/~griffin/courses/2133/downloads/Spring11/p677-pearson.pdf
// credit to https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function

// todo: go back to using this but in a cleaner way
struct KeyHash {
    uint8_t key, hash;
};

typedef std::function<KeyHash(const string &, const vector<uint8_t>&, const size_t)> fnHash;
typedef std::mt19937 rng_type;
static rng_type rng;

static int maxiter = 50000;
static int mixersize = 32;
static int permutesize = -1;

//////////////////////

// simple to implement on narrow (ie 8bit) architectures
uint8_t hash_pearson(const char* sz, const uint8_t* mixTable, const size_t mixSize)
{
    const uint8_t mixMask = mixSize -1;
    uint8_t hash = ~0;
    while(*sz)
        hash ^= mixTable[ *(sz++) & mixMask ];
    return hash;
}

// a standard single instruction since ARMv8.1 and SSE4.2
uint32_t hash_crc32(const char* sz) {
    const uint32_t crc32_tab[] = {
         0x00000000,  0x77073096,  0xee0e612c,  0x990951ba,  0x076dc419,  0x706af48f, 0xe963a535 , 0x9e6495a3,
         0x0edb8832,  0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
        0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
        0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
        0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
        0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
        0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
        0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
        0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
        0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
        0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
        0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
         0x4db26158,  0x3ab551ce,  0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
        0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
        0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
        0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
        0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
        0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
        0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
        0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
        0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
        0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
        0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
        0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
        0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
        0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
        0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
        0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
        0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
        0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
        0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
        0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
    };
    uint32_t crc = ~0U;
    while(*sz)
        crc = crc32_tab[ (0xFF & crc) ^ *(sz++) ] ^ (crc >> 8);
    crc = crc ^ ~0U;
    return crc;
}

// simple to implement 32bit
uint32_t hash_fnv1a32(const char* sz)
{
    uint32_t hash = 0x811c9dc5;
    while(*sz)
        hash = (hash ^ (uint32_t)*(sz++)) * 0x01000193;
    return hash;
}

// simple to implement with more state than 32bit hashes
uint64_t hash_fnv1a64(const char* sz)
{
    uint64_t hash = 0xcbf29ce484222325;
    while(*sz)
        hash = (hash ^ (uint64_t)*(sz++)) * 0x00000100000001B3;
    return hash;
}

//////////////////////

// pearson-style hash with mixtable
KeyHash sph_pearson(const string & keyword, const vector<uint8_t> & mixTable, const size_t pkSize)
{
    auto hash = hash_pearson(keyword.c_str(), mixTable.data(), mixTable.size());

    return {static_cast<uint8_t>(hash % pkSize), hash};
}

// 32bit hash with mixtable post-processing
KeyHash sph_crc32(const string & keyword, const vector<uint8_t> & mixTable, const size_t pkSize)
{
    // do some bit counting...
    const uint8_t mixMask = mixTable.size() -1;
    uint bits = mixMask;
    uint mixBits = 0;
    while (bits) { mixBits++; bits >>= 1; }

    // gobble the temp value through the mixtable
    uint8_t hash = 0U;
    auto tmp = hash_crc32(keyword.c_str());
    while (tmp) { hash ^= mixTable[tmp & mixMask ]; tmp >>= mixBits; }

    return {static_cast<uint8_t>(hash % pkSize), hash};
}

// 32bit hash with mixtable post-processing
KeyHash sph_fnv1a32(const string & keyword, const vector<uint8_t> & mixTable, const size_t pkSize)
{
    // do some bit counting...
    const uint8_t mixMask = mixTable.size() -1;
    uint bits = mixMask;
    uint mixBits = 0;
    while (bits) { mixBits++; bits >>= 1; }

    // gobble the temp value through the mixtable
    uint8_t hash = 0U;
    auto tmp = hash_fnv1a32(keyword.c_str());
    while (tmp) { hash ^= mixTable[tmp & mixMask ]; tmp >>= mixBits; }

    return {static_cast<uint8_t>(hash % pkSize), hash};
}

// 64bit hash with mixtable post-processing
KeyHash sph_fnv1a64(const string & keyword, const vector<uint8_t> & mixTable, const size_t pkSize)
{
    // do some bit counting...
    const uint8_t mixMask = mixTable.size() -1;
    uint bits = mixMask;
    uint mixBits = 0;
    while (bits) { mixBits++; bits >>= 1; }

    // gobble the temp value through the mixtable
    uint8_t hash = 0U;
    auto tmp = hash_fnv1a64(keyword.c_str());
    while (tmp) { hash ^= mixTable[ tmp & mixMask ]; tmp >>= mixBits; }

    return {static_cast<uint8_t>(hash % pkSize), hash};
}

///////////////////////////

// takes an array of keywords and returns permuted code and keywords tables that construct a perfect hash
bool sph_solve(const vector<string> &keywords, uint &iter, vector<uint8_t> &mixTable, vector<string> &permutedKeywords, vector<uint8_t> &hashes, fnHash fn_hash)
{
    vector<uint8_t> srcTable( 256);
    for( uint c = 0; c < 256; c++ )
        srcTable[ c ] = c;

    std::uniform_int_distribution<rng_type::result_type> udist(0, srcTable.size() -1);

    // mixtable retry loop...
    iter = 0;
    while(iter < maxiter )
    {
        // permute the srcTable and build mixtable and clear the hash stats
        for( uint i = 0; i < srcTable.size(); i++ )
            std::swap(srcTable[i], srcTable[ udist(rng) ]);
        for( uint i = 0; i < mixTable.size(); i++ )
            mixTable[i] = srcTable[i];
        for( uint pk = 0; pk < permutedKeywords.size(); pk++ )
            { hashes[pk] = 0; permutedKeywords[pk].clear(); }

        // trial hash all keywords. check for collisions
        uint k = 0;
        for( k = 0; k < keywords.size(); k++ )
        {
            auto kh = fn_hash(keywords[k], mixTable, permutedKeywords.size());
            auto pk = kh.key;
            if( permutedKeywords[ kh.key ].length() > 0 )
                break; // collision
            hashes[ kh.key ] = kh.hash;
            permutedKeywords[ kh.key ] = keywords[ k ];
        }
        if( k == keywords.size() )
            return false; // no collisions
        iter++;
    }

    return true;
}

void sph_keywords_random(vector<string>& keywords, const uint count, const uint minlength)
{
    const char* validchars = "abcdefghijklmnopqrstuvwxyz";
    auto validstrlen = strlen(validchars);

    std::uniform_int_distribution<rng_type::result_type> udistLen(0, 3);
    std::uniform_int_distribution<rng_type::result_type> udistChar(0, validstrlen -1);

    keywords.resize(count);

    for_each(keywords.begin(), keywords.end(), [&] (string &s)
    {
        int keylength = minlength + udistLen(rng);
        s.resize(keylength);
        for(uint i = 0; i < keylength; i++ )
            s[ i ] = validchars[ udistChar(rng) ];
    } );
}

void sph_keywords_file(vector<string>& keywords, const string &fqfilename)
{
    ifstream ifs(fqfilename);
    keywords.clear();
    string line;
    while (getline(ifs, line))
        if(line.length() > 0)
            keywords.emplace_back(line);
}

///////////////////////////

void sph_soln(const vector<string> &keywords, const vector<uint8_t> &mixTable, const vector<string> &permutedKeywords, const vector<uint8_t> &hashes)
{
    printf("const char* Keyword[ %lu ] = { ", permutedKeywords.size());
    for_each(permutedKeywords.begin(), permutedKeywords.end(), [] (const string s) {
        if(s.length() > 0)
            printf("\"%s\", ", s.data());
        else
            printf("0, ");
    } );
    printf("};\n");

    printf("enum Key { ");
    for( uint pk = 0; pk < permutedKeywords.size(); pk++ )
        if(permutedKeywords[pk].length() > 0)
        {
            string identifier = permutedKeywords[ pk ];
            for_each(identifier.begin(), identifier.end(), [](string::value_type& c ) { c = toupper(c); } );
            printf("%s=%u, ", identifier.data(), pk);
        }
    printf("};\n");

    printf("const uint8_t Hash[ %lu ] = { ", hashes.size());
    for_each(hashes.begin(), hashes.end(), [] (const uint32_t h) { printf("0x%02X, ", h); } );
    printf("};\n");

    printf("uint8_t Mixtable[ %lu ] = { ", mixTable.size());
    for_each(mixTable.begin(), mixTable.end(), [] (const uint8_t c) { printf("0x%02X, ", c); } );
    printf("};\n");
    printf("\n");
}

//////////////////////

void sph_try(const char* szMethod, vector<string> &keywords, vector<uint8_t> &mixTable, fnHash fn_hash)
{
    vector<string> permutedKeywords(keywords.size());
    vector<uint8_t> hashes(keywords.size());
    uint iterations = 0;
    bool collision = false;

    if (permutesize > -1)
    {
        permutedKeywords.resize(permutesize);
        hashes.resize(permutesize);
    }

    for(uint i = keywords.size(); i < keywords.size() * 2; i++)
    {
        if (permutesize == -1)
        {
            // if there are collisions, try a progressively larger keyword table
            permutedKeywords.resize(i);
            hashes.resize(i);
        }
        collision = sph_solve(keywords, iterations, mixTable, permutedKeywords, hashes, fn_hash);
        if (!collision)
        {
            printf("%s solution for %lu keywords using a %lu keyword table (%u iterations)\n", szMethod, keywords.size(), permutedKeywords.size(), iterations);
            sph_soln(keywords, mixTable, permutedKeywords, hashes);
            return;
        }
    }
    printf("Failure!\n");
}

int main(int argc, char **argv)
{
    string keywordfile;
    vector<uint8_t> mixTable;
    vector<string> keywords;

    time_t ltime;
    time(&ltime);
    rng.seed((unsigned int)ltime);

    vector<string> args(argv + 1, argv + argc);
    for (auto i = args.begin(); i != args.end(); ++i) {
        if (*i == "-h" || *i == "--help") {
            cout << "Syntax: sph [--maxiter <number>] [--mixersize <number>] [--permutesize <number>] [--keywordfile <infile>]" << endl;
            return 0;
        } else if (*i == "--maxiter") {
            maxiter = strtol( (++i)->c_str(), NULL, 10 );
        } else if (*i == "--mixersize") {
            mixersize = strtol( (++i)->c_str(), NULL, 10 );
        } else if (*i == "--permutesize") {
            permutesize = strtol( (++i)->c_str(), NULL, 10 );
        } else if (*i == "--keywordfile") {
            keywordfile = *++i;
        }
    }

    mixTable.resize(mixersize); // must be ^2 and <= 256

    if (keywordfile.length() > 0)
        sph_keywords_file(keywords, keywordfile);
    else
        sph_keywords_random(keywords, 20 /* count */, 5 /* minlength */);

    sph_try("pearson", keywords, mixTable, sph_pearson);
    sph_try("crc32", keywords, mixTable, sph_crc32);
    sph_try("fnv1a32", keywords, mixTable, sph_fnv1a32);
    sph_try("fnv1a64", keywords, mixTable, sph_fnv1a64);

    return 0;
}

