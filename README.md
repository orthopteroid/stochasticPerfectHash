# stochasticPerfectHash
Uses randomization to derive a Pearson mixing table for small sets of alphabetic keys. These tables might be
 useful for small parsers when parsing text identifiers - possibly for a communications protocol or a custom
 SAX-like xml parser.

For background info see: 
```http://cs.mwsu.edu/~griffin/courses/2133/downloads/Spring11/p677-pearson.pdf``` If this link is dead, try google.

This is just a small demo app - it has no options or interface. If you compile and run it you will see it
 try to create mixing tables for 10 randomly generated keys - all lower case alpha - into a 15 element codetable. To
 use this code you'd likely want to built it yourself with tweaks for your specific circumstances.

When run, the program generates N random keys and tries to find the keyword arrangement and character mixing
 table permutation that allows
 the keywords to hash into ordinals in the keyword table.  It tries a combination of approaches:
1. firstly, it increases the size of the character mixing table (up to some maximum). This permutes the calculation of 
 the Pearson Hash (see above mentioned paper).
2. then, it adds extra blank entries (sparsity) in the keyword table. This effectively increases the bucket-size of the
 hash table. It's assumed that increasing this table is more costly memory-wise as these might be pointers, whereas
 the mixing table is uint8s.

The output typically looks like:
```
Keywords (sejrerj, hgphwyx, yxruywpal, mtdow, shoxax, hdvpbto, tmjpmfen, lbmos, kmrajryna, jpfeez, )
Solution for 10 keywords using a 10 keyword table with a 17 character mix table (2831 iterations)
uint8_t Mixtable[ 17 ] = { 13, 2, 5, 6, 12, 15, 3, 10, 1, 14, 7, 16, 9, 11, 8, 0, 4, };
const char* Keyword[ 10 ] = { "hgphwyx", "mtdow", "sejrerj", "kmrajryna", "jpfeez", "hdvpbto", "tmjpmfen", "lbmos", "yxruywpal", "shoxax", };
enum Key { HGPHWYX=0, MTDOW=1, SEJRERJ=2, KMRAJRYNA=3, JPFEEZ=4, HDVPBTO=5, TMJPMFEN=6, LBMOS=7, YXRUYWPAL=8, SHOXAX=9, };

Keywords (emdskuwux, gjkisnzx, fywtfxjt, pmgval, mfkjcqrlc, frbfyidvc, ungdcv, xvzlkfytj, kvumcln, kvnhzadmg, )
Solution for 10 keywords using a 10 keyword table with a 18 character mix table (3466 iterations)
uint8_t Mixtable[ 18 ] = { 6, 4, 16, 3, 9, 17, 15, 10, 2, 0, 7, 13, 1, 14, 8, 11, 5, 12, };
const char* Keyword[ 10 ] = { "kvnhzadmg", "kvumcln", "fywtfxjt", "emdskuwux", "frbfyidvc", "xvzlkfytj", "mfkjcqrlc", "gjkisnzx", "pmgval", "ungdcv", };
enum Key { KVNHZADMG=0, KVUMCLN=1, FYWTFXJT=2, EMDSKUWUX=3, FRBFYIDVC=4, XVZLKFYTJ=5, MFKJCQRLC=6, GJKISNZX=7, PMGVAL=8, UNGDCV=9, };

Keywords (arduv, ryltt, wakidnnw, cmrjlg, gwointckj, jupeln, vvjkl, unumyhsd, rtxen, pwxasmgfc, )
Solution for 10 keywords using a 10 keyword table with a 18 character mix table (3443 iterations)
uint8_t Mixtable[ 18 ] = { 3, 12, 0, 15, 13, 4, 16, 7, 17, 10, 14, 9, 8, 2, 11, 1, 5, 6, };
const char* Keyword[ 10 ] = { "wakidnnw", "pwxasmgfc", "rtxen", "jupeln", "cmrjlg", "gwointckj", "arduv", "vvjkl", "unumyhsd", "ryltt", };
enum Key { WAKIDNNW=0, PWXASMGFC=1, RTXEN=2, JUPELN=3, CMRJLG=4, GWOINTCKJ=5, ARDUV=6, VVJKL=7, UNUMYHSD=8, RYLTT=9, };
```

It's unusual with smaller keyword sizes and key lengths but sometimes the program can't find a combination 
 of mixtable and keyword-table size that
 perfectly hashes the keywords:
```
Keywords (fuj, rhilz, crwdvmw, fyawb, qqxyxvh, rqkk, uwyt, ppf, dcv, cxgdzz, ).
Failed.
```

For larger sets of keywords 'holes' in the keyword-table may be apparent. These holes are for ordinals that don't
 map to any keyword and only serve to make a size-combination of the mixtable and keyword-table that creates a
 (close) perfect hash. In this case the code can take quite a few
 additional iterations:
```
Keywords (lzugzyw, dhyykbbih, xiizproys, bxhow, wxbupgene, rgyym, iwzxnn, hiyepo, skyvfqb, dgzuoau, ycwzzl, xswvylm, fwaadqc, vkqsysmaq, ypojdl, )
Solution for 15 keywords using a 18 keyword table with a 47 character mix table (65809 iterations)
uint8_t Mixtable[ 47 ] = { 26, 31, 1, 35, 8, 30, 2, 13, 42, 38, 3, 28, 7, 33, 6, 46, 41, 14, 4, 34, 12, 32, 19, 43, 25, 22, 44, 11, 27, 29, 20, 16, 17, 5, 9, 39, 37, 0, 15, 23, 24, 36, 21, 40, 18, 45, 10, };
const char* Keyword[ 18 ] = { "ypojdl", "bxhow", "dgzuoau", "iwzxnn", 0, "xiizproys", "fwaadqc", 0, "hiyepo", 0, "rgyym", "xswvylm", "ycwzzl", "lzugzyw", "vkqsysmaq", 0, 0, 0, };
enum Key { YPOJDL=0, BXHOW=1, DGZUOAU=2, IWZXNN=3, XIIZPROYS=5, FWAADQC=6, HIYEPO=8, RGYYM=10, XSWVYLM=11, YCWZZL=12, LZUGZYW=13, VKQSYSMAQ=14, };
```

Not accounting for actual keyword validation, output from this program for a specific set of keywords and
 mixtable could be used something like:
```
const char* Keyword[ 10 ] = { "wakidnnw", "pwxasmgfc", "rtxen", "jupeln", "cmrjlg", "gwointckj", "arduv", "vvjkl", "unumyhsd", "ryltt", };
enum Key { WAKIDNNW=0, PWXASMGFC=1, RTXEN=2, JUPELN=3, CMRJLG=4, GWOINTCKJ=5, ARDUV=6, VVJKL=7, UNUMYHSD=8, RYLTT=9, };

uint8_t parse_keyword(const char *keyword)
{
    const uint8_t Mixtable[ 18 ] = { 3, 12, 0, 15, 13, 4, 16, 7, 17, 10, 14, 9, 8, 2, 11, 1, 5, 6, };
    
    uint8_t hash = Mixtable[ keyword[0] - 'a' ]; // char 0
    for(uint i = 1; keyword[i]; i++) // chars 1 ... N
        hash = Mixtable[ ( hash ^ ( keyword[i] - 'a' ) ) % 18 ]; // 18 is the size of the Mixtable
        
    return hash % 10; // 10 is sizeof of the Keyword-table
}
```
