#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <utility>
#include <algorithm>
#include <iostream>

using namespace std;

// stochasticPerfectHash
// Uses codetable randomization to derive a Pearson mixing table for sets of alphabetic keys
// orthopteroid@gmail.com
// g++ -std=c++0x -O3 -lrt sph.cpp -o sph
//
// for background, read:
// http://cs.mwsu.edu/~griffin/courses/2133/downloads/Spring11/p677-pearson.pdf

// given the lowercase string and the codeTable, compute the pearson hash
template<class CodeType>
int ph_lowercase(const char *keyword, CodeType *codeTable, uint CodeCount)
{
    int hash = codeTable[ keyword[0] - 'a' ]; // char 0
    for(uint i=1; keyword[i]; i++) { // chars 1 ... N
        hash = codeTable[ (hash ^ (keyword[i] - 'a')) % CodeCount ];
    }
    return hash;
}

///////////////////////////
// calculation

// takes an array of keywords and returns permuted code and keywords tables that construct a perfect hash
// this version returns a packed vector of keywords where the keyword ordinal is it's hash
template<class CodeType, uint MaxIterations>
size_t sph_packed(const vector<string> keywords, vector<CodeType> &permutedCodes, vector<string> &permutedKeywords)
{
    const uint CodeCount = permutedCodes.size();
    const uint KeywordCount = keywords.size();
    permutedKeywords.resize(KeywordCount);

    // avg keyword length is used to count code swaps in the mixing table
    uint letterCount = 0;
    for(uint i=0;i<KeywordCount;i++) { letterCount += keywords[i].length(); }
    uint avgKeyLength = letterCount / KeywordCount;

    // setup mixing table
    for(uint i=0;i<CodeCount;i++) { permutedCodes[i] = max(CodeCount,KeywordCount) - i; }

    // retry loop...
    size_t iterations = 0;
    do {
        // randomly permute
        for(uint i=0;i<CodeCount/2;i++) { std::swap(permutedCodes[rand() % CodeCount], permutedCodes[rand() % CodeCount]); }

        // clear analysis state
        for(uint i=0;i<KeywordCount;i++) { permutedKeywords[i].clear(); }

        // hash all keywords
        uint k;
        for(k=0;k<KeywordCount;k++ )
        {
            int i = ph_lowercase<CodeType>(keywords[k].data(), permutedCodes.data(), permutedCodes.size()) % KeywordCount;
            if( !permutedKeywords[ i ].empty() ) { break; } // collision
            permutedKeywords[ i ] = keywords[k]; // record keyword for this index
        }
        if(k >= KeywordCount) { break; } // all accounted for!

    } while( ++iterations < MaxIterations );

    return iterations;
}

// takes an array of keywords and returns permuted code and keywords tables that construct a perfect hash
// this version also takes a sparsity coefficient that makes 'free space' in the permuted keyword table
// the returned vector of keywords is sparse, in that some ordinals are not mapped to keyword hashes
// this allows for some flexibility in the time-complexity of finding a suitable permuted-code-table (if it's even possible)
template<class CodeType, uint MaxIterations>
size_t sph_sparse(const vector<string> keywords, vector<CodeType> &permutedCodes, vector<string> &permutedKeywords, uint extra)
{
    permutedKeywords.resize(keywords.size() +extra);

    const uint CodeCount = permutedCodes.size();
    const uint KeywordCount = keywords.size();
    const uint SparseKeyCount = permutedKeywords.size();

    // avg keyword length is used to count code swaps in the mixing table
    uint letterCount = 0;
    for(uint i=0;i<KeywordCount;i++) { letterCount += keywords[i].length(); }
    uint avgKeyLength = letterCount / KeywordCount;

    // setup codetable table
    for(uint i=0;i<CodeCount;i++) { permutedCodes[i] = max(CodeCount,KeywordCount) - i; }

    // retry loop...
    size_t iterations = 0;
    do {
        // randomly permute
        for(uint i=0;i<CodeCount/2;i++) { std::swap(permutedCodes[rand() % CodeCount], permutedCodes[rand() % CodeCount]); }

        // clear analysis state
        for(uint i=0;i<SparseKeyCount;i++) { permutedKeywords[i].clear(); }

        // hash all keywords
        uint k;
        for(k=0;k<KeywordCount;k++ )
        {
            int i = ph_lowercase<CodeType>(keywords[k].data(), permutedCodes.data(), permutedCodes.size()) % SparseKeyCount;
            if( !permutedKeywords[ i ].empty() ) { break; } // collision
            permutedKeywords[ i ] = keywords[ k ]; // record keyword for this index
        }
        if(k >= KeywordCount) { break; } // all accounted for!

    } while( ++iterations < MaxIterations );

    return iterations;
}

