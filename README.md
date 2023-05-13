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
 the keywords to hash into ordinals in the keyword table.  It tries a combination of approaches:
1. firstly, it proceeds to randomize the order of the mixing table (up to some maximum number of times). This permutes
 the calculation of the key-hash, which depends upon the hashing method under analysis (pearson, crc32, fnv1a32 or fnv1a64).
2. then, it adds extra blank entries (sparsity) in the keyword table. This effectively increases the bucket-size of the
 hash table. It's assumed that increasing this table is more costly memory-wise as these might be pointers, whereas
 the mixing table is uint8s.

 The pearson table approach looks up keyword characters in the mixing table to compute a hash (and subsequently using
 mod to determine a unique keyword index) whereas the crc32 method
 computes the crc32 hash of a keyword and then gobbles the bits of the crc to create a secondary hash (to which mod is
 used to determine the unique keyword index). For both methods only the final (8 bit) hash is stored, it's probably not
 even necessary to keep unless you want to do some kind of keyword hash validation (in which case there may be other
 problems with the current approach). The fnv1a hashes use the same approach as the crc32 method to determine a keyword index.

The output typically looks like:
```
pearson solution for 10 keywords using a 10 keyword table (1042 iterations)
const char* Keyword[ 10 ] = { "cmurpbw", "rdssh", "napygiu", "kgkmvhg", "iesdvh", "wqzecqx", "gquyg", "sqpkqqa", "objaqeh", "owzrrr", };
enum Key { CMURPBW=0, RDSSH=1, NAPYGIU=2, KGKMVHG=3, IESDVH=4, WQZECQX=5, GQUYG=6, SQPKQQA=7, OBJAQEH=8, OWZRRR=9, };
const uint8_t Hash[ 10 ] = { 0xE6, 0xB5, 0x5C, 0xE9, 0x7C, 0xC3, 0x92, 0xF7, 0x08, 0xA9, };
uint8_t Mixtable[ 32 ] = { 0x76, 0xCB, 0x86, 0xDC, 0x39, 0x2D, 0x8A, 0x5A, 0x33, 0x77, 0x6E, 0x7D, 0x4B, 0x44, 0x24, 0x9A, 0x5C, 0x50, 0x40, 0xB2, 0x60, 0x74, 0x61, 0x6F, 0x41, 0x49, 0xE3, 0x84, 0x73, 0xD2, 0x32, 0xB7, };

crc32 solution for 10 keywords using a 10 keyword table (3447 iterations)
const char* Keyword[ 10 ] = { "cmurpbw", "objaqeh", "iesdvh", "sqpkqqa", "wqzecqx", "gquyg", "owzrrr", "napygiu", "kgkmvhg", "rdssh", };
enum Key { CMURPBW=0, OBJAQEH=1, IESDVH=2, SQPKQQA=3, WQZECQX=4, GQUYG=5, OWZRRR=6, NAPYGIU=7, KGKMVHG=8, RDSSH=9, };
const uint8_t Hash[ 10 ] = { 0x14, 0x65, 0x16, 0x0D, 0x54, 0xD7, 0xE2, 0x61, 0xB2, 0xEF, };
uint8_t Mixtable[ 32 ] = { 0xDD, 0x38, 0xFA, 0xAE, 0x12, 0xFF, 0x18, 0x3D, 0xDF, 0x13, 0x74, 0x71, 0x8A, 0xFB, 0x6B, 0x20, 0x41, 0xF5, 0xA6, 0x35, 0x46, 0x86, 0x50, 0xE9, 0xB6, 0x79, 0x36, 0xD5, 0x40, 0x2B, 0x93, 0xAA, };
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

pearson solution for 20 keywords using a 26 keyword table (18464 iterations)
const char* Keyword[ 26 ] = { "gdbaygtfmflzgdli", "hqynlyppyykwsxldv", "fnnaadgmsmqplpr", "hwhxpdxblxkydhw", "fvzulkcsteqhemflz", "zglihetxcdygvxu", "xlpopmrclbaqkzmpu", "vvhgsptmqgmpebx", 0, "evoimigztlovibe", 0, "xckobcoqjghicymzt", "kdgxnaexgqccusy", "qtldlhkjolsnvsnvd", "vwqvysepswmkepavl", "mbwuqohgguvzrdus", "xlphufvpusjipmtz", "sjxhnpwfygbhxkx", "xjwpaunkshhekfw", 0, "nlyenlfpesoqhiti", 0, "swdjqxsypgsqxtp", 0, "zygcediwfnszlaywp", 0, };
enum Key { GDBAYGTFMFLZGDLI=0, HQYNLYPPYYKWSXLDV=1, FNNAADGMSMQPLPR=2, HWHXPDXBLXKYDHW=3, FVZULKCSTEQHEMFLZ=4, ZGLIHETXCDYGVXU=5, XLPOPMRCLBAQKZMPU=6, VVHGSPTMQGMPEBX=7, EVOIMIGZTLOVIBE=9, XCKOBCOQJGHICYMZT=11, KDGXNAEXGQCCUSY=12, QTLDLHKJOLSNVSNVD=13, VWQVYSEPSWMKEPAVL=14, MBWUQOHGGUVZRDUS=15, XLPHUFVPUSJIPMTZ=16, SJXHNPWFYGBHXKX=17, XJWPAUNKSHHEKFW=18, NLYENLFPESOQHITI=20, SWDJQXSYPGSQXTP=22, ZYGCEDIWFNSZLAYWP=24, };
const uint8_t Hash[ 26 ] = { 0x4E, 0x1B, 0x6A, 0x37, 0x86, 0x05, 0x54, 0x89, 0x00, 0xBF, 0x00, 0x73, 0xF6, 0xC3, 0xAA, 0x91, 0x44, 0x2B, 0x60, 0x00, 0x7C, 0x00, 0x64, 0x00, 0x80, 0x00, };
uint8_t Mixtable[ 32 ] = { 0x72, 0x13, 0x3A, 0xC6, 0x03, 0x3E, 0xBD, 0x9A, 0xFC, 0xF0, 0x2C, 0x0F, 0xCA, 0x69, 0x49, 0x87, 0x14, 0xC0, 0x6C, 0xD7, 0x6E, 0x5E, 0x02, 0xBF, 0xF7, 0x28, 0xDD, 0xAC, 0x3D, 0xB5, 0x2B, 0x62, };
```

