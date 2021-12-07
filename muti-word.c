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

//Uses 310 operations, counting |, &, ^, and +

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define wordsize 64

int bitwise_alignment(char *s1, char *s2, int words) {

    int debug = 1;
    int wordsizem1 = wordsize - 1;
    int i, j, k, n, m;
    unsigned long long int bitmask;
    unsigned long long int Matches;
    unsigned long long int **matchvec;
    unsigned long long int *matchvecmem;
    unsigned long long int *matchv;
    unsigned long long int NotMatches;
    unsigned long long int all_ones = ~0x0000000000000000;
    unsigned long long int one = 0x0000000000000001;
    unsigned long long int sixtythreebitmask = ~((unsigned long long int) 1 << wordsizem1);

    unsigned long long int DVpos7shift, DVpos6shift, DVpos5shift, DVpos4shift, DVpos3shift, DVpos2shift, DVpos1shift, DVzeroshift, DVneg1shift, DVneg2shift, DVneg3shift, DVneg4shift, DVneg5shift;
    unsigned long long int DHpos7temp, DHpos6temp, DHpos5temp, DHpos4temp, DHpos3temp, DHpos2temp, DHpos1temp, DHzerotemp, DHneg1temp, DHneg2temp, DHneg3temp, DHneg4temp, DHneg5temp;
    unsigned long long int *DHpos7, *DHpos6, *DHpos5, *DHpos4, *DHpos3, *DHpos2, *DHpos1, *DHzero, *DHneg1, *DHneg2, *DHneg3, *DHneg4, *DHneg5;
    unsigned long long int INITpos7s, INITpos6s, INITpos5s, INITpos4s, INITpos3s, INITpos2s, INITpos1s, INITzeros, INITneg1s, INITneg2s, INITneg3s, INITneg4s;
    unsigned long long int INITpos7sprevbit, INITpos6sprevbit, INITpos5sprevbit, INITpos4sprevbit, INITpos3sprevbit, INITpos2sprevbit, INITpos1sprevbit, INITzerosprevbit, INITneg1sprevbit, INITneg2sprevbit, INITneg3sprevbit, INITneg4sprevbit;
    unsigned long long int OverFlow0, OverFlow1, OverFlow2, OverFlow3, OverFlow4, OverFlow5, OverFlow6, OverFlow7, OverFlow8, OverFlow9, OverFlow10, OverFlow11;

    unsigned long long int DVpos6shiftNotMatch, DVpos5shiftNotMatch, DVpos4shiftNotMatch, DVpos3shiftNotMatch;

    unsigned long long int RemainDHneg5;
    unsigned long long int DHneg5topos2;
    unsigned long long int DVpos7shiftorMatch;
    unsigned long long int DVnot7to3shiftorMatch;
    unsigned long long int DHpos7orMatch;
    unsigned long long int DHneg3temporDHneg2temp;
    unsigned long long int add2;
    unsigned long long int add1;
    unsigned long long int add4;
    unsigned long long int DHpos3temporDHpos4temporDHpos5temporDHpos6temporDHpos7temp;
    unsigned long long int add8;
    unsigned long long int DHpos3temporDHpos4temporDHpos5temporDHpos6temp;
    unsigned long long int DHpos3temporDHpos4temp;
    unsigned long long int DHneg1temporDHzerotemporDHpos1temporDHpos2temp;
    unsigned long long int DHpos1temporDHpos2temp;
    unsigned long long int add16;
    unsigned long long int DHneg1temporDHzerotemp;
    unsigned long long int DHpos5temporDHpos6temp;

    unsigned long long int sum;
    unsigned long long int highone = one << (wordsize - 1);
    unsigned long long int initval;
    char *iterate;
    int score;

    int counter = 0, w = 0;

    n = strlen(s1);
    m = strlen(s2);

    n--;
    m--;
    if (n == 0 || m == 0)return 0;

    DHpos7 = calloc(words, sizeof(unsigned long long int));
    DHpos6 = calloc(words, sizeof(unsigned long long int));
    DHpos5 = calloc(words, sizeof(unsigned long long int));
    DHpos4 = calloc(words, sizeof(unsigned long long int));
    DHpos3 = calloc(words, sizeof(unsigned long long int));
    DHpos2 = calloc(words, sizeof(unsigned long long int));
    DHpos1 = calloc(words, sizeof(unsigned long long int));
    DHzero = calloc(words, sizeof(unsigned long long int));
    DHneg1 = calloc(words, sizeof(unsigned long long int));
    DHneg2 = calloc(words, sizeof(unsigned long long int));
    DHneg3 = calloc(words, sizeof(unsigned long long int));
    DHneg4 = calloc(words, sizeof(unsigned long long int));
    DHneg5 = calloc(words, sizeof(unsigned long long int));



    //*************************encode match strings A C G T N for string1
    //loop through string1 and store bits in matchA, matchC, etc.
    //position zero corresponds to column one in the score matrix, i.e., first character
    //so we start with i = 0 and bitmask = 1
    //below is optimized
    bitmask = 0x0000000000000001;
    matchvec = (unsigned long long int **) calloc(256, sizeof(unsigned long long int *));
    matchvecmem = (unsigned long long int *) calloc(words * 256, sizeof(unsigned long long int));
    for (i = 0; i < 256; ++i)
        matchvec[i] = &matchvecmem[i * words];
    for (iterate = s1, i = 0; i < n; ++i, ++iterate) {
        matchvec[(*iterate)][w] |= bitmask;
        bitmask <<= 1;
        ++counter;
        if (counter == 63) {
            counter = 0;
            w++;
            bitmask = one;
        }
    }



    //intialize top row (penalty for initial gap)
    for (i = 0; i < words; ++i) {
        DHneg5[i] = sixtythreebitmask;
        DHneg4[i] = DHneg3[i] = DHneg2[i] = DHneg1[i] = DHzero[i] = DHpos1[i] = DHpos2[i] = DHpos3[i] = DHpos4[i] = DHpos5[i] = DHpos6[i] = DHpos7[i] = 0;

    }
    //recursion
    for (i = 0, iterate = s2; i < m; ++i, ++iterate) {
        //initialize OverFlow
        OverFlow0 = OverFlow1 = OverFlow2 = OverFlow3 = OverFlow4 = OverFlow5 = OverFlow6 = OverFlow7 = OverFlow8 = OverFlow9 = OverFlow10 = OverFlow11 = 0;
        INITpos7sprevbit = INITpos6sprevbit = INITpos5sprevbit = INITpos4sprevbit = INITpos3sprevbit = INITpos2sprevbit = INITpos1sprevbit = INITzerosprevbit = INITneg1sprevbit = INITneg2sprevbit = INITneg3sprevbit = INITneg4sprevbit = 0;

        matchv = matchvec[*iterate];
        for (j = 0; j < words; ++j) {
            DHpos7temp = DHpos7[j];
            DHpos6temp = DHpos6[j];
            DHpos5temp = DHpos5[j];
            DHpos4temp = DHpos4[j];
            DHpos3temp = DHpos3[j];
            DHpos2temp = DHpos2[j];
            DHpos1temp = DHpos1[j];
            DHzerotemp = DHzero[j];
            DHneg1temp = DHneg1[j];
            DHneg2temp = DHneg2[j];
            DHneg3temp = DHneg3[j];
            DHneg4temp = DHneg4[j];
            DHneg5temp = DHneg5[j];

            Matches = *matchv;
            ++matchv;
            //Complement Matches
            NotMatches = ~Matches;
            //Finding the vertical values
            //Find 7s
            INITpos7s = DHneg5temp & Matches;
            sum = (INITpos7s + DHneg5temp) + OverFlow0;
            DVpos7shift = ((sum ^ DHneg5temp) ^ INITpos7s) & sixtythreebitmask;
            OverFlow0 = sum >> wordsizem1;

            //set RemainingDHneg5
            RemainDHneg5 = DHneg5temp ^ (INITpos7s) & sixtythreebitmask;
            //combine 7s and Matches
            DVpos7shiftorMatch = DVpos7shift | Matches;

            //Find 6s
            INITpos6s = (DHneg4temp & DVpos7shiftorMatch);
            initval = ((INITpos6s << 1) | INITpos6sprevbit);
            INITpos6sprevbit = (initval >> wordsizem1);
            initval &= sixtythreebitmask;
            sum = initval + RemainDHneg5 + OverFlow1;
            DVpos6shift = (sum ^ RemainDHneg5);
            DVpos6shiftNotMatch = DVpos6shift & NotMatches;
            OverFlow1 = sum >> wordsizem1;
            //Find 5s
            INITpos5s = (DHneg3temp & DVpos7shiftorMatch) | (DHneg4temp & DVpos6shiftNotMatch);
            initval = ((INITpos5s << 1) | INITpos5sprevbit);
            INITpos5sprevbit = (initval >> wordsizem1);
            initval &= sixtythreebitmask;
            sum = initval + RemainDHneg5 + OverFlow2;
            DVpos5shift = (sum ^ RemainDHneg5);
            DVpos5shiftNotMatch = DVpos5shift & NotMatches;
            OverFlow2 = sum >> wordsizem1;
            //Find 4s
            INITpos4s = (DHneg2temp & DVpos7shiftorMatch) | (DHneg3temp & DVpos6shiftNotMatch) |
                        (DHneg4temp & DVpos5shiftNotMatch);
            initval = ((INITpos4s << 1) | INITpos4sprevbit);
            INITpos4sprevbit = (initval >> wordsizem1);
            initval &= sixtythreebitmask;
            sum = initval + RemainDHneg5 + OverFlow3;
            DVpos4shift = (sum ^ RemainDHneg5);
            DVpos4shiftNotMatch = DVpos4shift & NotMatches;
            OverFlow3 = sum >> wordsizem1;
            //Find 3s
            INITpos3s = (DHneg1temp & DVpos7shiftorMatch) | (DHneg2temp & DVpos6shiftNotMatch) |
                        (DHneg3temp & DVpos5shiftNotMatch) | (DHneg4temp & DVpos4shiftNotMatch);
            initval = ((INITpos3s << 1) | INITpos3sprevbit);
            INITpos3sprevbit = (initval >> wordsizem1);
            initval &= sixtythreebitmask;
            sum = initval + RemainDHneg5 + OverFlow4;
            DVpos3shift = (sum ^ RemainDHneg5);
            DVpos3shiftNotMatch = DVpos3shift & NotMatches;
            OverFlow4 = sum >> wordsizem1;
            //set DVnot7to3shiftorMatch
            DVnot7to3shiftorMatch = ~(DVpos7shiftorMatch | DVpos6shift | DVpos5shift | DVpos4shift | DVpos3shift);
            //Find 2s
            INITpos2s = ((DHzerotemp & DVpos7shiftorMatch) | (DHneg1temp & DVpos6shiftNotMatch) |
                         (DHneg2temp & DVpos5shiftNotMatch) | (DHneg3temp & DVpos4shiftNotMatch) |
                         (DHneg4temp & DVpos3shiftNotMatch) | (DHneg5temp & DVnot7to3shiftorMatch));
            DVpos2shift = (INITpos2s << 1) | INITpos2sprevbit;
            INITpos2sprevbit = (INITpos2s & sixtythreebitmask) >> (wordsizem1 - 1);
            //Find 1s
            INITpos1s = ((DHpos1temp & DVpos7shiftorMatch) | (DHzerotemp & DVpos6shiftNotMatch) |
                         (DHneg1temp & DVpos5shiftNotMatch) | (DHneg2temp & DVpos4shiftNotMatch) |
                         (DHneg3temp & DVpos3shiftNotMatch) | (DHneg4temp & DVnot7to3shiftorMatch));
            DVpos1shift = (INITpos1s << 1) | INITpos1sprevbit;
            INITpos1sprevbit = (INITpos1s & sixtythreebitmask) >> (wordsizem1 - 1);
            //Find 0s
            INITzeros = ((DHpos2temp & DVpos7shiftorMatch) | (DHpos1temp & DVpos6shiftNotMatch) |
                         (DHzerotemp & DVpos5shiftNotMatch) | (DHneg1temp & DVpos4shiftNotMatch) |
                         (DHneg2temp & DVpos3shiftNotMatch) | (DHneg3temp & DVnot7to3shiftorMatch));
            DVzeroshift = (INITzeros << 1) | INITzerosprevbit;
            INITzerosprevbit = (INITzeros & sixtythreebitmask) >> (wordsizem1 - 1);
            //Find -1s
            INITneg1s = ((DHpos3temp & DVpos7shiftorMatch) | (DHpos2temp & DVpos6shiftNotMatch) |
                         (DHpos1temp & DVpos5shiftNotMatch) | (DHzerotemp & DVpos4shiftNotMatch) |
                         (DHneg1temp & DVpos3shiftNotMatch) | (DHneg2temp & DVnot7to3shiftorMatch));
            DVneg1shift = (INITneg1s << 1) | INITneg1sprevbit;
            INITneg1sprevbit = (INITneg1s & sixtythreebitmask) >> (wordsizem1 - 1);
            //Find -2s
            INITneg2s = ((DHpos4temp & DVpos7shiftorMatch) | (DHpos3temp & DVpos6shiftNotMatch) |
                         (DHpos2temp & DVpos5shiftNotMatch) | (DHpos1temp & DVpos4shiftNotMatch) |
                         (DHzerotemp & DVpos3shiftNotMatch) | (DHneg1temp & DVnot7to3shiftorMatch));
            DVneg2shift = (INITneg2s << 1) | INITneg2sprevbit;
            INITneg2sprevbit = (INITneg2s & sixtythreebitmask) >> (wordsizem1 - 1);
            //Find -3s
            INITneg3s = ((DHpos5temp & DVpos7shiftorMatch) | (DHpos4temp & DVpos6shiftNotMatch) |
                         (DHpos3temp & DVpos5shiftNotMatch) | (DHpos2temp & DVpos4shiftNotMatch) |
                         (DHpos1temp & DVpos3shiftNotMatch) | (DHzerotemp & DVnot7to3shiftorMatch));
            DVneg3shift = (INITneg3s << 1) | INITneg3sprevbit;
            INITneg3sprevbit = (INITneg3s & sixtythreebitmask) >> (wordsizem1 - 1);
            //Find -4s
            INITneg4s = ((DHpos6temp & DVpos7shiftorMatch) | (DHpos5temp & DVpos6shiftNotMatch) |
                         (DHpos4temp & DVpos5shiftNotMatch) | (DHpos3temp & DVpos4shiftNotMatch) |
                         (DHpos2temp & DVpos3shiftNotMatch) | (DHpos1temp & DVnot7to3shiftorMatch));
            DVneg4shift = (INITneg4s << 1) | INITneg4sprevbit;
            INITneg4sprevbit = (INITneg4s & sixtythreebitmask) >> (wordsizem1 - 1);
            //Find -5s
            DVneg5shift = all_ones ^
                          (DVpos7shift | DVpos6shift | DVpos5shift | DVpos4shift | DVpos3shift | DVpos2shift |
                           DVpos1shift | DVzeroshift | DVneg1shift | DVneg2shift | DVneg3shift | DVneg4shift);

            //Finding the horizontal values
            //Remove matches from DH values except 7
            DHpos3temp &= NotMatches;
            DHpos4temp &= NotMatches;
            DHpos5temp &= NotMatches;
            DHpos6temp &= NotMatches;
            //combine 7s and Matches
            DHpos7orMatch = DHpos7temp | Matches;
            //group -5topos2
            DHneg5topos2 = all_ones ^ (DHpos7orMatch | DHpos6temp | DHpos5temp | DHpos4temp | DHpos3temp);
            //Find -4s
            DHneg4temp = ((DVpos6shift & DHpos7orMatch) | (DVpos5shift & DHpos6temp) | (DVpos4shift & DHpos5temp) |
                          (DVpos3shift & DHpos4temp) | (DVpos2shift & DHpos3temp) | (DVpos1shift & DHneg5topos2));
            //Find -3s
            DHneg3temp = ((DVpos5shift & DHpos7orMatch) | (DVpos4shift & DHpos6temp) | (DVpos3shift & DHpos5temp) |
                          (DVpos2shift & DHpos4temp) | (DVpos1shift & DHpos3temp) | (DVzeroshift & DHneg5topos2));
            //Find -2s
            DHneg2temp = ((DVpos4shift & DHpos7orMatch) | (DVpos3shift & DHpos6temp) | (DVpos2shift & DHpos5temp) |
                          (DVpos1shift & DHpos4temp) | (DVzeroshift & DHpos3temp) | (DVneg1shift & DHneg5topos2));
            //Find -1s
            DHneg1temp = ((DVpos3shift & DHpos7orMatch) | (DVpos2shift & DHpos6temp) | (DVpos1shift & DHpos5temp) |
                          (DVzeroshift & DHpos4temp) | (DVneg1shift & DHpos3temp) | (DVneg2shift & DHneg5topos2));
            //Find 0s
            DHzerotemp = ((DVpos2shift & DHpos7orMatch) | (DVpos1shift & DHpos6temp) | (DVzeroshift & DHpos5temp) |
                          (DVneg1shift & DHpos4temp) | (DVneg2shift & DHpos3temp) | (DVneg3shift & DHneg5topos2));
            //Find 1s
            DHpos1temp = ((DVpos1shift & DHpos7orMatch) | (DVzeroshift & DHpos6temp) | (DVneg1shift & DHpos5temp) |
                          (DVneg2shift & DHpos4temp) | (DVneg3shift & DHpos3temp) | (DVneg4shift & DHneg5topos2));
            //Find 2s
            DHpos2temp = ((DVzeroshift & DHpos7orMatch) | (DVneg1shift & DHpos6temp) | (DVneg2shift & DHpos5temp) |
                          (DVneg3shift & DHpos4temp) | (DVneg4shift & DHpos3temp) | (DVneg5shift & DHneg5topos2));
            //Find 3s
            DHpos3temp = ((DVneg1shift & DHpos7orMatch) | (DVneg2shift & DHpos6temp) | (DVneg3shift & DHpos5temp) |
                          (DVneg4shift & DHpos4temp) | (DVneg5shift & DHpos3temp));
            //Find 4s
            DHpos4temp = ((DVneg2shift & DHpos7orMatch) | (DVneg3shift & DHpos6temp) | (DVneg4shift & DHpos5temp) |
                          (DVneg5shift & DHpos4temp));
            //Find 5s
            DHpos5temp = ((DVneg3shift & DHpos7orMatch) | (DVneg4shift & DHpos6temp) | (DVneg5shift & DHpos5temp));
            //Find 6s
            DHpos6temp = ((DVneg4shift & DHpos7orMatch) | (DVneg5shift & DHpos6temp));
            //Find 7s
            DHpos7temp = ((DVneg5shift & DHpos7orMatch));
            //Find -5s
            DHneg5temp = sixtythreebitmask & (all_ones ^
                                              (DHneg4temp | DHneg3temp | DHneg2temp | DHneg1temp | DHzerotemp |
                                               DHpos1temp | DHpos2temp | DHpos3temp | DHpos4temp | DHpos5temp |
                                               DHpos6temp | DHpos7temp));
            DHpos7[j] = DHpos7temp;
            DHpos6[j] = DHpos6temp;
            DHpos5[j] = DHpos5temp;
            DHpos4[j] = DHpos4temp;
            DHpos3[j] = DHpos3temp;
            DHpos2[j] = DHpos2temp;
            DHpos1[j] = DHpos1temp;
            DHzero[j] = DHzerotemp;
            DHneg1[j] = DHneg1temp;
            DHneg2[j] = DHneg2temp;
            DHneg3[j] = DHneg3temp;
            DHneg4[j] = DHneg4temp;
            DHneg5[j] = DHneg5temp;

        }
    }
    //find scores in last row

    score = -5 * m;
    bitmask = one;
    for (j = 0; j < words; ++j) {
        DHpos7temp = DHpos7[j];
        DHpos6temp = DHpos6[j];
        DHpos5temp = DHpos5[j];
        DHpos4temp = DHpos4[j];
        DHpos3temp = DHpos3[j];
        DHpos2temp = DHpos2[j];
        DHpos1temp = DHpos1[j];
        DHzerotemp = DHzero[j];
        DHneg1temp = DHneg1[j];
        DHneg2temp = DHneg2[j];
        DHneg3temp = DHneg3[j];
        DHneg4temp = DHneg4[j];
        DHneg5temp = DHneg5[j];
        DHpos1temporDHpos2temp = (DHpos1temp | DHpos2temp);
        DHneg1temporDHzerotemp = (DHneg1temp | DHzerotemp);
        DHpos3temporDHpos4temp = (DHpos3temp | DHpos4temp);
        DHpos5temporDHpos6temp = (DHpos5temp | DHpos6temp);
        DHpos5temporDHpos6temp = (DHpos5temp | DHpos6temp);
        DHpos1temporDHpos2temp = (DHpos1temp | DHpos2temp);
        DHneg3temporDHneg2temp = (DHneg3temp | DHneg2temp);
        DHpos3temporDHpos4temporDHpos5temporDHpos6temp = (DHpos3temporDHpos4temp | DHpos5temporDHpos6temp);
        DHneg1temporDHzerotemporDHpos1temporDHpos2temp = (DHneg1temporDHzerotemp | DHpos1temporDHpos2temp);
        DHpos3temporDHpos4temporDHpos5temporDHpos6temporDHpos7temp = (DHpos3temporDHpos4temporDHpos5temporDHpos6temp |
                                                                      DHpos7temp);
        add1 = DHpos4temp | DHneg2temp | DHneg4temp | DHpos6temp | DHzerotemp | DHpos2temp;
        add2 = DHpos5temporDHpos6temp | DHpos1temporDHpos2temp | DHneg3temporDHneg2temp;
        add4 = DHpos7temp | DHneg1temporDHzerotemporDHpos1temporDHpos2temp;
        add8 = DHpos3temporDHpos4temporDHpos5temporDHpos6temporDHpos7temp;
        add16 = 0;


        for (i = j * wordsizem1; i < (j + 1) * wordsizem1 && i < n; ++i) {
            score += (add1 & bitmask) * 1 + (add2 & bitmask) * 2 + (add4 & bitmask) * 4 + (add8 & bitmask) * 8 +
                     (add16 & bitmask) * 16 - 5;
            add1 >>= 1;
            add2 >>= 1;
            add4 >>= 1;
            add8 >>= 1;
            add16 >>= 1;

        }
    }
    free(DHpos7);
    free(DHpos6);
    free(DHpos5);
    free(DHpos4);
    free(DHpos3);
    free(DHpos2);
    free(DHpos1);
    free(DHzero);
    free(DHneg1);
    free(DHneg2);
    free(DHneg3);
    free(DHneg4);
    free(DHneg5);

    free(matchvecmem);
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



