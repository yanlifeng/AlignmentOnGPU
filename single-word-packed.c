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

//Uses 166 operations, counting |, &, ^, and +

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int bitwise_alignment(char *s1, char *s2, int words)
/**
*
* s1 and s2 are the strings to be aligned.
* words is a dummy variable used to give the single word and
* multi word programs the same signature.
*
**/
{
    int debug = 1;
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

    unsigned long long int notDVDHbits16notDVDHbits8DVDHbits4notDVDHbits2;
    unsigned long long int notDVDHbits16DVDHbits8notDVDHbits4;
    unsigned long long int notDVDHbits16;
    unsigned long long int DVDHbits16notDVDHbits8notDVDHbits4DVDHbits2;
    unsigned long long int notDVDHbits16notDVDHbits8notDVDHbits4notDVDHbits2;
    unsigned long long int DVDHbits16DVDHbits8DVDHbits4;
    unsigned long long int notDVDHbits16notDVDHbits8;
    unsigned long long int DVDHbits16DVDHbits8DVDHbits4DVDHbits2;
    unsigned long long int notDVDHbits16notDVDHbits8notDVDHbits4DVDHbits2;
    unsigned long long int DVDHbits16notDVDHbits8DVDHbits4;
    unsigned long long int notDVDHbits16DVDHbits8notDVDHbits4notDVDHbits2;
    unsigned long long int notDVDHbits16notDVDHbits8DVDHbits4;
    unsigned long long int DVDHbits16notDVDHbits8DVDHbits4notDVDHbits2;
    unsigned long long int notDVDHbits16DVDHbits8DVDHbits4DVDHbits2;
    unsigned long long int DVDHbits16DVDHbits8notDVDHbits4;
    unsigned long long int DVDHbits16DVDHbits8;
    unsigned long long int notDVDHbits8;
    unsigned long long int DVDHbits16notDVDHbits8notDVDHbits4notDVDHbits2;
    unsigned long long int DVDHbits16DVDHbits8notDVDHbits4notDVDHbits2;
    unsigned long long int notDVDHbits4;
    unsigned long long int DVDHbits16notDVDHbits8DVDHbits4DVDHbits2;
    unsigned long long int notDVDHbits16DVDHbits8DVDHbits4;
    unsigned long long int notDVDHbits16notDVDHbits8DVDHbits4DVDHbits2;
    unsigned long long int DVDHbits16notDVDHbits8;
    unsigned long long int DVDHbits16DVDHbits8notDVDHbits4DVDHbits2;
    unsigned long long int notDVDHbits2;
    unsigned long long int notDVDHbits16DVDHbits8DVDHbits4notDVDHbits2;
    unsigned long long int notDVDHbits16DVDHbits8;
    unsigned long long int notDVDHbits16notDVDHbits8notDVDHbits4;
    unsigned long long int DVDHbits16notDVDHbits8notDVDHbits4;
    unsigned long long int DVDHbits16DVDHbits8DVDHbits4notDVDHbits2;
    unsigned long long int notDVDHbits16DVDHbits8notDVDHbits4DVDHbits2;
    unsigned long long int notDVDHbits1;
    unsigned long long int notDVbits16;
    unsigned long long int notDVbits4;
    unsigned long long int notDVbits16notDVbits8DVbits4DVbits2;
    unsigned long long int notDVbits16DVbits8notDVbits4DVbits2;
    unsigned long long int notDVbits1;
    unsigned long long int DVbits16DVbits8DVbits4notDVbits2;
    unsigned long long int DVbits16notDVbits8notDVbits4;
    unsigned long long int DVbits16DVbits8DVbits4DVbits2;
    unsigned long long int DVbits16DVbits8;
    unsigned long long int DVbits16DVbits8notDVbits4notDVbits2;
    unsigned long long int DVbits16notDVbits8DVbits4notDVbits2;
    unsigned long long int notDVbits16notDVbits8notDVbits4;
    unsigned long long int notDVbits2;
    unsigned long long int DVbits16notDVbits8DVbits4DVbits2;
    unsigned long long int DVbits16DVbits8notDVbits4DVbits2;
    unsigned long long int notDVbits16DVbits8notDVbits4notDVbits2;
    unsigned long long int notDVbits16DVbits8DVbits4notDVbits2;
    unsigned long long int DVbits16notDVbits8notDVbits4notDVbits2;
    unsigned long long int DVbits16notDVbits8DVbits4;
    unsigned long long int notDVbits16notDVbits8;
    unsigned long long int DVbits16DVbits8notDVbits4;
    unsigned long long int DVbits16notDVbits8notDVbits4DVbits2;
    unsigned long long int DVbits16notDVbits8;
    unsigned long long int notDVbits8;
    unsigned long long int notDVbits16notDVbits8DVbits4notDVbits2;
    unsigned long long int notDVbits16notDVbits8DVbits4;
    unsigned long long int DVbits16DVbits8DVbits4;
    unsigned long long int notDVbits16DVbits8notDVbits4;
    unsigned long long int notDVbits16notDVbits8notDVbits4DVbits2;
    unsigned long long int notDVbits16notDVbits8notDVbits4notDVbits2;
    unsigned long long int notDVbits16DVbits8DVbits4;
    unsigned long long int notDVbits16DVbits8;
    unsigned long long int notDVbits16DVbits8DVbits4DVbits2;
    unsigned long long int DHbits1;
    unsigned long long int DVbits1;
    unsigned long long int DVDHbits1;
    unsigned long long int DHbits2;
    unsigned long long int DVbits2;
    unsigned long long int DVDHbits2;
    unsigned long long int DHbits4;
    unsigned long long int DVbits4;
    unsigned long long int DVDHbits4;
    unsigned long long int DHbits8;
    unsigned long long int DVbits8;
    unsigned long long int DVDHbits8;
    unsigned long long int DHbits16;
    unsigned long long int DVbits16;
    unsigned long long int DVDHbits16;
    unsigned long long int DVDHbits32;
    unsigned long long int carry0;
    unsigned long long int carry1;
    unsigned long long int carry2;
    unsigned long long int carry3;
    unsigned long long int carry4;
    unsigned long long int DVDHbithighcomp;
    unsigned long long int compDHneg5topos2;
    unsigned long long int DVpos3shiftorDVpos4shiftorDVpos5shiftorDVpos6shift;
    unsigned long long int DVpos3shiftorDVpos4shift;
    unsigned long long int DVpos3shiftorDVpos4shiftorDVpos5shiftorDVpos6shiftorDVpos7shiftorMatch;
    unsigned long long int DVpos5shiftorDVpos6shift;
    unsigned long long int DHpos7orDHpos6orDHpos5orDHpos4orDHpos3orDHpos2orDHpos1orDHzeroorDHneg1orDHneg2orDHneg3orDHneg4;
    unsigned long long int DHpos1orDHzero;
    unsigned long long int DHpos3orDHpos2orDHpos1orDHzero;
    unsigned long long int DHpos3orDHpos2;
    unsigned long long int DHpos7orDHpos6orDHpos5orDHpos4;
    unsigned long long int DHpos5orDHpos4;
    unsigned long long int DHpos3orDHpos2orDHpos1orDHzeroorDHneg1orDHneg2orDHneg3orDHneg4;
    unsigned long long int DHneg1orDHneg2orDHneg3orDHneg4;
    unsigned long long int DHneg1orDHneg2;
    unsigned long long int DHpos7orDHpos6;
    unsigned long long int DHneg3orDHneg4;

    int score;//Holds the NW score calculated from the last row of DHbit values
    unsigned long long int *matchvec = (unsigned long long int *) calloc(256, sizeof(unsigned long long int));


    char *iterate;
    n = strlen(s1);
    m = strlen(s2);
    n--;
    m--;
    if (n == 0 || m == 0)return 0;

    //*************************encode match strings A C G T N for string1
    //loop through string1 and store bits in matchA, matchC, etc.
    //position zero corresponds to column one in the score matrix, i.e., first character
    //so we start with i = 0 and bitmask = 1

    bitmask = 0x0000000000000001;
    for (i = 0, iterate = s1; i < n; ++i, ++iterate) {
        matchvec[(int) ((*iterate))] |= bitmask;
        bitmask <<= 1;
    }


    //intialize top row (penalty for initial gap, unless semi-global alignment is being done)
    DHneg5 = all_ones;
    DHneg4 = DHneg3 = DHneg2 = DHneg1 = DHzero = DHpos1 = DHpos2 = DHpos3 = DHpos4 = DHpos5 = DHpos6 = DHpos7 = 0;
    DHneg1orDHneg2 = (DHneg1 | DHneg2);
    DHpos5orDHpos4 = (DHpos5 | DHpos4);
    DHneg3orDHneg4 = (DHneg3 | DHneg4);
    DHpos7orDHpos6 = (DHpos7 | DHpos6);
    DHpos1orDHzero = (DHpos1 | DHzero);
    DHpos3orDHpos2 = (DHpos3 | DHpos2);
    DHpos3orDHpos2orDHpos1orDHzero = (DHpos3orDHpos2 | DHpos1orDHzero);
    DHneg1orDHneg2orDHneg3orDHneg4 = (DHneg1orDHneg2 | DHneg3orDHneg4);
    DHpos7orDHpos6orDHpos5orDHpos4 = (DHpos7orDHpos6 | DHpos5orDHpos4);
    DHpos3orDHpos2orDHpos1orDHzeroorDHneg1orDHneg2orDHneg3orDHneg4 = (DHpos3orDHpos2orDHpos1orDHzero |
                                                                      DHneg1orDHneg2orDHneg3orDHneg4);
    DHpos7orDHpos6orDHpos5orDHpos4orDHpos3orDHpos2orDHpos1orDHzeroorDHneg1orDHneg2orDHneg3orDHneg4 = (
            DHpos7orDHpos6orDHpos5orDHpos4 | DHpos3orDHpos2orDHpos1orDHzeroorDHneg1orDHneg2orDHneg3orDHneg4);
    DHbits1 = DHneg2 | DHneg4 | DHpos4 | DHpos2 | DHzero | DHpos6;
    DHbits2 = DHneg3orDHneg4 | DHpos1orDHzero | DHpos5orDHpos4;
    DHbits4 = DHneg1orDHneg2orDHneg3orDHneg4 | DHpos7orDHpos6orDHpos5orDHpos4;
    DHbits8 = DHpos3orDHpos2orDHpos1orDHzeroorDHneg1orDHneg2orDHneg3orDHneg4;
    DHbits16 = DHpos7orDHpos6orDHpos5orDHpos4orDHpos3orDHpos2orDHpos1orDHzeroorDHneg1orDHneg2orDHneg3orDHneg4;

    //recursion
    for (i = 0, iterate = s2; i < m; ++i, ++iterate) {
        Matches = matchvec[(int) (*iterate)];
        //Complement Matches
        NotMatches = ~Matches;

        //Finding the vertical values.         //Find 7s
        INITpos7s = DHneg5 & Matches;
        DVpos7shift = (((INITpos7s + DHneg5) ^ DHneg5) ^ INITpos7s);

        //set RemainingDHneg5
        RemainDHneg5 = DHneg5 ^ (DVpos7shift >> 1);
        //combine 7s and Matches
        DVpos7shiftorMatch = DVpos7shift | Matches;

        //Find 6s
        INITpos6s = (DHneg4 & DVpos7shiftorMatch);
        DVpos6shift = (((INITpos6s << 1) + RemainDHneg5) ^ RemainDHneg5) & NotMatches;
        //Find 5s
        INITpos5s = (DHneg3 & DVpos7shiftorMatch) | (DHneg4 & DVpos6shift);
        DVpos5shift = (((INITpos5s << 1) + RemainDHneg5) ^ RemainDHneg5) & NotMatches;
        //Find 4s
        INITpos4s = (DHneg2 & DVpos7shiftorMatch) | (DHneg3 & DVpos6shift) | (DHneg4 & DVpos5shift);
        DVpos4shift = (((INITpos4s << 1) + RemainDHneg5) ^ RemainDHneg5) & NotMatches;
        //Find 3s
        INITpos3s = (DHneg1 & DVpos7shiftorMatch) | (DHneg2 & DVpos6shift) | (DHneg3 & DVpos5shift) |
                    (DHneg4 & DVpos4shift);
        DVpos3shift = (((INITpos3s << 1) + RemainDHneg5) ^ RemainDHneg5) & NotMatches;
        //set DVnot7to3shiftorMatch
        DVnot7to3shiftorMatch = ~(DVpos7shiftorMatch | DVpos6shift | DVpos5shift | DVpos4shift | DVpos3shift);
        DVpos3shiftorDVpos4shift = (DVpos3shift | DVpos4shift);
        DVpos5shiftorDVpos6shift = (DVpos5shift | DVpos6shift);
        DVpos3shiftorDVpos4shiftorDVpos5shiftorDVpos6shift = (DVpos3shiftorDVpos4shift | DVpos5shiftorDVpos6shift);
        DVpos3shiftorDVpos4shiftorDVpos5shiftorDVpos6shiftorDVpos7shiftorMatch = (
                DVpos3shiftorDVpos4shiftorDVpos5shiftorDVpos6shift | DVpos7shiftorMatch);
        DVbits1 = DVpos6shift | DVpos4shift | DVnot7to3shiftorMatch;
        DVbits2 = DVpos5shiftorDVpos6shift | DVnot7to3shiftorMatch;
        DVbits4 = DVpos7shiftorMatch | DVnot7to3shiftorMatch;
        DVbits8 = DVpos3shiftorDVpos4shiftorDVpos5shiftorDVpos6shiftorDVpos7shiftorMatch;
        DVbits16 = 0;
        carry0 = (DHbits1 & DVbits1);
        carry1 = ((DHbits2 & DVbits2) | ((DHbits2 ^ DVbits2) & carry0));
        carry2 = ((DHbits4 & DVbits4) | ((DHbits4 ^ DVbits4) & carry1));
        carry3 = ((DHbits8 & DVbits8) | ((DHbits8 ^ DVbits8) & carry2));
        carry4 = ((DHbits16 & DVbits16) | ((DHbits16 ^ DVbits16) & carry3));
        DVDHbits1 = (DHbits1 ^ DVbits1);
        DVDHbits2 = ((DHbits2 ^ DVbits2) ^ carry0);
        DVDHbits4 = ((DHbits4 ^ DVbits4) ^ carry1);
        DVDHbits8 = ((DHbits8 ^ DVbits8) ^ carry2);
        DVDHbits16 = ((DHbits16 ^ DVbits16) ^ carry3);
        DVDHbits32 = carry4;
        DVDHbithighcomp = ~DVDHbits16;
        DVDHbits1 &= DVDHbithighcomp;
        DVDHbits2 &= DVDHbithighcomp;
        DVDHbits4 &= DVDHbithighcomp;
        DVDHbits8 &= DVDHbithighcomp;
        DVDHbits16 &= DVDHbithighcomp;
        DVDHbits32 &= DVDHbithighcomp;
        DVDHbits1 <<= 1;
        DVDHbits2 <<= 1;
        DVDHbits4 <<= 1;
        DVDHbits8 <<= 1;
        DVDHbits16 <<= 1;
        DVDHbits32 <<= 1;
        DVbits1 = DVDHbits1;
        DVbits2 = DVDHbits2;
        DVbits4 = DVDHbits4;
        DVbits8 = DVDHbits8;
        DVbits16 = DVDHbits16;

        //Finding the horizontal values
        //Remove matches from DH values except 7
        DHneg5topos2 = (DHneg5 | DHneg4 | DHneg3 | DHneg2 | DHneg1 | DHzero | DHpos1 | DHpos2) & NotMatches;
        compDHneg5topos2 = all_ones ^ DHneg5topos2;
        //Representing the range -5 to 2 as 2;
        DHbits1 |= DHneg5topos2;
        DHbits2 &= compDHneg5topos2;
        DHbits4 &= compDHneg5topos2;
        DHbits8 |= DHneg5topos2;
        DHbits16 |= DHneg5topos2;
        DHbits1 &= NotMatches;
        DHbits2 &= NotMatches;
        DHbits4 |= Matches;
        DHbits8 &= NotMatches;
        DHbits16 |= Matches;
        carry0 = (DHbits1 & DVDHbits1);
        carry1 = ((DHbits2 & DVDHbits2) | ((DHbits2 ^ DVDHbits2) & carry0));
        carry2 = ((DHbits4 & DVDHbits4) | ((DHbits4 ^ DVDHbits4) & carry1));
        carry3 = ((DHbits8 & DVDHbits8) | ((DHbits8 ^ DVDHbits8) & carry2));
        carry4 = ((DHbits16 & DVDHbits16) | ((DHbits16 ^ DVDHbits16) & carry3));
        DVDHbits1 = (DHbits1 ^ DVDHbits1);
        DVDHbits2 = ((DHbits2 ^ DVDHbits2) ^ carry0);
        DVDHbits4 = ((DHbits4 ^ DVDHbits4) ^ carry1);
        DVDHbits8 = ((DHbits8 ^ DVDHbits8) ^ carry2);
        DVDHbits16 = ((DHbits16 ^ DVDHbits16) ^ carry3);
        DVDHbits32 = carry4;
        //Convert everything that is positive to 0
        DVDHbits1 &= DVDHbits16;
        DVDHbits2 &= DVDHbits16;
        DVDHbits4 &= DVDHbits16;
        DVDHbits8 &= DVDHbits16;
        DVDHbits16 &= DVDHbits16;
        DVDHbits32 &= DVDHbits16;
        DHbits1 = DVDHbits1;
        DHbits2 = DVDHbits2;
        DHbits4 = DVDHbits4;
        DHbits8 = DVDHbits8;
        DHbits16 = DVDHbits16;
        notDVDHbits16 = (~DVDHbits16);
        notDVDHbits8 = ~DVDHbits8;
        notDVDHbits16notDVDHbits8 = notDVDHbits16 & notDVDHbits8;
        notDVDHbits4 = ~DVDHbits4;
        DVDHbits16DVDHbits8 = DVDHbits16 & DVDHbits8;
        notDVDHbits16notDVDHbits8notDVDHbits4 = notDVDHbits16notDVDHbits8 & notDVDHbits4;
        DVDHbits16DVDHbits8notDVDHbits4 = DVDHbits16DVDHbits8 & notDVDHbits4;
        notDVDHbits2 = ~DVDHbits2;
        DVDHbits16DVDHbits8DVDHbits4 = DVDHbits16DVDHbits8 & DVDHbits4;
        DVDHbits16DVDHbits8DVDHbits4notDVDHbits2 = DVDHbits16DVDHbits8DVDHbits4 & notDVDHbits2;
        DVDHbits16DVDHbits8notDVDHbits4DVDHbits2 = DVDHbits16DVDHbits8notDVDHbits4 & DVDHbits2;
        DVDHbits16DVDHbits8notDVDHbits4notDVDHbits2 = DVDHbits16DVDHbits8notDVDHbits4 & notDVDHbits2;
        notDVDHbits16notDVDHbits8notDVDHbits4notDVDHbits2 = notDVDHbits16notDVDHbits8notDVDHbits4 & notDVDHbits2;
        DVDHbits16DVDHbits8DVDHbits4DVDHbits2 = DVDHbits16DVDHbits8DVDHbits4 & DVDHbits2;
        notDVDHbits1 = ~DVDHbits1;
        DHneg5 = notDVDHbits16notDVDHbits8notDVDHbits4notDVDHbits2 & notDVDHbits1;
        DHneg4 = DVDHbits16DVDHbits8DVDHbits4DVDHbits2 & DVDHbits1;
        DHneg3 = DVDHbits16DVDHbits8DVDHbits4DVDHbits2 & notDVDHbits1;
        DHneg2 = DVDHbits16DVDHbits8DVDHbits4notDVDHbits2 & DVDHbits1;
        DHneg1 = DVDHbits16DVDHbits8DVDHbits4notDVDHbits2 & notDVDHbits1;
        DHzero = DVDHbits16DVDHbits8notDVDHbits4DVDHbits2 & DVDHbits1;
        DHpos1 = DVDHbits16DVDHbits8notDVDHbits4DVDHbits2 & notDVDHbits1;
        DHpos2 = DVDHbits16DVDHbits8notDVDHbits4notDVDHbits2 & DVDHbits1;

    }

    //find scores in last row
    score = -5 * m;

    bitmask = 0x0000000000000001;

    for (i = 0; i < n; i++) {
        score -=
                ((DVDHbits1 & bitmask) >> i) * 1 + ((DVDHbits2 & bitmask) >> i) * 2 + ((DVDHbits4 & bitmask) >> i) * 4 +
                ((DVDHbits8 & bitmask) >> i) * 8 - ((DVDHbits16 & bitmask) >> i) * 16 + 5;

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



