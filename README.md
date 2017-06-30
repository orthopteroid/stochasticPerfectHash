# stochasticPerfectHash
Uses stochastic remainder sampling to derive a Pearson mixing table for small sets of alphabetic keys

This is just a small demo app - it has no options or interface. If you compile and run it you will see it try to create mixing tables for 10 randomly generated 10 character keys - all lower case alpha only. The demo aspect of using just lowercase alpha is that the mix table is only 32 bytes.

The program uses a stochastic method of adjusting the mixing table. I think there might be a poly-time algorithim for finding the mixtable, so if you are at all interested in something more reliable look elsewhere perhaps.

When run, the program generates some random keys (seperated by !) and coughs up the mixtable that perfectly hashes those keys. You can change the code to make more or fewer keys or longer or shorted length. If you're unable to hash a large set of long keys try making the table bigger - but note that a larger type will be needed for larger tables.

The output looks like:
```
Generating 10 keys of length 10. Hashing into 35 element table
Finding mix table for eyykxasvss!dsggyqlvwl!bxmgywmdoy!yswwftyzrq!rukzciqneo!zhnnnnmaqa!youwmbsmcj!evfovkxnxd!bykqoadaat!cakywyaone!:
uint16_t mix[] = {33, 5, 29, 30, 11, 4, 32, 1, 24, 10, 19, 18, 9, 2, 21, 26, 28, 27, 0, 16, 20, 6, 14, 34, 31, 7, 23, 15, 12, 25, 17, 22, 13, 8, 3, };
Took 6 iterations
Finding mix table for qnljzgoqox!sksyygtdfc!uhgpyotbtq!bjgmuhuixk!hsucqvkjar!mvzumxlfae!xbpfpjpkuo!wdgrfxorio!jwjkrxjcek!iclxjbiane!:
uint16_t mix[] = {18, 1, 2, 3, 23, 26, 13, 28, 8, 5, 21, 15, 31, 16, 17, 29, 33, 34, 0, 6, 32, 27, 22, 4, 14, 24, 12, 9, 19, 25, 11, 7, 20, 10, 30, };
Took 0 iterations
Finding mix table for txtkfcfjbn!mvrwrenvds!ndrhotcnnv!sguoszqzis!nxpgvglidq!cswvamrcze!zuntihsahd!uwcjcxrphw!ijpggpsxtr!cvnrovyjyh!:
uint16_t mix[] = {5, 28, 16, 3, 0, 22, 6, 23, 8, 14, 13, 10, 1, 9, 32, 15, 29, 17, 18, 2, 24, 21, 30, 7, 11, 25, 20, 27, 34, 19, 4, 12, 26, 33, 31, };
Took 0 iterations
Finding mix table for cymqmdulwk!wmaxvhflcv!rhxdvsnrtg!zvgnosskeq!ucexbaeinh!dfqaklvyeo!efmluafmmj!ejnjgrlmzy!vdfofqzcqg!rwnfjihpwt!:
ack! can't do it.
Took 1000000 iterations
Finding mix table for keeonuptmi!kxumyijrgr!zwpysumvay!xncbbpxskl!cuiwiigrbo!lalaadumaw!kzloanfzhp!kkmuiucpod!dzgqzgwvtw!tfwfuyuzyb!:
uint16_t mix[] = {6, 34, 19, 3, 25, 1, 7, 10, 12, 14, 2, 15, 5, 13, 29, 9, 8, 11, 4, 26, 20, 28, 30, 31, 18, 27, 16, 22, 24, 17, 23, 0, 32, 33, 21, };
Took 0 iterations
Finding mix table for qwnerkyzhw!uarrvjxmwi!ccyfnfcgzt!rrpfxirwky!uealwxwvls!govetklxqm!rigipdtgbd!hvhjifhfbs!xjgupcfabx!puhvdwywfc!:
uint16_t mix[] = {19, 32, 9, 34, 0, 15, 30, 14, 8, 18, 12, 22, 2, 5, 11, 6, 7, 21, 3, 27, 20, 23, 10, 4, 31, 25, 28, 26, 13, 29, 24, 1, 17, 33, 16, };
Took 3 iterations
Finding mix table for ycyvhkilbp!hfwekrsovv!yomymywouh!islipssafw!rnbntnelez!hcqtccuatq!hdkumcogcu!cvhfiatooz!pvbfqfimhd!cogpksrybv!:
uint16_t mix[] = {29, 11, 2, 3, 14, 19, 33, 13, 16, 24, 31, 12, 15, 28, 25, 0, 17, 22, 30, 8, 26, 10, 27, 18, 7, 20, 1, 9, 23, 6, 5, 21, 32, 34, 4, };
Took 2 iterations
Finding mix table for wsrkwnjdox!yznvhwsdac!kezddwtcup!fsjycfllia!kizygiuaou!caadeezzgt!qnlalqhwds!yoaymgiigy!ekzenfkmes!hwfvwslepo!:
uint16_t mix[] = {16, 22, 10, 3, 1, 11, 31, 24, 6, 27, 15, 13, 0, 32, 23, 7, 2, 5, 30, 14, 19, 9, 17, 18, 34, 4, 26, 21, 28, 25, 20, 8, 33, 29, 12, };
Took 2 iterations
Finding mix table for ihjsoulelu!nejwljqrcf!dzqzvrpofp!lpxujlqxsc!rhiddvotms!aprqopkgdr!vojuksibra!flhnokjefx!xinqadfkjk!chyldlflmz!:
uint16_t mix[] = {22, 24, 29, 18, 21, 4, 6, 30, 3, 34, 7, 20, 10, 26, 12, 32, 5, 14, 16, 28, 9, 1, 23, 8, 31, 11, 2, 33, 19, 15, 0, 17, 27, 13, 25, };
Took 7 iterations
Finding mix table for xfrcicrwoe!oizziiegaj!ygzpnabfmj!ljqcnyghwx!llflmpvqwv!buccjrcnyq!wjcmopkuwh!rkuyviqsao!obksdvliij!ygtauhshdq!:
uint16_t mix[] = {1, 4, 2, 33, 15, 17, 19, 20, 8, 23, 11, 27, 6, 9, 34, 10, 26, 24, 31, 7, 5, 16, 12, 0, 30, 28, 21, 22, 3, 14, 25, 32, 13, 18, 29, };
Took 1 iterations
```
