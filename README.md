# stochasticPerfectHash
Uses randomization to derive a mixing table on hashes for small sets of alphabetic keys. These tables might be
 useful for small parsers when parsing text identifiers - possibly for a communications protocol or a custom
 SAX-like xml parser. The code illustrates this process for [Pearson hashing](http://cs.mwsu.edu/~griffin/courses/2133/downloads/Spring11/p677-pearson.pdf)
 and [crc32 hashing](http://web.mit.edu/freebsd/head/sys/libkern/crc32.c).

This is just a small demo app - it has no options or interface. If you compile and run it you will see it
 try to create mixing tables for 10 randomly generated keys - all lower case alpha.
 When run, the program generates N random keys and tries to find the keyword arrangement and character mixing
 table permutation that allows
 the keywords to hash into ordinals in the keyword table.  It tries a combination of approaches:
1. firstly, it proceeds to randomize the order of the mixing table (up to some maximum number of times). This permutes
 the calculation of the key-hash, which depends upon the hashing method under analysis (Pearson or crc32).
2. then, it adds extra blank entries (sparsity) in the keyword table. This effectively increases the bucket-size of the
 hash table. It's assumed that increasing this table is more costly memory-wise as these might be pointers, whereas
 the mixing table is uint8s.

 The pearson table approach looks up keyword characters in the mixing table to compute a hash (and subsequently using
 mod to determine a unique keyword index) whereas the crc32 method
 computes the crc32 hash of a keyword and then gobbles the bits of the crc to create a secondary hash (to which mod is
 used to determine the unique keyword index). For both methods only the final (8 bit) hash is stored, it's probably not
 even necessary to keep unless you want to do some kind of keyword hash validation (in which case there may be other
 problems with the current approach). It also seems that crc32 hashing produces shorted keyword tables. 

The output typically looks like:
```
Keywords (wqzecqx, cmurpbw, sqpkqqa, gquyg, rdssh, kgkmvhg, iesdvh, napygiu, owzrrr, objaqeh, )

Pearson solution for 10 keywords using a 10 keyword table (1042 iterations)
const char* Keyword[ 10 ] = { "cmurpbw", "rdssh", "napygiu", "kgkmvhg", "iesdvh", "wqzecqx", "gquyg", "sqpkqqa", "objaqeh", "owzrrr", };
enum Key { CMURPBW=0, RDSSH=1, NAPYGIU=2, KGKMVHG=3, IESDVH=4, WQZECQX=5, GQUYG=6, SQPKQQA=7, OBJAQEH=8, OWZRRR=9, };
const uint8_t Hash[ 10 ] = { 0xE6, 0xB5, 0x5C, 0xE9, 0x7C, 0xC3, 0x92, 0xF7, 0x08, 0xA9, };
uint8_t Mixtable[ 32 ] = { 0x76, 0xCB, 0x86, 0xDC, 0x39, 0x2D, 0x8A, 0x5A, 0x33, 0x77, 0x6E, 0x7D, 0x4B, 0x44, 0x24, 0x9A, 0x5C, 0x50, 0x40, 0xB2, 0x60, 0x74, 0x61, 0x6F, 0x41, 0x49, 0xE3, 0x84, 0x73, 0xD2, 0x32, 0xB7, };

CRC-32 solution for 10 keywords using a 10 keyword table (3447 iterations)
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

Pearson solution for 20 keywords using a 26 keyword table (18464 iterations)
const char* Keyword[ 26 ] = { "gdbaygtfmflzgdli", "hqynlyppyykwsxldv", "fnnaadgmsmqplpr", "hwhxpdxblxkydhw", "fvzulkcsteqhemflz", "zglihetxcdygvxu", "xlpopmrclbaqkzmpu", "vvhgsptmqgmpebx", 0, "evoimigztlovibe", 0, "xckobcoqjghicymzt", "kdgxnaexgqccusy", "qtldlhkjolsnvsnvd", "vwqvysepswmkepavl", "mbwuqohgguvzrdus", "xlphufvpusjipmtz", "sjxhnpwfygbhxkx", "xjwpaunkshhekfw", 0, "nlyenlfpesoqhiti", 0, "swdjqxsypgsqxtp", 0, "zygcediwfnszlaywp", 0, };
enum Key { GDBAYGTFMFLZGDLI=0, HQYNLYPPYYKWSXLDV=1, FNNAADGMSMQPLPR=2, HWHXPDXBLXKYDHW=3, FVZULKCSTEQHEMFLZ=4, ZGLIHETXCDYGVXU=5, XLPOPMRCLBAQKZMPU=6, VVHGSPTMQGMPEBX=7, EVOIMIGZTLOVIBE=9, XCKOBCOQJGHICYMZT=11, KDGXNAEXGQCCUSY=12, QTLDLHKJOLSNVSNVD=13, VWQVYSEPSWMKEPAVL=14, MBWUQOHGGUVZRDUS=15, XLPHUFVPUSJIPMTZ=16, SJXHNPWFYGBHXKX=17, XJWPAUNKSHHEKFW=18, NLYENLFPESOQHITI=20, SWDJQXSYPGSQXTP=22, ZYGCEDIWFNSZLAYWP=24, };
const uint8_t Hash[ 26 ] = { 0x4E, 0x1B, 0x6A, 0x37, 0x86, 0x05, 0x54, 0x89, 0x00, 0xBF, 0x00, 0x73, 0xF6, 0xC3, 0xAA, 0x91, 0x44, 0x2B, 0x60, 0x00, 0x7C, 0x00, 0x64, 0x00, 0x80, 0x00, };
uint8_t Mixtable[ 32 ] = { 0x72, 0x13, 0x3A, 0xC6, 0x03, 0x3E, 0xBD, 0x9A, 0xFC, 0xF0, 0x2C, 0x0F, 0xCA, 0x69, 0x49, 0x87, 0x14, 0xC0, 0x6C, 0xD7, 0x6E, 0x5E, 0x02, 0xBF, 0xF7, 0x28, 0xDD, 0xAC, 0x3D, 0xB5, 0x2B, 0x62, };

CRC-32 solution for 20 keywords using a 25 keyword table (23777 iterations)
const char* Keyword[ 25 ] = { "hwhxpdxblxkydhw", "qtldlhkjolsnvsnvd", "vvhgsptmqgmpebx", "vwqvysepswmkepavl", 0, "hqynlyppyykwsxldv", "gdbaygtfmflzgdli", 0, "fnnaadgmsmqplpr", "kdgxnaexgqccusy", "xjwpaunkshhekfw", "nlyenlfpesoqhiti", 0, "xlphufvpusjipmtz", "mbwuqohgguvzrdus", "swdjqxsypgsqxtp", "zygcediwfnszlaywp", "xckobcoqjghicymzt", 0, "sjxhnpwfygbhxkx", "evoimigztlovibe", "fvzulkcsteqhemflz", "xlpopmrclbaqkzmpu", "zglihetxcdygvxu", 0, };
enum Key { HWHXPDXBLXKYDHW=0, QTLDLHKJOLSNVSNVD=1, VVHGSPTMQGMPEBX=2, VWQVYSEPSWMKEPAVL=3, HQYNLYPPYYKWSXLDV=5, GDBAYGTFMFLZGDLI=6, FNNAADGMSMQPLPR=8, KDGXNAEXGQCCUSY=9, XJWPAUNKSHHEKFW=10, NLYENLFPESOQHITI=11, XLPHUFVPUSJIPMTZ=13, MBWUQOHGGUVZRDUS=14, SWDJQXSYPGSQXTP=15, ZYGCEDIWFNSZLAYWP=16, XCKOBCOQJGHICYMZT=17, SJXHNPWFYGBHXKX=19, EVOIMIGZTLOVIBE=20, FVZULKCSTEQHEMFLZ=21, XLPOPMRCLBAQKZMPU=22, ZGLIHETXCDYGVXU=23, };
const uint8_t Hash[ 25 ] = { 0xC8, 0x1A, 0xFC, 0xB2, 0x00, 0x69, 0xE7, 0x00, 0x08, 0xB8, 0xD2, 0xBA, 0x00, 0x3F, 0x59, 0x8C, 0xBF, 0x5C, 0x00, 0x90, 0x46, 0x47, 0x16, 0xF8, 0x00, };
uint8_t Mixtable[ 32 ] = { 0x14, 0xC0, 0x64, 0x79, 0x67, 0xE2, 0x3A, 0xD9, 0xFC, 0x5D, 0xB5, 0x57, 0xF3, 0x22, 0x30, 0xCB, 0x1F, 0xBB, 0xFF, 0xAF, 0xC5, 0x2A, 0x56, 0x4E, 0xD0, 0x8E, 0xD5, 0xEB, 0x99, 0xB6, 0xCE, 0x06, };
```

