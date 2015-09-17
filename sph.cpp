#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

// stochasticPerfectHash
// Uses stochastic remainder sampling to derive a Pearson mixing table for small sets of alphabetic keys
// orthopteroid@gmail.com
// g++ -std=c++0x -O3 -lrt sph.cpp -o sph
//
// for background, read:
// http://cs.mwsu.edu/~griffin/courses/2133/downloads/Spring11/p677-pearson.pdf

// stochastic remainder sampler
size_t sample( uint8_t* counts, uint32_t sum )
{
	int32_t target = rand() % sum;
	size_t i = 0;
	while( (target -= counts[ i ]) > 0 ) { i++; }
	return i;
}

// calculates a mixtable for the pearson hash to create a perfect hash.
// restrictions are keys can only be ascii lowercase, but can be of any length.
// keys are seperated in the input string using the ! delimiter. ! must also be at end of string.
size_t calcMixTable( char* sz )
{
	const size_t MAXITER = 25000;

	// special marker '!' in the string designates the end of a key
	size_t keyCount = 0;
	for( size_t i=0;sz[i];i++ ) { if( sz[i] == '!' ) { keyCount++; } }
	bool hashed[ keyCount ];

	// the defualt mix table is a null transform
	uint8_t mix[32] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

	// these weights influence convergence rate.
	// they are used by the stochastic sampling method that adjusts the mixing table to resolve hash collisions
	const uint8_t DEFAULT = 1;
	const uint8_t SIGNAL = 100;
	
	// reset flags...
	size_t iterations = 0;
	do {
		// counts will track mix table usage each iteration we evaluate the hashed
		uint8_t counts[32];
		
		for( size_t i=0;i<32;i++ ) { counts[i] = DEFAULT; }
		for( size_t i=0;i<keyCount;i++ ) { hashed[i] = false; }

		// compute the pearson hash using the current mix table.
		uint8_t hash = 0;
		bool collision = false;
		for( size_t i=0;sz[i];i++ )
		{
			if( sz[i] == '!' ) // end of key detected
			{
				size_t j = hash % keyCount; // mod with keyCount as we are trying to find the perfect hash function;
				if( hashed[ j ] ) { collision = true; }
				hashed[ j ] = true;
				hash = 0;
			}
			else
			{
				uint8_t j = 31 & ( hash ^ ( sz[i] - 'a' ) );
				hash = mix[ j ];
				counts[ j ] = SIGNAL; // inc the mix item that was used to compute the hashs this iteration.
			}
		}
		if( !collision ) { break; }
		
		// calc sum and swap items
		uint32_t countsum = 0;
		for( size_t i=0;i<32;i++ ) { countsum += counts[i]; }
		
		size_t i = sample( counts, countsum );				// select item i.
		countsum -= counts[i]; counts[i] = 0;				// remove from possible selection set
		size_t j = sample( counts, countsum );				// select item j
		uint8_t t = mix[i]; mix[i] = mix[j]; mix[j] = t;	// swap i and j

	} while( ++iterations < MAXITER );

	if( iterations < MAXITER )
		{ printf("uint_8 mix[] = {"); for( size_t i=0;i<32;i++ ) { printf("%d, ",mix[i]); } printf("};\n"); }
	else
		{ printf("ack! can't do it.\n"); }
	
	return iterations;
}

int main()
{
	time_t ltime;
	time(&ltime);
	srand((unsigned int)ltime);

	char buf[1000]; // a fixed buffer for a sequence of keys we wish to perfect-hash
	for( size_t k=0;k<10;k++)
	{
		const size_t keyCount = 10;
		size_t pos = 0;
		for( size_t i=0;i<keyCount;i++)
		{
			// keys are all ascii lowsercase
			const size_t keyLength = 10;
			for( size_t j=0;j<keyLength;j++) { buf[ pos++ ] = 'a' + rand() % 26; }

			// keys end with special '!' char
			buf[ pos++ ] = '!';
		}
		buf[ pos ] = 0; // zero term buffer

		printf("Finding mix table for %s:\n", buf);
		size_t iter = calcMixTable( buf );
		printf("Took %lu iterations\n",iter);
	}


	return 0;
}
