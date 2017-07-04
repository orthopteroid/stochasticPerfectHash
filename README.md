# stochasticPerfectHash
Uses codetable randomization to derive a Pearson mixing table for small sets of alphabetic keys

For background info see: 
```http://cs.mwsu.edu/~griffin/courses/2133/downloads/Spring11/p677-pearson.pdf``` If this link is dead, try google.

This is just a small demo app - it has no options or interface. If you compile and run it you will see it
 try to create mixing tables for 10 randomly generated keys - all lower case alpha - into a 15 element codetable.

When run, the program generates N random keys and tried to find the codetable arrangement that allows
those keys to hash into ordinals in the keyword table.  If this is not possible, the program then tries
two alternate approaches:
1. it adjusts the size of the codetable and continues again
2. it adds extra blank entries (sparsity) in the keyword table and continues again (with the original codetable size)

The output typically looks like:
```
Tring to hash 10 keys into 15 element Pearson codetable...
Keywords (ete, dibfgnc, wiqsgh, jpa, krbidm, xqd, hgmj, sypwwam, gtqrkq, xkahp, ).
Looking for a packed keyword solution... Solution after 4874 iterations
char* ph_keywords[] = {"gtqrkq", "ete", "hgmj", "dibfgnc", "sypwwam", "xqd", "krbidm", "xkahp", "wiqsgh", "jpa", };
uint8_t ph_mix[] = {5, 3, 4, 2, 14, 10, 15, 11, 6, 1, 8, 12, 7, 9, 13, };

Keywords (xwt, ytvvn, kame, ligen, umif, pmvnyvj, wjm, ezw, aluvwe, btm, ).
Looking for a packed keyword solution... Solution after 711 iterations
char* ph_keywords[] = {"xwt", "btm", "ytvvn", "ligen", "kame", "pmvnyvj", "ezw", "wjm", "umif", "aluvwe", };
uint8_t ph_mix[] = {4, 10, 14, 5, 8, 9, 2, 3, 13, 6, 7, 11, 12, 1, 15, };

Keywords (hlfwhv, urncu, cxitd, unnywqi, eyrdhd, emhbgz, ivzl, hgyea, rbjkjh, jbutg, ).
Looking for a packed keyword solution... Solution after 5008 iterations
char* ph_keywords[] = {"hgyea", "urncu", "hlfwhv", "eyrdhd", "ivzl", "unnywqi", "emhbgz", "rbjkjh", "jbutg", "cxitd", };
uint8_t ph_mix[] = {14, 2, 15, 6, 8, 9, 10, 13, 11, 1, 12, 3, 7, 4, 5, };
```

But sometimes the program can't find the a codetable of the specified size that perfectly hashes the keywords:
```
Keywords (fuj, rhilz, crwdvmw, fyawb, qqxyxvh, rqkk, uwyt, ppf, dcv, cxgdzz, ).
Looking for a packed keyword solution... Failed. Trying comparative approaches.
```
so it starts to adjust the codetable:
```
Adjusting codetable size... Solution in a 12 element codetable after 566 iterations
char* ph_keywords[] = {"crwdvmw", "qqxyxvh", "cxgdzz", "rhilz", "rqkk", "fuj", "ppf", "uwyt", "fyawb", "dcv", };
uint8_t ph_mix[] = {4, 7, 11, 6, 9, 10, 3, 12, 8, 2, 5, 1, };
```
and also adjusts the sparsity of the keyword table:
```
Trying sparse in 15 element codetable... Solution at sparsity 1.10 after 3065 iterations
char* ph_keywords[] = {"fuj", "rhilz", "qqxyxvh", "uwyt", "crwdvmw", 0, "fyawb", "dcv", "rqkk", "cxgdzz", "ppf", };
uint8_t ph_mix[] = {13, 8, 11, 3, 12, 5, 14, 9, 10, 6, 2, 1, 4, 15, 7, };
```
The result of this is a keyword table that has some empty/unused entries but all keywords
still hash to their corresponding ordinals. 

Another example:
```
Keywords (jekpvup, wei, qrgzfut, umpuo, vrgpms, whqs, bpc, pud, virozm, ogqmei, ).
Looking for a packed keyword solution... Failed. Trying comparative approaches.

Adjusting codetable size... Solution in a 12 element codetable after 1022 iterations
char* ph_keywords[] = {"umpuo", "bpc", "pud", "qrgzfut", "vrgpms", "virozm", "whqs", "ogqmei", "wei", "jekpvup", };
uint8_t ph_mix[] = {11, 12, 9, 8, 7, 2, 5, 4, 6, 10, 1, 3, };

Trying sparse in 15 element codetable... Solution at sparsity 1.10 after 123 iterations
char* ph_keywords[] = {0, "virozm", "qrgzfut", "pud", "jekpvup", "ogqmei", "whqs", "bpc", "vrgpms", "umpuo", "wei", };
uint8_t ph_mix[] = {7, 9, 11, 3, 13, 10, 2, 1, 8, 14, 6, 5, 12, 15, 4, };
```
