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

//Uses 202 operations, counting |, &, ^, and +

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define wordsize 64

int bitwise_alignment(char *s1, char *s2, int words) {

    int debug = 1;
    int wordsizem1 = wordsize - 1;
    int wordsizem2 = wordsize - 2;
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
    unsigned long long int INITpos7s, INITpos6s, INITpos5s, INITpos4s, INITpos3s, INITpos2s, INITpos1s, INITzeros, INITneg1s, INITneg2s, INITneg3s, INITneg4s;
    unsigned long long int DHpos7, DHpos6, DHpos5, DHpos4, DHpos3, DHpos2, DHpos1, DHzero, DHneg1, DHneg2, DHneg3, DHneg4, DHneg5;
    unsigned long long int INITpos7sprevbit, INITpos6sprevbit, INITpos5sprevbit, INITpos4sprevbit, INITpos3sprevbit, INITpos2sprevbit, INITpos1sprevbit, INITzerosprevbit, INITneg1sprevbit, INITneg2sprevbit, INITneg3sprevbit, INITneg4sprevbit;
    unsigned long long int OverFlow0, OverFlow1, OverFlow2, OverFlow3, OverFlow4, OverFlow5, OverFlow6, OverFlow7, OverFlow8, OverFlow9, OverFlow10, OverFlow11;

    unsigned long long int DVpos6shiftNotMatch, DVpos5shiftNotMatch, DVpos4shiftNotMatch, DVpos3shiftNotMatch;
    unsigned long long int RemainDHneg5;
    unsigned long long int DHneg5topos2;
    unsigned long long int DVpos7shiftorMatch;
    unsigned long long int DVnot7to3shiftorMatch;
    unsigned long long int DHpos7orMatch;

    //unsigned long long int DHneg3toDHneg2;
    //unsigned long long int DHneg1toDHzer0;
    //unsigned long long int DHpos1toDHpos2;
    //unsigned long long int DHpos3toDHpos4;
    //unsigned long long int DHpos5toDHpos6;
    //unsigned long long int DHneg1topos2;
    //unsigned long long int DHpos3topos6;
    unsigned long long int DHbits16DHbits8notDHbits4notDHbits2;
    unsigned long long int notDHbits4;
    unsigned long long int DHbits16notDHbits8DHbits4notDHbits2;
    unsigned long long int notDHbits16;
    unsigned long long int DHbits16notDHbits8notDHbits4DHbits2;
    unsigned long long int DHbits16notDHbits8notDHbits4notDHbits2;
    unsigned long long int notDHbits2;
    unsigned long long int DHbits16DHbits8notDHbits4;
    unsigned long long int notDHbits16notDHbits8notDHbits4notDHbits2;
    unsigned long long int notDHbits16notDHbits8DHbits4notDHbits2;
    unsigned long long int notDHbits16notDHbits8notDHbits4DHbits2;
    unsigned long long int notDHbits16DHbits8DHbits4DHbits2;
    unsigned long long int notDHbits16notDHbits8DHbits4DHbits2;
    unsigned long long int notDHbits16notDHbits8DHbits4;
    unsigned long long int notDHbits1;
    unsigned long long int DHbits16notDHbits8notDHbits4;
    unsigned long long int notDHbits16DHbits8DHbits4notDHbits2;
    unsigned long long int notDHbits8;
    unsigned long long int notDHbits16DHbits8notDHbits4DHbits2;
    unsigned long long int notDHbits16DHbits8notDHbits4;
    unsigned long long int DHbits16notDHbits8DHbits4DHbits2;
    unsigned long long int notDHbits16notDHbits8notDHbits4;
    unsigned long long int DHbits16notDHbits8DHbits4;
    unsigned long long int DHbits16DHbits8DHbits4notDHbits2;
    unsigned long long int DHbits16notDHbits8;
    unsigned long long int notDHbits16DHbits8DHbits4;
    unsigned long long int DHbits16DHbits8;
    unsigned long long int DHbits16DHbits8DHbits4DHbits2;
    unsigned long long int DHbits16DHbits8DHbits4;
    unsigned long long int notDHbits16notDHbits8;
    unsigned long long int DHbits16DHbits8notDHbits4DHbits2;
    unsigned long long int notDHbits16DHbits8notDHbits4notDHbits2;
    unsigned long long int notDHbits16DHbits8;
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
    unsigned long long int *DVDHbits16;
    unsigned long long int *DVDHbits8;
    unsigned long long int *DVDHbits4;
    unsigned long long int *DVDHbits2;
    unsigned long long int *DVDHbits1;
    unsigned long long int *DVDHbits32;
    unsigned long long int carry0;
    unsigned long long int carry1;
    unsigned long long int carry2;
    unsigned long long int carry3;
    unsigned long long int carry4;
    unsigned long long int DVDHbithighcomp;
    unsigned long long int compDHneg5topos2;
    unsigned long long int DVpos3shiftorDVpos4shiftorDVpos5shiftorDVpos6shiftorDVpos7shiftorMatch;
    unsigned long long int DVpos3shiftorDVpos4shift;
    unsigned long long int DVbits4;
    unsigned long long int DVbits16;
    unsigned long long int DVpos3shiftorDVpos4shiftorDVpos5shiftorDVpos6shift;
    unsigned long long int DVbits1;
    unsigned long long int DVbits8;
    unsigned long long int DVbits2;
    unsigned long long int DVpos5shiftorDVpos6shift;
    unsigned long long int DHbits2;
    unsigned long long int DHpos7orDHpos6orDHpos5orDHpos4orDHpos3orDHpos2orDHpos1orDHzeroorDHneg1orDHneg2orDHneg3orDHneg4;
    unsigned long long int DHpos1orDHzero;
    unsigned long long int DHpos3orDHpos2orDHpos1orDHzero;
    unsigned long long int DHbits1;
    unsigned long long int DHbits16;
    unsigned long long int DHpos3orDHpos2;
    unsigned long long int DHpos7orDHpos6orDHpos5orDHpos4;
    unsigned long long int DHpos7orDHpos6;
    unsigned long long int DHpos3orDHpos2orDHpos1orDHzeroorDHneg1orDHneg2orDHneg3orDHneg4;
    unsigned long long int DHbits8;
    unsigned long long int DHbits4;
    unsigned long long int DHneg1orDHneg2orDHneg3orDHneg4;
    unsigned long long int DHneg1orDHneg2;
    unsigned long long int DHpos5orDHpos4;
    unsigned long long int DHneg3orDHneg4;
    unsigned long long int tempbit16, tempbit8, tempbit4, tempbit2, tempbit1;
    unsigned long long int sumbit1, sumbit2, sumbit4, sumbit8, sumbit16;
    unsigned long long int bit16, bit8, bit4, bit2, bit1;

    unsigned long long int sum;
    unsigned long long int highone = one << wordsizem1;
    unsigned long long int nexthighone = one << wordsizem2;
    unsigned long long int initval;
    char *iterate;
    int counter = 0, w = 0;
    int score;

    n = strlen(s1);
    m = strlen(s2);

	n--;
	m--;
	if(n==0||m==0)return 0;
    //*************************encode match strings A C G T N for string1
    //loop through string1 and store bits in matchA, matchC, etc.
    //position zero corresponds to column one in the score matrix, i.e., first character
    //so we start with i = 0 and bitmask = 1

    bitmask = one;
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
    DVDHbits1 = (unsigned long long int *) calloc(words, sizeof(unsigned long long int));
    DVDHbits2 = (unsigned long long int *) calloc(words, sizeof(unsigned long long int));
    DVDHbits4 = (unsigned long long int *) calloc(words, sizeof(unsigned long long int));
    DVDHbits8 = (unsigned long long int *) calloc(words, sizeof(unsigned long long int));
    DVDHbits16 = (unsigned long long int *) calloc(words, sizeof(unsigned long long int));
    DVDHbits32 = (unsigned long long int *) calloc(words, sizeof(unsigned long long int));



    //intialize top row (penalty for initial gap)
    DHneg5 = all_ones;
    DHneg4 = DHneg3 = DHneg2 = DHneg1 = DHzero = DHpos1 = DHpos2 = DHpos3 = DHpos4 = DHpos5 = DHpos6 = DHpos7 = 0;
    DHpos7orDHpos6 = (DHpos7 | DHpos6);
    DHneg1orDHneg2 = (DHneg1 | DHneg2);
    DHneg3orDHneg4 = (DHneg3 | DHneg4);
    DHpos5orDHpos4 = (DHpos5 | DHpos4);
    DHpos3orDHpos2 = (DHpos3 | DHpos2);
    DHpos1orDHzero = (DHpos1 | DHzero);
    DHpos3orDHpos2orDHpos1orDHzero = (DHpos3orDHpos2 | DHpos1orDHzero);
    DHneg1orDHneg2orDHneg3orDHneg4 = (DHneg1orDHneg2 | DHneg3orDHneg4);
    DHpos7orDHpos6orDHpos5orDHpos4 = (DHpos7orDHpos6 | DHpos5orDHpos4);
    DHpos3orDHpos2orDHpos1orDHzeroorDHneg1orDHneg2orDHneg3orDHneg4 = (DHpos3orDHpos2orDHpos1orDHzero |
                                                                      DHneg1orDHneg2orDHneg3orDHneg4);
    DHpos7orDHpos6orDHpos5orDHpos4orDHpos3orDHpos2orDHpos1orDHzeroorDHneg1orDHneg2orDHneg3orDHneg4 = (
            DHpos7orDHpos6orDHpos5orDHpos4 | DHpos3orDHpos2orDHpos1orDHzeroorDHneg1orDHneg2orDHneg3orDHneg4);
    for (i = 0; i < words; ++i) {
        DVDHbits1[i] = DHneg2 | DHpos4 | DHpos6 | DHzero | DHpos2 | DHneg4;
        DVDHbits2[i] = DHneg3orDHneg4 | DHpos1orDHzero | DHpos5orDHpos4;
        DVDHbits4[i] = DHneg1orDHneg2orDHneg3orDHneg4 | DHpos7orDHpos6orDHpos5orDHpos4;
        DVDHbits8[i] = DHpos3orDHpos2orDHpos1orDHzeroorDHneg1orDHneg2orDHneg3orDHneg4;
        DVDHbits16[i] = DHpos7orDHpos6orDHpos5orDHpos4orDHpos3orDHpos2orDHpos1orDHzeroorDHneg1orDHneg2orDHneg3orDHneg4;
        DVDHbits32[i] = 0;
    }

    //recursion
    for (i = 0, iterate = s2; i < m; ++i, ++iterate) {
        bit1 = bit2 = bit4 = bit8 = bit16 = 0;
        //initialize OverFlow
        OverFlow0 = OverFlow1 = OverFlow2 = OverFlow3 = OverFlow4 = OverFlow5 = OverFlow6 = OverFlow7 = OverFlow8 = OverFlow9 = OverFlow10 = OverFlow11 = 0;
        INITpos7sprevbit = INITpos6sprevbit = INITpos5sprevbit = INITpos4sprevbit = INITpos3sprevbit = INITpos2sprevbit = INITpos1sprevbit = INITzerosprevbit = INITneg1sprevbit = INITneg2sprevbit = INITneg3sprevbit = INITneg4sprevbit = 0;

        matchv = matchvec[*iterate];
        for (j = 0; j < words; ++j) {
            DHbits16 = DVDHbits16[j];
            DHbits8 = DVDHbits8[j];
            DHbits4 = DVDHbits4[j];
            DHbits2 = DVDHbits2[j];
            DHbits1 = DVDHbits1[j];

            Matches = *matchv;
            matchv++;
            //Complement Matches
            NotMatches = ~Matches;
            notDHbits8 = ~DHbits8;
            notDHbits16 = (~DHbits16);
            DHbits16DHbits8 = DHbits16 & DHbits8;
            notDHbits16notDHbits8 = notDHbits16 & notDHbits8;
            notDHbits4 = ~DHbits4;
            notDHbits2 = ~DHbits2;
            DHbits16DHbits8DHbits4 = DHbits16DHbits8 & DHbits4;
            notDHbits16notDHbits8notDHbits4 = notDHbits16notDHbits8 & notDHbits4;
            DHbits16DHbits8notDHbits4 = DHbits16DHbits8 & notDHbits4;
            notDHbits16notDHbits8notDHbits4notDHbits2 = notDHbits16notDHbits8notDHbits4 & notDHbits2;
            DHbits16DHbits8DHbits4notDHbits2 = DHbits16DHbits8DHbits4 & notDHbits2;
            DHbits16DHbits8notDHbits4DHbits2 = DHbits16DHbits8notDHbits4 & DHbits2;
            DHbits16DHbits8notDHbits4notDHbits2 = DHbits16DHbits8notDHbits4 & notDHbits2;
            DHbits16DHbits8DHbits4DHbits2 = DHbits16DHbits8DHbits4 & DHbits2;
            notDHbits1 = ~DHbits1;
            DHneg5 = notDHbits16notDHbits8notDHbits4notDHbits2 & notDHbits1 & sixtythreebitmask;
            DHneg4 = DHbits16DHbits8DHbits4DHbits2 & DHbits1;
            DHneg3 = DHbits16DHbits8DHbits4DHbits2 & notDHbits1;
            DHneg2 = DHbits16DHbits8DHbits4notDHbits2 & DHbits1;
            DHneg1 = DHbits16DHbits8DHbits4notDHbits2 & notDHbits1;
            DHzero = DHbits16DHbits8notDHbits4DHbits2 & DHbits1;
            DHpos1 = DHbits16DHbits8notDHbits4DHbits2 & notDHbits1;
            DHpos2 = DHbits16DHbits8notDHbits4notDHbits2 & DHbits1;
            //Finding the vertical values
            //Find 7s
            INITpos7s = DHneg5 & Matches;
            sum = (INITpos7s + DHneg5) + OverFlow0;
            DVpos7shift = ((sum ^ DHneg5) ^ INITpos7s) & sixtythreebitmask;
            OverFlow0 = sum >> wordsizem1;

            //set RemainingDHneg5
            RemainDHneg5 = DHneg5 ^ (INITpos7s);
            //combine 7s and Matches
            DVpos7shiftorMatch = DVpos7shift | Matches;

            //Find 6s
            INITpos6s = (DHneg4 & DVpos7shiftorMatch);
            initval = ((INITpos6s << 1) | INITpos6sprevbit);
            INITpos6sprevbit = (initval >> wordsizem1);
            initval &= sixtythreebitmask;
            sum = initval + RemainDHneg5 + OverFlow1;
            DVpos6shift = (sum ^ RemainDHneg5) & NotMatches;
            OverFlow1 = sum >> wordsizem1;
            //Find 5s
            INITpos5s = (DHneg3 & DVpos7shiftorMatch) | (DHneg4 & DVpos6shift);
            initval = ((INITpos5s << 1) | INITpos5sprevbit);
            INITpos5sprevbit = (initval >> wordsizem1);
            initval &= sixtythreebitmask;
            sum = initval + RemainDHneg5 + OverFlow2;
            DVpos5shift = (sum ^ RemainDHneg5) & NotMatches;
            OverFlow2 = sum >> wordsizem1;
            //Find 4s
            INITpos4s = (DHneg2 & DVpos7shiftorMatch) | (DHneg3 & DVpos6shift) | (DHneg4 & DVpos5shift);
            initval = ((INITpos4s << 1) | INITpos4sprevbit);
            INITpos4sprevbit = (initval >> wordsizem1);
            initval &= sixtythreebitmask;
            sum = initval + RemainDHneg5 + OverFlow3;
            DVpos4shift = (sum ^ RemainDHneg5) & NotMatches;
            OverFlow3 = sum >> wordsizem1;
            //Find 3s
            INITpos3s = (DHneg1 & DVpos7shiftorMatch) | (DHneg2 & DVpos6shift) | (DHneg3 & DVpos5shift) |
                        (DHneg4 & DVpos4shift);
            initval = ((INITpos3s << 1) | INITpos3sprevbit);
            INITpos3sprevbit = (initval >> wordsizem1);
            initval &= sixtythreebitmask;
            sum = initval + RemainDHneg5 + OverFlow4;
            DVpos3shift = (sum ^ RemainDHneg5) & NotMatches;
            OverFlow4 = sum >> wordsizem1;
            //set DVnot7to3shiftorMatch
            DVnot7to3shiftorMatch = ~(DVpos7shiftorMatch | DVpos6shift | DVpos5shift | DVpos4shift | DVpos3shift);
            DVpos5shiftorDVpos6shift = (DVpos5shift | DVpos6shift);
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
            sumbit1 = (DHbits1 ^ DVbits1);
            sumbit2 = ((DHbits2 ^ DVbits2) ^ carry0);
            sumbit4 = ((DHbits4 ^ DVbits4) ^ carry1);
            sumbit8 = ((DHbits8 ^ DVbits8) ^ carry2);
            sumbit16 = ((DHbits16 ^ DVbits16) ^ carry3);
            DVDHbithighcomp = ~sumbit16;
            sumbit1 &= DVDHbithighcomp;
            sumbit2 &= DVDHbithighcomp;
            sumbit4 &= DVDHbithighcomp;
            sumbit8 &= DVDHbithighcomp;
            sumbit16 &= DVDHbithighcomp;
            tempbit16 = (sumbit16 & nexthighone) >> wordsizem2;
            tempbit8 = (sumbit8 & nexthighone) >> wordsizem2;
            tempbit4 = (sumbit4 & nexthighone) >> wordsizem2;
            tempbit2 = (sumbit2 & nexthighone) >> wordsizem2;
            tempbit1 = (sumbit1 & nexthighone) >> wordsizem2;
            sumbit1 <<= 1;
            sumbit2 <<= 1;
            sumbit4 <<= 1;
            sumbit8 <<= 1;
            sumbit16 <<= 1;
            sumbit16 |= bit16;
            sumbit8 |= bit8;
            sumbit4 |= bit4;
            sumbit2 |= bit2;
            sumbit1 |= bit1;
            bit16 = tempbit16;
            bit8 = tempbit8;
            bit4 = tempbit4;
            bit2 = tempbit2;
            bit1 = tempbit1;
            DVbits1 = sumbit1;
            DVbits2 = sumbit2;
            DVbits4 = sumbit4;
            DVbits8 = sumbit8;
            DVbits16 = sumbit16;

            //Finding the horizontal values
            //Remove matches from DH values except 7
            //combine 7s and Matches
            DHpos7orMatch = DHpos7 | Matches;
            //group -5topos2
            DHneg5topos2 = (DHneg5 | DHneg4 | DHneg3 | DHneg2 | DHneg1 | DHzero | DHpos1 | DHpos2) & NotMatches;
            compDHneg5topos2 = all_ones ^ DHneg5topos2;
            //Representing the range -5 to 2 as 2;
            DHbits1 |= DHneg5topos2;
            DHbits2 &= compDHneg5topos2;
            DHbits4 &= compDHneg5topos2;
            DHbits8 |= DHneg5topos2;
            DHbits16 |= DHneg5topos2;
            //Representing Matches as the packed value of 7 which is -12
            DHbits1 &= NotMatches;
            DHbits2 &= NotMatches;
            DHbits4 |= Matches;
            DHbits8 &= NotMatches;
            DHbits16 |= Matches;
            carry0 = (DHbits1 & sumbit1);
            carry1 = ((DHbits2 & sumbit2) | ((DHbits2 ^ sumbit2) & carry0));
            carry2 = ((DHbits4 & sumbit4) | ((DHbits4 ^ sumbit4) & carry1));
            carry3 = ((DHbits8 & sumbit8) | ((DHbits8 ^ sumbit8) & carry2));
            carry4 = ((DHbits16 & sumbit16) | ((DHbits16 ^ sumbit16) & carry3));
            sumbit1 = (DHbits1 ^ sumbit1);
            sumbit2 = ((DHbits2 ^ sumbit2) ^ carry0);
            sumbit4 = ((DHbits4 ^ sumbit4) ^ carry1);
            sumbit8 = ((DHbits8 ^ sumbit8) ^ carry2);
            sumbit16 = ((DHbits16 ^ sumbit16) ^ carry3);
            //Convert everything that is positive to 0
            sumbit1 &= sumbit16;
            sumbit2 &= sumbit16;
            sumbit4 &= sumbit16;
            sumbit8 &= sumbit16;
            sumbit16 &= sumbit16;
            DVDHbits1[j] = sumbit1;
            DVDHbits2[j] = sumbit2;
            DVDHbits4[j] = sumbit4;
            DVDHbits8[j] = sumbit8;
            DVDHbits16[j] = sumbit16;


        }
    }
    //find scores in last row

    score = -5 * m;

    bitmask = one;
    for (j = 0; j < words; ++j) {
        for (i = j * wordsizem1; i < (j + 1) * wordsizem1 && i < n; ++i) {
            score -= (DVDHbits1[j] & bitmask) * 1 + (DVDHbits2[j] & bitmask) * 2 + (DVDHbits4[j] & bitmask) * 4 +
                     (DVDHbits8[j] & bitmask) * 8 + -(DVDHbits16[j] & bitmask) * 16 + 5;
            DVDHbits1[j] >>= 1;
            DVDHbits2[j] >>= 1;
            DVDHbits4[j] >>= 1;
            DVDHbits8[j] >>= 1;
            DVDHbits16[j] >>= 1;


        }

    }
    free(DVDHbits1);
    free(DVDHbits2);
    free(DVDHbits4);
    free(DVDHbits8);
    free(DVDHbits16);
    free(DVDHbits32);

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



