# stochasticPerfectHash
Uses randomization to derive a mixing table on hashes for small sets of alphabetic keys. These tables might be
 useful for small parsers when parsing text identifiers - possibly for a communications protocol or a custom
 SAX-like xml parser. The code illustrates this process for the [Pearson hash](http://cs.mwsu.edu/~griffin/courses/2133/downloads/Spring11/p677-pearson.pdf),
[crc32 hash](http://web.mit.edu/freebsd/head/sys/libkern/crc32.c) and the [fnv1a 32bit and 64bit hashes](https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function).

This is a small app that can run as a demo with randomly generated keywords or by specifying a language file.
 If you compile and run it without any args you will see it
 try to create mixing tables for randomly generated keys - all lower case alpha.
 When run, the program generates N random keys and tries to find the keyword arrangement and character mixing
 table permutation that allows
 the keywords to hash into ordinals in the keyword table.

**Approach** It tries a combination of approaches:
1. firstly, it proceeds to randomize the order of the mixing table (up to some maximum number of times). This permutes
 the calculation of the key-hash, which depends upon the hashing method under analysis (pearson, crc32, fnv1a32 or fnv1a64).
2. then, it adds extra blank entries (sparsity) in the keyword table. This effectively increases the bucket-size of the
 hash table. It's assumed that increasing this table is more costly memory-wise as these might be pointers, whereas
 the mixing table is uint8s.

 **Secondary Hash** The pearson table approach looks up keyword characters in the mixing table to compute a hash (and subsequently using
 mod to determine a unique keyword index) whereas the crc32 method
 computes the crc32 hash of a keyword and then gobbles the bits of the crc to create a secondary hash (to which a mask or mod is
 used to determine the unique keyword index). The fnv1a hashes use the same approach as the crc32 method to determine a keyword
 index. Other hashes could be added.

 **UTF-8** This approach has been applied to strings made from 8-bit letters. To hash variable-length
character encodings (like utf8) with the Pearson would require the mixtable to be the
full 256 entries. Alternatively, hashing the utf-8 string with crc32 (or the other longer hashes) and
bit-gobbling the hash through the mix-table might be effective.

 **Validation** The code saves the full-width hash which could be used for keyword validation after the key is computed. Using a longer hash
like the 64 bit or 32 bit ones are probably better for this purpose.
 
The output typically looks like:
```
Pearson8x8 solution for 10 keywords using a 10 keyword table (1042 iterations)
const char* Keyword[ 10 ] = { "cmurpbw", "rdssh", "napygiu", "kgkmvhg", "iesdvh", "wqzecqx", "gquyg", "sqpkqqa", "objaqeh", "owzrrr", };
enum Key { CMURPBW=0, RDSSH=1, NAPYGIU=2, KGKMVHG=3, IESDVH=4, WQZECQX=5, GQUYG=6, SQPKQQA=7, OBJAQEH=8, OWZRRR=9, };
const uint8_t Hash[ 10 ] = { 0xE6, 0xB5, 0x5C, 0xE9, 0x7C, 0xC3, 0x92, 0xF7, 0x08, 0xA9, };
uint8_t Mixtable[ 32 ] = { 0x76, 0xCB, 0x86, 0xDC, 0x39, 0x2D, 0x8A, 0x5A, 0x33, 0x77, 0x6E, 0x7D, 0x4B, 0x44, 0x24, 0x9A, 0x5C, 0x50, 0x40, 0xB2, 0x60, 0x74, 0x61, 0x6F, 0x41, 0x49, 0xE3, 0x84, 0x73, 0xD2, 0x32, 0xB7, };
```

It's unusual with smaller keyword sizes and key lengths but sometimes the program can't find a combination 
 of mixtable and keyword-table size that perfectly hashes the keywords:
```
Keywords (fuj, rhilz, crwdvmw, fyawb, qqxyxvh, rqkk, uwyt, ppf, dcv, cxgdzz, ).
Failed.
```

For more and larger keywords more 'holes' in the keyword-table may be apparent. These holes are for ordinals that don't
 map to any keyword and only serve to make a size-combination of the mixtable and keyword-table that creates a
 (close) perfect hash (and the program requires more iterations):
```
Keywords (vwqvysepswmkepavl, hwhxpdxblxkydhw, evoimigztlovibe, xlpopmrclbaqkzmpu, zglihetxcdygvxu, nlyenlfpesoqhiti, hqynlyppyykwsxldv, sjxhnpwfygbhxkx, xjwpaunkshhekfw, qtldlhkjolsnvsnvd, kdgxnaexgqccusy, xlphufvpusjipmtz, zygcediwfnszlaywp, swdjqxsypgsqxtp, vvhgsptmqgmpebx, xckobcoqjghicymzt, fnnaadgmsmqplpr, fvzulkcsteqhemflz, mbwuqohgguvzrdus, gdbaygtfmflzgdli, )

Pearson8x8 solution for 20 keywords using a 26 keyword table (18464 iterations)
const char* Keyword[ 26 ] = { "gdbaygtfmflzgdli", "hqynlyppyykwsxldv", "fnnaadgmsmqplpr", "hwhxpdxblxkydhw", "fvzulkcsteqhemflz", "zglihetxcdygvxu", "xlpopmrclbaqkzmpu", "vvhgsptmqgmpebx", 0, "evoimigztlovibe", 0, "xckobcoqjghicymzt", "kdgxnaexgqccusy", "qtldlhkjolsnvsnvd", "vwqvysepswmkepavl", "mbwuqohgguvzrdus", "xlphufvpusjipmtz", "sjxhnpwfygbhxkx", "xjwpaunkshhekfw", 0, "nlyenlfpesoqhiti", 0, "swdjqxsypgsqxtp", 0, "zygcediwfnszlaywp", 0, };
enum Key { GDBAYGTFMFLZGDLI=0, HQYNLYPPYYKWSXLDV=1, FNNAADGMSMQPLPR=2, HWHXPDXBLXKYDHW=3, FVZULKCSTEQHEMFLZ=4, ZGLIHETXCDYGVXU=5, XLPOPMRCLBAQKZMPU=6, VVHGSPTMQGMPEBX=7, EVOIMIGZTLOVIBE=9, XCKOBCOQJGHICYMZT=11, KDGXNAEXGQCCUSY=12, QTLDLHKJOLSNVSNVD=13, VWQVYSEPSWMKEPAVL=14, MBWUQOHGGUVZRDUS=15, XLPHUFVPUSJIPMTZ=16, SJXHNPWFYGBHXKX=17, XJWPAUNKSHHEKFW=18, NLYENLFPESOQHITI=20, SWDJQXSYPGSQXTP=22, ZYGCEDIWFNSZLAYWP=24, };
const uint8_t Hash[ 26 ] = { 0x4E, 0x1B, 0x6A, 0x37, 0x86, 0x05, 0x54, 0x89, 0x00, 0xBF, 0x00, 0x73, 0xF6, 0xC3, 0xAA, 0x91, 0x44, 0x2B, 0x60, 0x00, 0x7C, 0x00, 0x64, 0x00, 0x80, 0x00, };
uint8_t Mixtable[ 32 ] = { 0x72, 0x13, 0x3A, 0xC6, 0x03, 0x3E, 0xBD, 0x9A, 0xFC, 0xF0, 0x2C, 0x0F, 0xCA, 0x69, 0x49, 0x87, 0x14, 0xC0, 0x6C, 0xD7, 0x6E, 0x5E, 0x02, 0xBF, 0xF7, 0x28, 0xDD, 0xAC, 0x3D, 0xB5, 0x2B, 0x62, };
```

Here are several solutions using different hashes on lua's 21 keywords:
```
Pearson8x8 solution for 21 keywords using a 32 keyword table (3312 iterations)
const char* Keyword[ 32 ] = { 0, "in", "do", "local", "repeat", "if", "break", 0, 0, 0, "true", 0, 0, "elseif", "and", "or", 0, "function", "false", "until", 0, 0, "return", "else", "then", "end", 0, "nil", 0, "for", "not", "while", };
enum Key { IN=1, DO=2, LOCAL=3, REPEAT=4, IF=5, BREAK=6, TRUE=10, ELSEIF=13, AND=14, OR=15, FUNCTION=17, FALSE=18, UNTIL=19, RETURN=22, ELSE=23, THEN=24, END=25, NIL=27, FOR=29, NOT=30, WHILE=31, };
const uint8_t Hash[ 32 ] = { 0x0, 0x61, 0xe2, 0x23, 0x64, 0xc5, 0xc6, 0x0, 0x0, 0x0, 0x8a, 0x0, 0x0, 0xd, 0xe, 0x4f, 0x0, 0x51, 0xd2, 0x73, 0x0, 0x0, 0xf6, 0x37, 0x18, 0x99, 0x0, 0x1b, 0x0, 0x3d, 0x5e, 0x9f, };
const uint8_t Mixtable[ 32 ] = { 0xd0, 0x20, 0x5d, 0xe6, 0x7, 0xb7, 0x72, 0x24, 0xeb, 0x48, 0xe9, 0x59, 0x7a, 0x28, 0xd6, 0x1a, 0x7c, 0x89, 0xaa, 0xb2, 0x6d, 0x5, 0x8c, 0xe, 0x23, 0xc6, 0x6f, 0x56, 0x32, 0x71, 0x8f, 0x7f, };

CRC32x8 solution for 21 keywords using a 32 keyword table (14697 iterations)
const char* Keyword[ 32 ] = { "return", "function", "do", 0, "nil", "or", 0, "then", 0, "local", 0, "while", 0, "true", 0, 0, "break", "not", "in", 0, "else", "elseif", "end", "for", 0, "until", 0, "if", 0, "and", "repeat", "false", };
enum Key { RETURN=0, FUNCTION=1, DO=2, NIL=4, OR=5, THEN=7, LOCAL=9, WHILE=11, TRUE=13, BREAK=16, NOT=17, IN=18, ELSE=20, ELSEIF=21, END=22, FOR=23, UNTIL=25, IF=27, AND=29, REPEAT=30, FALSE=31, };
const uint32_t Hash[ 32 ] = { 0xa79e3f0f, 0xcaae163d, 0x9d45c095, 0x0, 0x4d33c2a5, 0x1db77587, 0x0, 0x315da1f2, 0x0, 0x8bd688e8, 0x0, 0xc0d455fd, 0x0, 0xfdfc4c8d, 0x0, 0x0, 0xc7c50da, 0x805fd75, 0x5fec8e4e, 0x0, 0x55bb558b, 0x9241b73c, 0xfc33b1, 0xef7509f8, 0x0, 0x8f2a2583, 0x0, 0x5137067c, 0x0, 0x7f59b6d, 0xa857b3c0, 0x2bcd6830, };
const uint8_t Mixtable[ 32 ] = { 0x23, 0xb3, 0xae, 0xc, 0xc7, 0x53, 0x36, 0xea, 0xd4, 0x20, 0x21, 0x7e, 0x2f, 0xa3, 0x4e, 0x3c, 0xe5, 0x8a, 0x68, 0x75, 0x56, 0xc8, 0x38, 0x6e, 0xa8, 0xb2, 0xa4, 0x47, 0xc1, 0x87, 0xa5, 0x3d, };

FNV1A32x8 solution for 21 keywords using a 32 keyword table (1678 iterations)
const char* Keyword[ 32 ] = { "end", "do", "break", "while", 0, 0, 0, 0, 0, "and", 0, "until", "if", "for", 0, "else", 0, "local", "not", "then", "repeat", 0, "nil", 0, "elseif", "or", "in", 0, "function", "true", "return", "false", };
enum Key { END=0, DO=1, BREAK=2, WHILE=3, AND=9, UNTIL=11, IF=12, FOR=13, ELSE=15, LOCAL=17, NOT=18, THEN=19, REPEAT=20, NIL=22, ELSEIF=24, OR=25, IN=26, FUNCTION=28, TRUE=29, RETURN=30, FALSE=31, };
const uint32_t Hash[ 32 ] = { 0x6a8e75aa, 0x621cd814, 0xc9648178, 0xdc628ce, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf29c2a6, 0x0, 0xbab5214f, 0x39386e06, 0xacf38390, 0x0, 0xbdbf5bf0, 0x0, 0x9c436708, 0x29b19c8a, 0xe522e976, 0xd99ba82a, 0x0, 0xda3f8ec, 0x0, 0x54554c87, 0x5d342984, 0x41387a9e, 0x0, 0x9ed64249, 0x4db211e5, 0x85ee37bf, 0xb069958, };
const uint8_t Mixtable[ 32 ] = { 0x89, 0x8e, 0x6b, 0x30, 0xd4, 0x7f, 0xd7, 0x19, 0xb7, 0xb1, 0x41, 0x1, 0x58, 0xbb, 0x69, 0x37, 0xfb, 0x32, 0xa9, 0x84, 0x22, 0x31, 0x49, 0x43, 0xf6, 0xd1, 0x74, 0xec, 0x33, 0x21, 0xcd, 0x5a, };

FNV1A64x8 solution for 21 keywords using a 32 keyword table (1837 iterations)
const char* Keyword[ 32 ] = { "or", "not", "break", 0, "false", 0, "and", "in", "end", 0, "true", "else", "function", "for", 0, "while", 0, "local", 0, "return", 0, "until", "repeat", "nil", 0, "do", "if", "then", "elseif", 0, 0, 0, };
enum Key { OR=0, NOT=1, BREAK=2, FALSE=4, AND=6, IN=7, END=8, TRUE=10, ELSE=11, FUNCTION=12, FOR=13, WHILE=15, LOCAL=17, RETURN=19, UNTIL=21, REPEAT=22, NIL=23, DO=25, IF=26, THEN=27, ELSEIF=28, };
const uint64_t Hash[ 32 ] = { 0x8b05407b5565ca4, 0x215ad619258e9f4a, 0x93b7591debc7ce38, 0x0, 0xb5fae2c14238b978, 0x0, 0xe6f79719051ff286, 0x8b73807b55c4bbe, 0xc2f00318f053500a, 0x0, 0x5b5c98ef514dbfa5, 0x7f2b6c605332dd30, 0x2284cbb38b9cf89, 0xdcb27818fed9da90, 0x0, 0xce87a3885811296e, 0x0, 0x249f1fb6f3a680e8, 0x0, 0xc5c7b983377cad5f, 0x0, 0x1fcc8b4e81fbac2f, 0x573bb1642d08280a, 0x2146ba19257dc6ac, 0x0, 0x8915907b53bb494, 0x8b73007b55c3e26, 0x2579c7ef323d0d96, 0x940b0399281d9807, 0x0, 0x0, 0x0, };
const uint8_t Mixtable[ 32 ] = { 0x27, 0xd2, 0x38, 0xb6, 0x34, 0x87, 0x76, 0x71, 0x49, 0x58, 0x9, 0x48, 0x97, 0xf, 0x31, 0x83, 0x6a, 0x43, 0x56, 0x80, 0x6d, 0x23, 0x42, 0x59, 0xc1, 0x77, 0xe4, 0xc9, 0x9a, 0x2a, 0xfc, 0x69, };

```

You can use the output of this program to make a fast scanner:

```
const char* Keyword[ 32 ] = { "repeat", "return", 0, 0, "if", "while", "for", "elseif", "nil", "in", 0, "or", 0, 0, "function", "local", "break", "do", 0, 0, 0, "false", "then", "true", 0, 0, "until", "and", "else", "not", 0, "end", };
enum Key { REPEAT=0, RETURN=1, IF=4, WHILE=5, FOR=6, ELSEIF=7, NIL=8, IN=9, OR=11, FUNCTION=14, LOCAL=15, BREAK=16, DO=17, FALSE=21, THEN=22, TRUE=23, UNTIL=26, AND=27, ELSE=28, NOT=29, END=31, };
const uint8_t Hash[ 32 ] = { 0x00, 0x61, 0x00, 0x00, 0x64, 0x05, 0x46, 0x87, 0xE8, 0xE9, 0x00, 0xEB, 0x00, 0x00, 0x8E, 0x4F, 0x30, 0xF1, 0x00, 0x00, 0x00, 0x55, 0xB6, 0x97, 0x00, 0x00, 0xBA, 0x1B, 0x1C, 0x5D, 0x00, 0xFF, };

uint8_t parse_keyword(const char *keyword)
{
  const uint8_t Mixtable[ 32 ] = { 0x0C, 0x08, 0x1D, 0x7A, 0xCC, 0xEC, 0xAD, 0x58, 0xC5, 0x36, 0x37, 0xE0, 0x01, 0x69, 0x20, 0xC2, 0x61, 0xF9, 0xD6, 0xE2, 0x40, 0x12, 0xF8, 0xE4, 0x8F, 0x49, 0x0E, 0xB1, 0x4F, 0x70, 0x3A, 0x84, };
  const uint8_t bitmask = 0b00011111;

  uint8_t hash = Mixtable[ Keyword[0] & bitmask ];
  for(uint i = 1; keyword[i]; i++)
    hash ^= Mixtable[ Keyword[i] & bitmask ];
        
  return hash & bitmask;
}
```