template<class CodeType>
void sph_dump(vector<string> permutedKeywords, vector<CodeType> permutedCodes )
{
    printf("char* ph_keywords[] = {");
    for_each(permutedKeywords.begin(), permutedKeywords.end(), [] (const string s) {
        if(s.length() > 0)
            printf("\"%s\", ", s.data());
        else
            printf("0, ");
    } );
    printf("};\n");

    printf("%s ph_mix[] = {", permutedCodes.size() > 256 ? "uint16_t" : "uint8_t");
    for_each(permutedCodes.begin(), permutedCodes.end(), [] (const CodeType c) { printf("%u, ", c); } );
    printf("};\n");
}

//////////////////////
// demo code

int main()
{
    time_t ltime;
    time(&ltime);
    srand((unsigned int)ltime);

    using codeType = uint16_t;
    const int codeCount = 15;
    const int keyCount = 10;
    const int maxIter = 10000;
    const int trials = 5;
    const float growthFactor = 1.2;

    printf("Tring to hash %d keys into %d element Pearson codetable...\n", keyCount, codeCount);

    for(uint t=0;t<trials;t++)
    {
        fflush(stdout);

        vector<string> keywords(keyCount);
        vector<string> permutedKeywords(keyCount);
        vector<codeType> permutedCodes(codeCount);

        // init strings to random lowercase chars
        printf("Keywords (");
        for_each(keywords.begin(), keywords.end(), [] (string &s) {
            int keyLength = 3 + rand() % 5;
            s.resize(keyLength);
            for(uint i=0;i<keyLength;i++) { s[ i ] = 'a' + rand() % 26; }
            cout << s << ", ";
        } );
        printf("). ");

        uint iterations = 0;
        printf("Looking for a packed keyword solution... ");
        iterations = sph_packed<codeType, maxIter>(keywords, permutedCodes, permutedKeywords);
        if (iterations < maxIter) {
            printf("Solution after %u iterations\n",iterations);
            sph_dump(permutedKeywords, permutedCodes);
            continue;
        }
        printf("Failed. Trying comparative approaches.\n");

        {
            uint totaliter = 0;
            printf("Adjusting codetable size... ");
            uint newSize = codeCount -2; // start smaller - it might help
            for (int i = 0; i < 100; i++) {
                permutedCodes.resize(newSize);
                iterations = sph_packed<codeType, maxIter>(keywords, permutedCodes, permutedKeywords);
                totaliter += iterations;
                if (iterations < maxIter) { break; }
                newSize += 1;
            }
            if (iterations < maxIter) {
                printf("Solution in a %u element codetable after %u iterations\n", newSize, totaliter);
                sph_dump(permutedKeywords, permutedCodes);
            } else {
                printf("Failure.\n");
            }
        }

        {
            uint totaliter = 0;
            permutedCodes.resize(codeCount); // reset
            printf("Trying sparse in %u element codetable... ", codeCount);
            uint extra = 1;
            for (int i = 0; i < 100; i++) {
                iterations = sph_sparse<codeType, maxIter>(keywords, permutedCodes, permutedKeywords, extra);
                totaliter += iterations;
                if (iterations < maxIter) { break; }
                extra += 1;
            }
            if (iterations < maxIter) {
                float sparsity = ((float)keywords.size() + (float)extra) / (float)keywords.size();
                printf("Solution at sparsity %4.2f after %u iterations\n", sparsity, totaliter);
                sph_dump(permutedKeywords, permutedCodes);
            } else {
                printf("Failure.\n");
            }
        }
    }

    return 0;
}
