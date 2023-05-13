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

 **Validation** The sample code saves pre-masked 8bit hash which could be used for keyword validation if it wasn't so short - if you want to
 do that, use a 32bit hash method and store the hash to validate the keyword after lookup.
 
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
pearson solution for 21 keywords using a 32 keyword table (8628 iterations)
const char* Keyword[ 32 ] = { "repeat", "return", 0, 0, "if", "while", "for", "elseif", "nil", "in", 0, "or", 0, 0, "function", "local", "break", "do", 0, 0, 0, "false", "then", "true", 0, 0, "until", "and", "else", "not", 0, "end", };
enum Key { REPEAT=0, RETURN=1, IF=4, WHILE=5, FOR=6, ELSEIF=7, NIL=8, IN=9, OR=11, FUNCTION=14, LOCAL=15, BREAK=16, DO=17, FALSE=21, THEN=22, TRUE=23, UNTIL=26, AND=27, ELSE=28, NOT=29, END=31, };
const uint8_t Hash[ 32 ] = { 0x00, 0x61, 0x00, 0x00, 0x64, 0x05, 0x46, 0x87, 0xE8, 0xE9, 0x00, 0xEB, 0x00, 0x00, 0x8E, 0x4F, 0x30, 0xF1, 0x00, 0x00, 0x00, 0x55, 0xB6, 0x97, 0x00, 0x00, 0xBA, 0x1B, 0x1C, 0x5D, 0x00, 0xFF, };
uint8_t Mixtable[ 32 ] = { 0x0C, 0x08, 0x1D, 0x7A, 0xCC, 0xEC, 0xAD, 0x58, 0xC5, 0x36, 0x37, 0xE0, 0x01, 0x69, 0x20, 0xC2, 0x61, 0xF9, 0xD6, 0xE2, 0x40, 0x12, 0xF8, 0xE4, 0x8F, 0x49, 0x0E, 0xB1, 0x4F, 0x70, 0x3A, 0x84, };

crc32 solution for 21 keywords using a 32 keyword table (2615 iterations)
const char* Keyword[ 32 ] = { 0, "function", 0, "false", "return", "if", 0, "for", 0, "while", "end", "local", "and", "else", "repeat", 0, "elseif", "do", "break", 0, 0, "in", "nil", 0, "not", "until", 0, "true", 0, "or", 0, "then", };
enum Key { FUNCTION=1, FALSE=3, RETURN=4, IF=5, FOR=7, WHILE=9, END=10, LOCAL=11, AND=12, ELSE=13, REPEAT=14, ELSEIF=16, DO=17, BREAK=18, IN=21, NIL=22, NOT=24, UNTIL=25, TRUE=27, OR=29, THEN=31, };
const uint8_t Hash[ 32 ] = { 0x00, 0xA1, 0x00, 0x03, 0x04, 0x85, 0x00, 0x27, 0x00, 0xE9, 0x2A, 0x8B, 0xCC, 0xCD, 0xAE, 0x00, 0xF0, 0xF1, 0x12, 0x00, 0x00, 0xD5, 0xD6, 0x00, 0x78, 0x19, 0x00, 0x3B, 0x00, 0xBD, 0x00, 0xBF, };
uint8_t Mixtable[ 32 ] = { 0x52, 0x61, 0xC3, 0x93, 0xA2, 0x56, 0xC0, 0x73, 0x23, 0x51, 0xB0, 0x0E, 0xA3, 0xD0, 0x6A, 0x7F, 0x57, 0x13, 0x92, 0xFC, 0x5A, 0xF9, 0x78, 0x9E, 0xB8, 0x4F, 0x83, 0x30, 0xCC, 0x5D, 0xEF, 0x71, };

fnv1a32 solution for 21 keywords using a 32 keyword table (16500 iterations)
const char* Keyword[ 32 ] = { 0, "or", "if", "function", "and", 0, "true", "then", "elseif", "until", 0, 0, "not", "in", "else", 0, "nil", 0, "while", "do", "repeat", "break", "end", 0, 0, "false", 0, 0, 0, "return", "for", "local", };
enum Key { OR=1, IF=2, FUNCTION=3, AND=4, TRUE=6, THEN=7, ELSEIF=8, UNTIL=9, NOT=12, IN=13, ELSE=14, NIL=16, WHILE=18, DO=19, REPEAT=20, BREAK=21, END=22, FALSE=25, RETURN=29, FOR=30, LOCAL=31, };
const uint8_t Hash[ 32 ] = { 0x00, 0x61, 0x22, 0x03, 0xA4, 0x00, 0x06, 0x07, 0x08, 0x69, 0x00, 0x00, 0x2C, 0x8D, 0x6E, 0x00, 0x70, 0x00, 0xD2, 0x93, 0xB4, 0xB5, 0x96, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0xBD, 0x5E, 0x5F, };
uint8_t Mixtable[ 32 ] = { 0xC8, 0x6C, 0x1F, 0xB5, 0x74, 0x3D, 0x19, 0xC6, 0xDE, 0xA4, 0x3B, 0xFC, 0xFD, 0x33, 0x8A, 0x00, 0x9C, 0x98, 0xEA, 0xEB, 0x5E, 0xE6, 0x4D, 0x40, 0xA9, 0xD0, 0x3E, 0x4E, 0x9E, 0x54, 0xAA, 0xEE, };

fnv1a64 solution for 21 keywords using a 32 keyword table (8285 iterations)
const char* Keyword[ 32 ] = { 0, "end", 0, 0, "local", "while", 0, 0, "and", "if", 0, "do", "false", "true", "or", "then", "not", 0, "in", "else", 0, 0, "repeat", "for", "return", "function", 0, "break", 0, "elseif", "nil", "until", };
enum Key { END=1, LOCAL=4, WHILE=5, AND=8, IF=9, DO=11, FALSE=12, TRUE=13, OR=14, THEN=15, NOT=16, IN=18, ELSE=19, REPEAT=22, FOR=23, RETURN=24, FUNCTION=25, BREAK=27, ELSEIF=29, NIL=30, UNTIL=31, };
const uint8_t Hash[ 32 ] = { 0x00, 0xC1, 0x00, 0x00, 0xA4, 0x85, 0x00, 0x00, 0x68, 0xA9, 0x00, 0x8B, 0xEC, 0x2D, 0x4E, 0x0F, 0x10, 0x00, 0xD2, 0x33, 0x00, 0x00, 0x96, 0x17, 0xB8, 0x59, 0x00, 0x5B, 0x00, 0x3D, 0xBE, 0xDF, };
uint8_t Mixtable[ 32 ] = { 0x5F, 0xBC, 0x1E, 0x40, 0x94, 0x73, 0x00, 0xF3, 0x90, 0x95, 0xB2, 0x79, 0x78, 0x28, 0x32, 0x18, 0x03, 0x9E, 0x9D, 0x01, 0xA6, 0x6D, 0x4B, 0x8D, 0x52, 0xCA, 0x15, 0x17, 0xBF, 0x14, 0x25, 0x19, };
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
