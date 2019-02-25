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
 the calculation of the key-hash, which depends upon the hashing method undern analysis (Pearson or crc32).
2. then, it adds extra blank entries (sparsity) in the keyword table. This effectively increases the bucket-size of the
 hash table. It's assumed that increasing this table is more costly memory-wise as these might be pointers, whereas
 the mixing table is uint8s.

 It uses the same mixing table approach to
 derive keys using Pearson hashing and a crc32 hash of a keyword. The advantage of the crc32 hash would be for keyword validation
 as the Pearson hash only give an 8 bit crc which might be unsuitable for keyword validation in mixed or noisy channels. It
 also seems that crc32 hashing produces shorted keyword tables. 

The output typically looks like:
```
Keywords (speudvf, mpitgop, vcilh, gbmqwzu, lmwqjz, onzeytm, kzjmk, ttfuhwq, qxueu, nvzcl, )

Solution for 10 keywords using a 10 keyword table with a 64 slot mix table (396 iterations)
uint8_t Mixtable[ 64 ] = { 50, 12, 60, 40, 31, 9, 65, 5, 4, 34, 47, 10, 58, 54, 28, 63, 16, 11, 7, 45, 6, 21, 17, 35, 57, 3, 23, 56, 24, 14, 55, 42, 48, 25, 49, 26, 43, 30, 20, 13, 33, 52, 59, 8, 51, 44, 62, 64, 53, 38, 18, 22, 15, 27, 66, 61, 41, 32, 37, 36, 46, 39, 19, 29, };
const char* Keyword[ 10 ] = { "kzjmk", "mpitgop", "lmwqjz", "nvzcl", "vcilh", "speudvf", "gbmqwzu", "onzeytm", "qxueu", "ttfuhwq", };
const uint32_t Hash[ 10 ] = { 0x0000000A, 0x0000003D, 0x00000016, 0x00000035, 0x00000040, 0x00000005, 0x00000024, 0x00000039, 0x00000008, 0x0000001D, };
enum Key { KZJMK=0, MPITGOP=1, LMWQJZ=2, NVZCL=3, VCILH=4, SPEUDVF=5, GBMQWZU=6, ONZEYTM=7, QXUEU=8, TTFUHWQ=9, };

Solution for 10 keywords using a 11 keyword table with a 64 slot mix table (207 iterations)
uint8_t Mixtable[ 64 ] = { 15, 16, 38, 28, 41, 47, 55, 11, 53, 59, 5, 19, 23, 26, 51, 10, 43, 12, 48, 58, 40, 56, 45, 17, 24, 27, 25, 34, 7, 49, 4, 64, 22, 39, 65, 21, 66, 18, 54, 8, 20, 32, 63, 50, 61, 46, 60, 30, 36, 62, 52, 3, 42, 14, 33, 57, 44, 29, 13, 31, 6, 9, 37, 35, };
const char* Keyword[ 11 ] = { "qxueu", "nvzcl", "mpitgop", "ttfuhwq", "vcilh", "gbmqwzu", "speudvf", "kzjmk", "lmwqjz", 0, "onzeytm", };
const uint32_t Hash[ 11 ] = { 0xC6C406FB, 0xFDC639E0, 0xF3A8069C, 0x5ED382D1, 0x540487F1, 0x80611D5B, 0xD497C97B, 0x63311275, 0x234F5C33, 0x00000000, 0x4E8680CF, };
enum Key { QXUEU=0, NVZCL=1, MPITGOP=2, TTFUHWQ=3, VCILH=4, GBMQWZU=5, SPEUDVF=6, KZJMK=7, LMWQJZ=8, ONZEYTM=10, };
```

It's unusual with smaller keyword sizes and key lengths but sometimes the program can't find a combination 
 of mixtable and keyword-table size that perfectly hashes the keywords:
```
Keywords (fuj, rhilz, crwdvmw, fyawb, qqxyxvh, rqkk, uwyt, ppf, dcv, cxgdzz, ).
Failed.
```

For larger keyword lengths more 'holes' in the keyword-table may be apparent. These holes are for ordinals that don't
 map to any keyword and only serve to make a size-combination of the mixtable and keyword-table that creates a
 (close) perfect hash:
