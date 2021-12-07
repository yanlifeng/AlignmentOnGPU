/**
* Copyright (c) 2013, Laboratory for Biocomputing and Informatics, Boston University
*  All rights reserved.
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are met:
*   + Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*   + Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*   + This source code may not be used in any program that is sold, any
*    derivative work must allow free distribution and modification.
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE LABORATORY FOR BIOCOMPUTING AND INFORMATICS
*  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
OR CONSEQUENTIAL
*  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
*  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

//Uses 265 operations, counting |, &, ^, and +


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define wordsize 64

int bitwise_alignment(char *s1, char *s2, int words)
/**
*
* s1 and s2 are the strings to be aligned.
* words is a dummy variable used to give the single word and
* multi word programs the same signature.
*
**/
{

    int i, j, n, m;
    unsigned long long int bitmask;
    unsigned long long int Matches;
    unsigned long long int NotMatches;
    unsigned long long int all_ones = ~0x0000000000000000;
    unsigned long long int one = 0x0000000000000001;

    unsigned long long int DVpos7shift, DVpos6shift, DVpos5shift, DVpos4shift, DVpos3shift, DVpos2shift, DVpos1shift, DVzeroshift, DVneg1shift, DVneg2shift, DVneg3shift, DVneg4shift, DVneg5shift;
    unsigned long long int DHpos7, DHpos6, DHpos5, DHpos4, DHpos3, DHpos2, DHpos1, DHzero, DHneg1, DHneg2, DHneg3, DHneg4, DHneg5;
    unsigned long long int INITpos7s, INITpos6s, INITpos5s, INITpos4s, INITpos3s, INITpos2s, INITpos1s, INITzeros, INITneg1s, INITneg2s, INITneg3s, INITneg4s;
    unsigned long long int DVpos6shiftNotMatch, DVpos5shiftNotMatch, DVpos4shiftNotMatch, DVpos3shiftNotMatch;
    unsigned long long int RemainDHneg5;
    unsigned long long int DHneg5topos2;
    unsigned long long int DVpos7shiftorMatch;
    unsigned long long int DVnot7to3shiftorMatch;
    unsigned long long int DHpos7orMatch;
    unsigned long long int DHpos3orDHpos4orDHpos5orDHpos6orDHpos7;
    unsigned long long int DHpos1orDHpos2;
    unsigned long long int DHneg3orDHneg2;
    unsigned long long int DHneg1orDHzeroorDHpos1orDHpos2;
    unsigned long long int DHpos3orDHpos4orDHpos5orDHpos6;
    unsigned long long int DHneg1orDHzero;
    unsigned long long int DHpos5orDHpos6;
    unsigned long long int DHpos3orDHpos4;
    unsigned long long int add1;
    unsigned long long int add2;
    unsigned long long int add4;
    unsigned long long int add8;
    unsigned long long int add16;


    int score;


    char *iterate = s1;
    unsigned long long int *matchvec = NULL;
    if (matchvec == NULL) {
        matchvec = (unsigned long long int *) calloc(256, sizeof(unsigned long long int));
    }
    n = strlen(s1);
    m = strlen(s2);
    n--;
    m--;
    if (n == 0 || m == 0)return 0;



    //*************************encode match strings A C G T N for string1
    //loop through string1 and store bits in matchA, matchC, etc.
    //position zero corresponds to column one in the score matrix, i.e., first character
    //so we start with i = 0 and bitmask = 1
    //below is optimized
    bitmask = 0x0000000000000001;
    for (i = 0; i < n; ++i) {
        matchvec[(*iterate)] |= bitmask;
        ++iterate;
        bitmask <<= 1;
    }



    //intialize top row (penalty for initial gap, unless semi-global alignment is being done)
    DHneg5 = all_ones;
    DHneg4 = DHneg3 = DHneg2 = DHneg1 = DHzero = DHpos1 = DHpos2 = DHpos3 = DHpos4 = DHpos5 = DHpos6 = DHpos7 = 0;

    // M=2 I=-3 G=-5
    // min=-5 mid=2 max=7
    // step1 7
    // step2 3--6
    // step3 -4--2
    // step 4 -5
    //recursion
    for (i = 0, iterate = s2; i < m; ++i, ++iterate) {
        Matches = matchvec[*iterate];
        //Complement Matches
        NotMatches = ~Matches;

        //Finding the vertical values. 		//Find 7s == find max
        INITpos7s = DHneg5 & Matches;
        DVpos7shift = (((INITpos7s + DHneg5) ^ DHneg5) ^
                       INITpos7s);//store DV+7 which has shifted one position to high bit direction

        //set RemainingDHneg5
        RemainDHneg5 = DHneg5 ^ (DVpos7shift >> 1);//shift to low bit before use
        //combine 7s and Matches
        DVpos7shiftorMatch = DVpos7shift | Matches;

        //Find 6s
        INITpos6s = (DHneg4 & DVpos7shiftorMatch);//DVij <<-- DVij-1,DHi-1j so use Dv+7<<1 to cal now DV+6
        DVpos6shift = (((INITpos6s << 1) + RemainDHneg5) ^ RemainDHneg5);//if DH==min, then DVij == DVij-1
        DVpos6shiftNotMatch = DVpos6shift & NotMatches;

        //Find 5s
        INITpos5s = (DHneg3 & DVpos7shiftorMatch) | (DHneg4 & DVpos6shiftNotMatch);
        DVpos5shift = (((INITpos5s << 1) + RemainDHneg5) ^ RemainDHneg5);
        DVpos5shiftNotMatch = DVpos5shift & NotMatches;

        //Find 4s
        INITpos4s = (DHneg2 & DVpos7shiftorMatch) | (DHneg3 & DVpos6shiftNotMatch) | (DHneg4 & DVpos5shiftNotMatch);
        DVpos4shift = (((INITpos4s << 1) + RemainDHneg5) ^ RemainDHneg5);
        DVpos4shiftNotMatch = DVpos4shift & NotMatches;

        //Find 3s
        INITpos3s = (DHneg1 & DVpos7shiftorMatch) | (DHneg2 & DVpos6shiftNotMatch) | (DHneg3 & DVpos5shiftNotMatch) |
                    (DHneg4 & DVpos4shiftNotMatch);
        DVpos3shift = (((INITpos3s << 1) + RemainDHneg5) ^ RemainDHneg5);
        DVpos3shiftNotMatch = DVpos3shift & NotMatches;

        //set DVnot7to3shiftorMatch
        DVnot7to3shiftorMatch = ~(DVpos7shiftorMatch | DVpos6shift | DVpos5shift | DVpos4shift | DVpos3shift);

        //new add method
        //pack all DH into DHbits (2s complement)
        //use mapping [-5..7]->[0..-12]
        //pack DV3..7 and DVnot7to3shiftorMatch into DVbits (regular numbers)
        //use mapping [-5..7]->[0..12], note DVnot7to3shiftorMatch->7
        //shift DVbits back one to make copy unshifted.  Now there is an unshifted copy and shifted copy
        //"add" shifted DVbits to DHbits
        //OR unshifted DVbits to result of add
        //somehow find DV=-5 (without unpacking DVbits<=0?)
        //pack DV=-5 into DVbits (set bits equal zero)
        //find DHneg5to2notMatch
        //mask DHbits with DHneg5to2notMatch
        //pack DHneg5to2notMatch into DHbits
        //"add" unshifted DVbits and DHbits
        //unpack result into DH

        //Find 2s
        DVpos2shift = ((DHzero & DVpos7shiftorMatch) | (DHneg1 & DVpos6shiftNotMatch) | (DHneg2 & DVpos5shiftNotMatch) |
                       (DHneg3 & DVpos4shiftNotMatch) | (DHneg4 & DVpos3shiftNotMatch) |
                       (DHneg5 & DVnot7to3shiftorMatch)) << 1;
        //Find 1s
        DVpos1shift = ((DHpos1 & DVpos7shiftorMatch) | (DHzero & DVpos6shiftNotMatch) | (DHneg1 & DVpos5shiftNotMatch) |
                       (DHneg2 & DVpos4shiftNotMatch) | (DHneg3 & DVpos3shiftNotMatch) |
                       (DHneg4 & DVnot7to3shiftorMatch)) << 1;
        //Find 0s
        DVzeroshift = ((DHpos2 & DVpos7shiftorMatch) | (DHpos1 & DVpos6shiftNotMatch) | (DHzero & DVpos5shiftNotMatch) |
                       (DHneg1 & DVpos4shiftNotMatch) | (DHneg2 & DVpos3shiftNotMatch) |
                       (DHneg3 & DVnot7to3shiftorMatch)) << 1;
        //Find -1s
        DVneg1shift = ((DHpos3 & DVpos7shiftorMatch) | (DHpos2 & DVpos6shiftNotMatch) | (DHpos1 & DVpos5shiftNotMatch) |
                       (DHzero & DVpos4shiftNotMatch) | (DHneg1 & DVpos3shiftNotMatch) |
                       (DHneg2 & DVnot7to3shiftorMatch)) << 1;
        //Find -2s
        DVneg2shift = ((DHpos4 & DVpos7shiftorMatch) | (DHpos3 & DVpos6shiftNotMatch) | (DHpos2 & DVpos5shiftNotMatch) |
                       (DHpos1 & DVpos4shiftNotMatch) | (DHzero & DVpos3shiftNotMatch) |
                       (DHneg1 & DVnot7to3shiftorMatch)) << 1;
        //Find -3s
        DVneg3shift = ((DHpos5 & DVpos7shiftorMatch) | (DHpos4 & DVpos6shiftNotMatch) | (DHpos3 & DVpos5shiftNotMatch) |
                       (DHpos2 & DVpos4shiftNotMatch) | (DHpos1 & DVpos3shiftNotMatch) |
                       (DHzero & DVnot7to3shiftorMatch)) << 1;
        //Find -4s
        DVneg4shift = ((DHpos6 & DVpos7shiftorMatch) | (DHpos5 & DVpos6shiftNotMatch) | (DHpos4 & DVpos5shiftNotMatch) |
                       (DHpos3 & DVpos4shiftNotMatch) | (DHpos2 & DVpos3shiftNotMatch) |
                       (DHpos1 & DVnot7to3shiftorMatch)) << 1;
        //Find -5s
        DVneg5shift = all_ones ^
                      (DVpos7shift | DVpos6shift | DVpos5shift | DVpos4shift | DVpos3shift | DVpos2shift | DVpos1shift |
                       DVzeroshift | DVneg1shift | DVneg2shift | DVneg3shift | DVneg4shift);
        DHpos3 &= NotMatches;
        DHpos4 &= NotMatches;
        DHpos5 &= NotMatches;
        DHpos6 &= NotMatches;
        //combine 7s and Matches
        DHpos7orMatch = DHpos7 | Matches;
        //group -5topos2
        DHneg5topos2 = all_ones ^ (DHpos7orMatch | DHpos6 | DHpos5 | DHpos4 | DHpos3);
        //Find -4s
        DHneg4 = ((DVpos6shift & DHpos7orMatch) | (DVpos5shift & DHpos6) | (DVpos4shift & DHpos5) |
                  (DVpos3shift & DHpos4) | (DVpos2shift & DHpos3) | (DVpos1shift & DHneg5topos2));
        //Find -3s
        DHneg3 = ((DVpos5shift & DHpos7orMatch) | (DVpos4shift & DHpos6) | (DVpos3shift & DHpos5) |
                  (DVpos2shift & DHpos4) | (DVpos1shift & DHpos3) | (DVzeroshift & DHneg5topos2));
        //Find -2s
        DHneg2 = ((DVpos4shift & DHpos7orMatch) | (DVpos3shift & DHpos6) | (DVpos2shift & DHpos5) |
                  (DVpos1shift & DHpos4) | (DVzeroshift & DHpos3) | (DVneg1shift & DHneg5topos2));
        //Find -1s
        DHneg1 = ((DVpos3shift & DHpos7orMatch) | (DVpos2shift & DHpos6) | (DVpos1shift & DHpos5) |
                  (DVzeroshift & DHpos4) | (DVneg1shift & DHpos3) | (DVneg2shift & DHneg5topos2));
        //Find 0s
        DHzero = ((DVpos2shift & DHpos7orMatch) | (DVpos1shift & DHpos6) | (DVzeroshift & DHpos5) |
                  (DVneg1shift & DHpos4) | (DVneg2shift & DHpos3) | (DVneg3shift & DHneg5topos2));
        //Find 1s
        DHpos1 = ((DVpos1shift & DHpos7orMatch) | (DVzeroshift & DHpos6) | (DVneg1shift & DHpos5) |
                  (DVneg2shift & DHpos4) | (DVneg3shift & DHpos3) | (DVneg4shift & DHneg5topos2));
        //Find 2s
        DHpos2 = ((DVzeroshift & DHpos7orMatch) | (DVneg1shift & DHpos6) | (DVneg2shift & DHpos5) |
                  (DVneg3shift & DHpos4) | (DVneg4shift & DHpos3) | (DVneg5shift & DHneg5topos2));
        //Find 3s
        DHpos3 = ((DVneg1shift & DHpos7orMatch) | (DVneg2shift & DHpos6) | (DVneg3shift & DHpos5) |
                  (DVneg4shift & DHpos4) | (DVneg5shift & DHpos3));
        //Find 4s
        DHpos4 = ((DVneg2shift & DHpos7orMatch) | (DVneg3shift & DHpos6) | (DVneg4shift & DHpos5) |
                  (DVneg5shift & DHpos4));
        //Find 5s
        DHpos5 = ((DVneg3shift & DHpos7orMatch) | (DVneg4shift & DHpos6) | (DVneg5shift & DHpos5));
        //Find 6s
        DHpos6 = ((DVneg4shift & DHpos7orMatch) | (DVneg5shift & DHpos6));
        //Find 7s
        DHpos7 = ((DVneg5shift & DHpos7orMatch));
        //Find -5s
        DHneg5 = all_ones ^
                 (DHneg4 | DHneg3 | DHneg2 | DHneg1 | DHzero | DHpos1 | DHpos2 | DHpos3 | DHpos4 | DHpos5 | DHpos6 |
                  DHpos7);
    }
    //find scores in last row
    DHneg1orDHzero = (DHneg1 | DHzero);
    DHpos1orDHpos2 = (DHpos1 | DHpos2);
    DHpos3orDHpos4 = (DHpos3 | DHpos4);
    DHpos5orDHpos6 = (DHpos5 | DHpos6);
    DHpos5orDHpos6 = (DHpos5 | DHpos6);
    DHpos1orDHpos2 = (DHpos1 | DHpos2);
    DHneg3orDHneg2 = (DHneg3 | DHneg2);
    DHpos3orDHpos4orDHpos5orDHpos6 = (DHpos3orDHpos4 | DHpos5orDHpos6);
    DHneg1orDHzeroorDHpos1orDHpos2 = (DHneg1orDHzero | DHpos1orDHpos2);
    DHpos3orDHpos4orDHpos5orDHpos6orDHpos7 = (DHpos3orDHpos4orDHpos5orDHpos6 | DHpos7);
    //-5:0 0000
    //-4:1 0001
    //-3:2 0010
    //-2:3 0011
    //-1:4 0100
    //0:5  0101
    //1:6  0110
    //2:7  0111
    //3:8  1000
    //4:9  1001
    //5:10 1010
    //6:11 1011
    //7:12 1100
    add1 = DHpos4 | DHneg4 | DHpos6 | DHneg2 | DHzero | DHpos2;
    add2 = DHpos5orDHpos6 | DHpos1orDHpos2 | DHneg3orDHneg2;
    add4 = DHpos7 | DHneg1orDHzeroorDHpos1orDHpos2;
    add8 = DHpos3orDHpos4orDHpos5orDHpos6orDHpos7;
    add16 = 0;

    bitmask = 0x0000000000000001;
    score = -5 * m;//init dp[-1][m] is all m positions GAP(-5)
    for (i = 0; i < n; i++) {
        score += ((add1 & bitmask) >> i) * 1 + ((add2 & bitmask) >> i) * 2 + ((add4 & bitmask) >> i) * 4 +
                 ((add8 & bitmask) >> i) * 8 + ((add16 & bitmask) >> i) * 16 - 5;
        //print this score is dp[i][m]
        bitmask <<= 1;
    }

    free(matchvec);

    return score;
}


int main(int argc, char *argv[]) {
    char *fname;
    int i;
    if (argc == 1) {

        fprintf(stderr, "usage: %s filename [-h]\n", argv[0]);
        exit(1);
    }
    for (i = 1; i < argc; i++)  // Skip program name
    {
        if (strcmp(argv[i], "-h") == 0)  /*if -h, print help */
        {
            printf(" \n");
            exit(0);
        } else {
            fname = argv[i];
        }
    }
    int cnt=0;
    FILE *in = fopen(fname, "r");
    char *s1 = malloc(1000 * sizeof(char));//One of the strings to align
    char *s2 = malloc(1000 * sizeof(char));//The other string to align
    while (fgets(s1, 1000, in) != NULL) {

        fgets(s2, 1000, in);
        int score=bitwise_alignment(s1, s2, (strlen(s1) / 63 + 1));
        printf("%d,score is %d\n",cnt++,score);
    }
    return 0;
}



