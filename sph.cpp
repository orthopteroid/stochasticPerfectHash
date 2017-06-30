#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <utility>
#include <algorithm>

// stochasticPerfectHash
// Uses stochastic remainder sampling to derive a Pearson mixing table for small sets of alphabetic keys
// orthopteroid@gmail.com
// g++ -std=c++0x -O3 -lrt sph.cpp -o sph
//
// for background, read:
// http://cs.mwsu.edu/~griffin/courses/2133/downloads/Spring11/p677-pearson.pdf

// stochastic remainder sampler
template<class TableType>
int sample( TableType* counts, int sum )
{
    int32_t target = (int32_t)rand() % sum;
    size_t i = 0;
    while( (target -= counts[ i ]) > 0 ) { i++; }
    return i;
}

template<class TableType, int TableSize>
int sum( TableType* counts )
{
    uint32_t sum = 0;
    for( size_t i=0;i<TableSize;i++ )
    {
        sum += counts[i];
    }
    return sum;
}

// calculates a mixtable for the pearson hash to create a perfect hash.
// keys can only be mixed-case, but can be of any length.
// keys are seperated in the input string using the ! delimiter. ! must also be at end of string.
template<class TableType, int TableSize>
size_t calcMixTable( char* sz )
{
	const size_t MAXITER = 1000000;

    // special marker '!' in the string designates the end of a key
    int keyCount = 0;
    for( size_t i=0;sz[i];i++ ) { if( sz[i] == '!' ) { keyCount++; } }

    TableType usedKeyHash[TableSize]; // to mark key-hashes used
    TableType usedCode[TableSize], notUsedCode[TableSize]; // to mark ascii codes used
    TableType mixTable[TableSize];

    int avgKeyLength = ( ::strlen(sz) - keyCount ) / keyCount;

	// start with a random mixing
    // thanks to https://github.com/tdewolff/hasher
    for(int i=0;i<TableSize;i++) mixTable[i] = i;
    for(int i=0;i<TableSize;i++) std::swap(mixTable[rand() % TableSize],mixTable[rand() % TableSize]);

	// reset flags...
	size_t iterations = 0;
	do {
        for(int i=0;i<TableSize;i++) usedCode[i] = usedKeyHash[i] = 0;

		// compute the pearson hash using the current mix table.
        TableType hash = mixTable[ sz[0] - 'a' ]; // start hash for 1st keyword
		for( size_t i=0;sz[i];i++ )
		{
			if( sz[i] == '!' ) // end of key detected
			{
                usedKeyHash[ hash % TableSize ]++; // record hash for completed key as used
                hash = mixTable[ sz[++i] - 'a' ]; // restart hash for next keyword or null
			}
			else
			{
				int j = (hash ^ (sz[i] - 'a')) % TableSize;
				hash = mixTable[ j ];
				usedCode[ j ] = 1;
			}
		}

        // if any key-hashes have been used more than once, there is a collision
        bool collision = false;
        for(int i=0;i<TableSize;i++) { collision |= (usedKeyHash[i] > 1); }
		if( !collision ) { break; }

        // mark unused codes
        for( size_t i=0;i<TableSize;i++ ) { notUsedCode[i] = usedCode[i] ? 0 : 1; }

		// calc sum and swap some of the most used with some of the least used
		int sumUsed = sum<TableType,TableSize>(usedCode);
        int sumNotUsed = sum<TableType,TableSize>(notUsedCode);

        // if all chars are used, there isnt a way we can rearrange the mixing table
        if( sumNotUsed == 0 ) { iterations = MAXITER; break; }

        for( int k=0;k<(1+avgKeyLength);k++)
        {
            // select which to swap
            int i = sample<TableType>(usedCode, sumUsed);
            int j = sample<TableType>(notUsedCode, sumNotUsed);

            // disqualify items from ever being selected again and swap
            sumUsed -= usedCode[i] + usedCode[j]; // reduce sum
            sumNotUsed -= notUsedCode[i] + notUsedCode[j]; // reduce sum
            usedCode[i] = notUsedCode[i] = usedCode[j] = notUsedCode[j] = 0; // clear summable values
            std::swap(mixTable[i], mixTable[j]);

            if(sumUsed < 1 || sumNotUsed < 1) break;
        }

	} while( ++iterations < MAXITER );

	if( iterations < MAXITER )
		{ printf("uint16_t mix[] = {"); for( size_t i=0;i<TableSize;i++ ) { printf("%d, ",mixTable[i]); } printf("};\n"); }
	else
		{ printf("ack! can't do it.\n"); }
	
	return iterations;
}

int main()
{
	time_t ltime;
	time(&ltime);
	srand((unsigned int)ltime);

    const int keyCount = 10;
    const int keyLength = 10;
    const int tableSize = 256;
    using tableType = uint8_t;
    printf("Generating %d keys of length %d. Hashing into %d element table\n", keyCount, keyLength, tableSize);

	char buf[100000];
	for( size_t k=0;k<10;k++)
	{
		size_t pos = 0;
		for( size_t i=0;i<keyCount;i++)
		{
			// keys are all ascii lowsercase
			for( size_t j=0;j<keyLength;j++) { buf[ pos++ ] = 'a' + rand() % 26; }

			// keys end with special '!' char
			buf[ pos++ ] = '!';
		}
		buf[ pos ] = 0; // zero term buffer

        printf("Finding mix table for %s:\n", buf);
		size_t iter = calcMixTable<tableType,tableSize>( buf );
		printf("Took %lu iterations\n",iter);
	}

	return 0;
}
