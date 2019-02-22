#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <utility>
#include <algorithm>
#include <iostream>
#include <cassert>

using namespace std;

// stochasticPerfectHash
// Uses codetable randomization to derive a Pearson mixing table for sets of alphabetic keys
// orthopteroid@gmail.com
// g++ -std=c++0x -O3 -lrt sph.cpp -o sph
//
// for background, read:
// http://cs.mwsu.edu/~griffin/courses/2133/downloads/Spring11/p677-pearson.pdf

const char* validchars = "abcdefghijklmnopqrstuvwxyz"; // contigious ascii chars

// given the string and the mix table, compute the pearson hash
uint8_t ph_hash(const char *keyword, const uint8_t keywordSize, const uint8_t *mixTable, const uint8_t mixSize)
{
    uint8_t hash = mixTable[ keyword[0] - validchars[0] ]; // char 0
    for(uint i = 1; keyword[i]; i++) // chars 1 ... N
        hash = mixTable[ ( hash ^ ( keyword[i] - validchars[0] ) ) % mixSize ];
    return hash % keywordSize;
}

///////////////////////////
// calculation

// takes an array of keywords and returns permuted code and keywords tables that construct a perfect hash
template<uint MaxIterations>
bool sph_calc(uint &iter, const vector<string> &keywords, vector<uint8_t> &charMixTable, vector<string> &permutedKeywords)
{
    vector<int> index(permutedKeywords.size());

    // retry loop...
    uint iterations = 0;
    do {
        for(uint i=0;i<permutedKeywords.size();i++) { index[i] = -1; }

        // trial hash all keywords. check for collisions
        uint k = 0;
        for( k = 0; k < keywords.size(); k++ )
        {
            uint8_t i = ph_hash(keywords[k].data(), (uint8_t)permutedKeywords.size(), charMixTable.data(), (uint8_t)charMixTable.size());
            if( index[ i ] != -1 ) break; // collision
            index[ i ] = k;
        }
        if( k == keywords.size() ) { // no collisions
            for( uint i = 0; i < keywords.size(); i++ )
                if( index[ i ] != -1 ) permutedKeywords[ i ] = keywords[ index[ i ] ];

            return false;
        }

        // permute the character mixtable
        for(uint i=0;i<charMixTable.size()/2;i++) { std::swap(charMixTable[i], charMixTable[rand() % charMixTable.size()]); }

        iter++;
    } while( ++iterations < MaxIterations );

    return true;
}

//////////////////////
// demo code

int main()
{
    time_t ltime;
    time(&ltime);
    srand((unsigned int)ltime);

    using codeType = uint8_t;
    const int keyCount = 10;
    const int maxIter = 500;
    const int trials = 5;

    for(uint t=0;t<trials;t++)
    {
        fflush(stdout);

        vector<string> keywords(keyCount);
        vector<string> permutedKeywords(keyCount);

        // init strings to random lowercase chars
        printf("Keywords (");
        for_each(keywords.begin(), keywords.end(), [&] (string &s) {
            int keyLength = 5 + rand() % 5;
            s.resize(keyLength);
            for(uint i=0;i<keyLength;i++) { s[ i ] = validchars[ rand() % strlen(validchars) ]; }
            cout << s << ", ";
        } );
        printf(")\n");

        uint iterations = 0;
        bool collision = false;
        vector<codeType> charMixTable;

        // try progressively larger keyword table
        for (uint i = keywords.size(); i < keywords.size() * 2; i++) {
            permutedKeywords.resize(i);

            // try progressively larger character mix table
            for(uint j = keyCount * 5 / 4; j < strlen(validchars) * 2; j++) {
                charMixTable.resize(j);

                for(uint8_t c = 0; c < charMixTable.size(); c++) { charMixTable[ c ] = c + 3; }

                // test some permutations of the above
                collision = sph_calc<maxIter>(iterations, keywords, charMixTable, permutedKeywords);
                if (!collision) { goto foo; }
            }
        }
foo:
        if (!collision) {
            printf("Solution for %u keywords using a %lu keyword table with a %lu character mix table (%u iterations)\n", keyCount, permutedKeywords.size(), charMixTable.size(), iterations);

            printf("uint8_t Mixtable[ %lu ] = { ", charMixTable.size());
            for_each(charMixTable.begin(), charMixTable.end(), [] (const uint8_t c) { printf("%u, ", c); } );
            printf("};\n");

            printf("const char* Keyword[ %lu ] = { ", permutedKeywords.size());
            for_each(permutedKeywords.begin(), permutedKeywords.end(), [] (const string s) {
                if(s.length() > 0)
                    printf("\"%s\", ", s.data());
                else
                    printf("0, ");
            } );
            printf("};\n");

            printf("enum Key { ");
            for( uint i = 0; i < permutedKeywords.size(); i++ ) {
                if(permutedKeywords[i].length() == 0) continue;
                auto hash = ph_hash(permutedKeywords[i].data(), (uint8_t)permutedKeywords.size(), charMixTable.data(), (uint8_t)charMixTable.size());
                string identifier = permutedKeywords[i];
                for_each(identifier.begin(), identifier.end(), [](string::value_type& c ) { c = toupper(c); } );
                printf("%s=%u, ", identifier.data(), hash);
                assert(i == hash);
            }
            printf("};\n");
        } else {
            printf("Failure.\n");
        }
        printf("\n");
    }

    return 0;
}