Here are several solutions using different hashes on lua's 21 keywords:
```
pearson solution for 21 keywords using a 25 keyword table (1841 iterations)
const char* Keyword[ 25 ] = { "while", "and", 0, "else", "not", "function", "false", "for", "do", "return", "true", 0, "local", 0, "repeat", "break", "or", "in", "if", 0, "elseif", "then", "until", "end", "nil", };
enum Key { WHILE=0, AND=1, ELSE=3, NOT=4, FUNCTION=5, FALSE=6, FOR=7, DO=8, RETURN=9, TRUE=10, LOCAL=12, REPEAT=14, BREAK=15, OR=16, IN=17, IF=18, ELSEIF=20, THEN=21, UNTIL=22, END=23, NIL=24, };
const uint8_t Hash[ 25 ] = { 0xC8, 0xE2, 0x00, 0xFD, 0x04, 0xCD, 0x06, 0x84, 0x08, 0xB8, 0xD2, 0x00, 0x57, 0x00, 0x27, 0xD7, 0x42, 0xD9, 0xA8, 0x00, 0xAA, 0x15, 0x93, 0xDF, 0x63, };
uint8_t Mixtable[ 32 ] = { 0xE1, 0x3D, 0x57, 0xF5, 0x97, 0x00, 0xC6, 0x24, 0x71, 0x91, 0x11, 0x9F, 0xBA, 0x4C, 0xB7, 0x60, 0x14, 0x92, 0xDD, 0xB8, 0x2C, 0xDC, 0x76, 0x6D, 0x9C, 0x15, 0x85, 0xFB, 0x5B, 0x13, 0xB5, 0x6B, };

crc32 solution for 21 keywords using a 28 keyword table (8457 iterations)
const char* Keyword[ 28 ] = { 0, "true", 0, "false", 0, "while", "if", "repeat", "end", "until", 0, "function", "break", "return", "do", "elseif", "not", 0, 0, "nil", "for", "then", "or", "else", 0, "and", "in", "local", };
enum Key { TRUE=1, FALSE=3, WHILE=5, IF=6, REPEAT=7, END=8, UNTIL=9, FUNCTION=11, BREAK=12, RETURN=13, DO=14, ELSEIF=15, NOT=16, NIL=19, FOR=20, THEN=21, OR=22, ELSE=23, AND=25, IN=26, LOCAL=27, };
const uint8_t Hash[ 28 ] = { 0x00, 0x01, 0x00, 0xAB, 0x00, 0x21, 0xAE, 0x3F, 0x5C, 0x79, 0x00, 0x43, 0xB4, 0x29, 0x9A, 0x9B, 0x48, 0x00, 0x00, 0x2F, 0x68, 0xD9, 0x32, 0x17, 0x00, 0x89, 0x1A, 0x1B, };
uint8_t Mixtable[ 32 ] = { 0xD0, 0x0B, 0x94, 0x14, 0xA6, 0x65, 0xBB, 0xB8, 0x77, 0x46, 0x2D, 0xBC, 0x9B, 0x3E, 0xA1, 0x27, 0xDC, 0x8D, 0xF1, 0x9A, 0x6D, 0x04, 0x8C, 0xBF, 0xE2, 0xBD, 0x83, 0x9E, 0x78, 0x8F, 0xCF, 0x3A, };

fnv1a32 solution for 21 keywords using a 26 keyword table (13692 iterations)
const char* Keyword[ 26 ] = { "end", "nil", 0, "false", 0, 0, "if", "elseif", "not", "for", "return", "local", "then", "else", "and", "or", "do", "function", "in", 0, "until", "true", "break", "repeat", "while", 0, };
enum Key { END=0, NIL=1, FALSE=3, IF=6, ELSEIF=7, NOT=8, FOR=9, RETURN=10, LOCAL=11, THEN=12, ELSE=13, AND=14, OR=15, DO=16, FUNCTION=17, IN=18, UNTIL=20, TRUE=21, BREAK=22, REPEAT=23, WHILE=24, };
const uint8_t Hash[ 26 ] = { 0xB6, 0xEB, 0x00, 0xB9, 0x00, 0x00, 0xA2, 0x21, 0xA4, 0x8B, 0xDA, 0x73, 0x74, 0x41, 0xC4, 0x77, 0x10, 0x93, 0xE2, 0x00, 0xFE, 0xCB, 0xCC, 0x7F, 0xB4, 0x00, };
uint8_t Mixtable[ 32 ] = { 0xC6, 0x5B, 0xBA, 0x3F, 0xAB, 0x2D, 0x94, 0x73, 0xBE, 0xAD, 0x45, 0x2E, 0xE8, 0x4C, 0x1B, 0x92, 0x4D, 0x25, 0xFF, 0x8F, 0xBD, 0x1E, 0x59, 0xE4, 0x44, 0x17, 0x11, 0xBB, 0x02, 0x46, 0x86, 0x54, };

fnv1a64 solution for 21 keywords using a 27 keyword table (15047 iterations)
const char* Keyword[ 27 ] = { "while", "end", "nil", "function", 0, "repeat", 0, "in", "true", "if", "and", "local", 0, "until", "for", "not", 0, "else", "or", "false", 0, "break", "elseif", "then", "return", "do", 0, };
enum Key { WHILE=0, END=1, NIL=2, FUNCTION=3, REPEAT=5, IN=7, TRUE=8, IF=9, AND=10, LOCAL=11, UNTIL=13, FOR=14, NOT=15, ELSE=17, OR=18, FALSE=19, BREAK=21, ELSEIF=22, THEN=23, RETURN=24, DO=25, };
const uint8_t Hash[ 27 ] = { 0x1B, 0x52, 0x89, 0xC0, 0x00, 0xF8, 0x00, 0x58, 0x23, 0x24, 0x91, 0x5C, 0x00, 0x94, 0x44, 0x2A, 0x00, 0xCE, 0x12, 0x64, 0x00, 0x30, 0x4C, 0xEF, 0x33, 0x4F, 0x00, };
uint8_t Mixtable[ 32 ] = { 0xC7, 0xDF, 0x87, 0x48, 0xB4, 0xA4, 0x47, 0xBE, 0xFF, 0xF6, 0x80, 0x4F, 0x13, 0xAF, 0x39, 0x4D, 0xB6, 0xFD, 0x5C, 0xF3, 0xAB, 0x2A, 0x21, 0xCA, 0x12, 0x3C, 0x65, 0x95, 0x77, 0xE2, 0x91, 0x45, };
```

