# stochasticPerfectHash
Uses stochastic remainder sampling to derive a Pearson mixing table for small sets of alphabetic keys

This is just a small demo app - it has no options or interface. If you compile and run it you will see it try to create mixing tables for 10 randomly generated 10 character keys - all lower case alpha only. The demo aspect of using just lowercase alpha is that the mix table is only 32 bytes.

The program uses a stochastic method of adjusting the mixing table. I think there might be a poly-time algorithim for finding the mixtable, so if you are at all interested in something more reliable look elsewhere perhaps.

When run, the program generates some random keys (seperated by !) and coughs up the mixtable that perfectly hashes those keys. You can change the code to make more or fewer keys or longer or shorted length. By trial, I've found that when you try to hash more keys you won't find a perfect hash but the code seems to work ok for keys of very long length.

The output looks like:

Finding mix table for dckavdrgjr!zrwohicvzp!dblnuuoyoz!qtectzhkhq!dihbwrlbmk!qpmbeiysio!ubhyddzlpi!bvslypckqq!uifijlrjeb!xychyimzvd!:
uint_8 mix[] = {26, 29, 3, 16, 2, 27, 17, 15, 12, 8, 1, 13, 0, 5, 21, 31, 7, 19, 18, 10, 14, 6, 4, 9, 23, 28, 24, 25, 11, 30, 20, 22, };
Took 14066 iterations
Finding mix table for baxfpodtpf!xyrprkzwqa!ooiddbtxvn!mxnjceyiap!nxoeohrodh!quwyxbbsyx!fkuuwybwgb!ltabaqjrem!avgyvfzxzz!ugmqdioeex!:
uint_8 mix[] = {28, 3, 24, 31, 10, 25, 20, 16, 6, 5, 2, 21, 13, 8, 14, 12, 4, 11, 7, 26, 18, 17, 30, 9, 15, 19, 23, 22, 1, 29, 27, 0, };
Took 2218 iterations
Finding mix table for lpdvbntudd!glsrwcaahy!vlbibfjnzz!nkqqhrebnj!gvwbmsdntm!nqxoyzwjov!kbfcsnvyoi!hwfdxsxchq!pugolgnhpd!cbfhdzuyxk!:
uint_8 mix[] = {18, 12, 3, 2, 29, 0, 26, 27, 4, 10, 23, 9, 17, 24, 8, 31, 1, 11, 25, 5, 6, 21, 13, 19, 22, 30, 7, 15, 28, 14, 16, 20, };
Took 833 iterations
Finding mix table for ahsxagelch!niivwukfnd!xkggzwgpla!elhxkjfquh!xhqifoepur!urbazdzfsm!fyxoxjycas!mzcejhsnyo!hsgkvfnulh!gsgfjdojiq!:
uint_8 mix[] = {11, 29, 28, 24, 25, 14, 18, 30, 31, 21, 27, 17, 26, 15, 4, 13, 10, 12, 5, 2, 1, 0, 7, 9, 19, 8, 6, 22, 23, 16, 3, 20, };
Took 3013 iterations
Finding mix table for nhquhrbhca!ajzlzzrftf!gjxkajhtaf!hnmxjvomdt!mdenqdmjkh!qqqocsxklx!rtmgsxbjjg!cxlglblzmv!hdnzrqsqcf!ptadbtafcl!:
uint_8 mix[] = {14, 4, 26, 22, 18, 13, 9, 3, 28, 23, 10, 24, 1, 16, 2, 11, 7, 20, 25, 12, 31, 19, 29, 27, 15, 5, 6, 21, 0, 17, 30, 8, };
Took 122 iterations
Finding mix table for qjnfzvfqfh!wzgosgtckk!odwsoxfqhi!xxulctijjp!shobvijrlt!dzwbtmybch!lcefpiyzrj!omqfnonwfa!pibnlvbjwf!rjhxpyipxz!:
uint_8 mix[] = {15, 9, 27, 6, 3, 29, 2, 18, 23, 11, 24, 30, 25, 5, 21, 13, 26, 12, 14, 22, 17, 10, 4, 16, 8, 31, 0, 7, 20, 19, 28, 1, };
Took 2984 iterations
Finding mix table for vlfkoyxdix!qeakkvdjkt!fqewgselyu!atffdvfdyn!aqudaeyfni!avyergwwsw!ssqzzvueau!rbmlgoretf!puapatwzro!vlinkhlgll!:
uint_8 mix[] = {6, 11, 16, 12, 7, 17, 3, 31, 27, 19, 14, 1, 9, 18, 28, 0, 21, 13, 25, 26, 30, 29, 4, 23, 2, 22, 24, 20, 10, 5, 8, 15, };
Took 2513 iterations
Finding mix table for vhjixohxdl!wcbekmnoli!rmlxbxuelh!miqvsqlcnr!njvpphbcvo!lobylexgji!pxsiumyiol!zcwwtldwoy!kbmnzaswid!gzaahwmfed!:
uint_8 mix[] = {2, 25, 23, 19, 6, 12, 3, 30, 5, 1, 27, 20, 14, 26, 10, 21, 31, 18, 22, 28, 11, 7, 24, 15, 16, 4, 0, 17, 9, 29, 8, 13, };
Took 935 iterations
Finding mix table for udxgiqagcu!lellcewvku!jimddnmdqx!inagtlyvtd!qghbrliphs!jscxvfnjjf!hrshzluyjo!bzukcoykdh!fpzhownbfy!hmrbwrpqry!:
uint_8 mix[] = {8, 18, 1, 19, 29, 27, 10, 14, 23, 26, 13, 22, 25, 4, 24, 0, 28, 16, 31, 7, 5, 9, 3, 11, 30, 2, 15, 12, 17, 6, 21, 20, };
Took 6161 iterations
Finding mix table for whsltbsngd!dedghgigeq!abmjgsvyjs!fhzyusbnhh!smnxsxfcdm!tfnfpvzktk!ebrdzlxcye!jstwrntxqy!jleysturfn!blosopflrg!:
uint_8 mix[] = {9, 17, 7, 2, 3, 15, 29, 23, 16, 24, 31, 14, 30, 8, 10, 28, 6, 27, 21, 19, 13, 12, 18, 25, 22, 11, 5, 4, 20, 0, 26, 1, };
Took 2889 iterations