```
Keywords (kjcwgtqvozv, rokaarnumeisc, eitxvdgxao, qkgrfojtalvrap, fyxjjdtcaof, mhxexhlrobcie, dvqvbxtygce, eeqlnetkis, vlhzprdfxa, sdsncaribxm, )

Solution for 10 keywords using a 13 keyword table with a 64 slot mix table (235 iterations)
uint8_t Mixtable[ 64 ] = { 22, 33, 45, 60, 35, 61, 25, 56, 49, 55, 47, 64, 24, 13, 23, 40, 18, 38, 9, 51, 19, 27, 46, 8, 42, 36, 58, 54, 50, 34, 37, 39, 12, 43, 29, 66, 26, 63, 14, 4, 62, 41, 16, 31, 6, 59, 32, 30, 5, 65, 3, 17, 28, 10, 7, 20, 21, 52, 11, 53, 44, 48, 15, 57, };
const char* Keyword[ 13 ] = { "sdsncaribxm", "kjcwgtqvozv", 0, "dvqvbxtygce", "qkgrfojtalvrap", "fyxjjdtcaof", 0, "mhxexhlrobcie", "eitxvdgxao", "rokaarnumeisc", "vlhzprdfxa", 0, "eeqlnetkis", };
const uint32_t Hash[ 13 ] = { 0x00000041, 0x0000001B, 0x00000000, 0x00000037, 0x0000002B, 0x0000002C, 0x00000000, 0x0000002E, 0x0000002F, 0x00000023, 0x00000024, 0x00000000, 0x0000000C, };
enum Key { SDSNCARIBXM=0, KJCWGTQVOZV=1, DVQVBXTYGCE=3, QKGRFOJTALVRAP=4, FYXJJDTCAOF=5, MHXEXHLROBCIE=7, EITXVDGXAO=8, ROKAARNUMEISC=9, VLHZPRDFXA=10, EEQLNETKIS=12, };

Solution for 10 keywords using a 11 keyword table with a 64 slot mix table (38 iterations)
uint8_t Mixtable[ 64 ] = { 22, 8, 61, 23, 51, 36, 56, 65, 16, 46, 66, 44, 64, 33, 32, 34, 48, 28, 57, 38, 39, 54, 18, 14, 29, 11, 53, 31, 7, 12, 49, 4, 43, 60, 25, 50, 6, 35, 40, 3, 37, 63, 24, 41, 47, 26, 20, 30, 17, 19, 13, 10, 62, 9, 45, 21, 15, 59, 58, 52, 55, 27, 5, 42, };
const char* Keyword[ 11 ] = { "dvqvbxtygce", "sdsncaribxm", "eitxvdgxao", "fyxjjdtcaof", "rokaarnumeisc", "qkgrfojtalvrap", "eeqlnetkis", 0, "kjcwgtqvozv", "vlhzprdfxa", "mhxexhlrobcie", };
const uint32_t Hash[ 11 ] = { 0x2297E567, 0xFB781C79, 0x3F287EB7, 0x4529CC6C, 0x1D34F473, 0x19E27CA2, 0x509CA143, 0x00000000, 0x66B5D32A, 0x44E110EA, 0x1068251A, };
enum Key { DVQVBXTYGCE=0, SDSNCARIBXM=1, EITXVDGXAO=2, FYXJJDTCAOF=3, ROKAARNUMEISC=4, QKGRFOJTALVRAP=5, EEQLNETKIS=6, KJCWGTQVOZV=8, VLHZPRDFXA=9, MHXEXHLROBCIE=10, };
```

Output from this approach can be used to identify and validate keyword inputs using something like:
```
const char* Keyword[ 11 ] = { "qxueu", "nvzcl", "mpitgop", "ttfuhwq", "vcilh", "gbmqwzu", "speudvf", "kzjmk", "lmwqjz", 0, "onzeytm", };
enum Key { QXUEU=0, NVZCL=1, MPITGOP=2, TTFUHWQ=3, VCILH=4, GBMQWZU=5, SPEUDVF=6, KZJMK=7, LMWQJZ=8, ONZEYTM=10, };

// return ordinal of identified keyword, ~0 otherwise
uint32_t parse_keyword(const char *keyword)
{
    uint8_t Mixtable[ 64 ] = { 15, 16, 38, 28, 41, 47, 55, 11, 53, 59, 5, 19, 23, 26, 51, 10, 43, 12, 48, 58, 40, 56,
     45, 17, 24, 27, 25, 34, 7, 49, 4, 64, 22, 39, 65, 21, 66, 18, 54, 8, 20, 32, 63, 50, 61, 46, 60, 30, 36, 62, 52,
      3, 42, 14, 33, 57, 44, 29, 13, 31, 6, 9, 37, 35, };

    const uint32_t Hash[ 11 ] = { 0xC6C406FB, 0xFDC639E0, 0xF3A8069C, 0x5ED382D1, 0x540487F1,
     0x80611D5B, 0xD497C97B, 0x63311275, 0x234F5C33, 0x00000000, 0x4E8680CF, };
    
    uint32_t crc = crc32(keyword);
    
    // now we hash the crc using the mixtable to (hopefully) derive a unique key
    uint32_t key = ~0U;
    key ^= mixTable[(crc >> 26) & 0b0111111];
    key ^= mixTable[(crc >> 20) & 0b0111111];
    key ^= mixTable[(crc >> 14) & 0b0111111];
    key ^= mixTable[(crc >>  8) & 0b0111111];
    key ^= mixTable[(crc >>  2) & 0b0111111]; // leftovers
    key %= 11; // hash/keyword table size

    return Hash[key] == crc ? key : ~0U;
}
```
