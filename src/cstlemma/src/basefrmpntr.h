/*
CSTLEMMA - trainable lemmatiser

Copyright (C) 2002, 2014  Center for Sprogteknologi, University of Copenhagen

This file is part of CSTLEMMA.

CSTLEMMA is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

CSTLEMMA is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with CSTLEMMA; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef BASEFORMPOINTER_H
#define BASEFORMPOINTER_H

#include "defines.h"
#if defined PROGLEMMATISE

#include <stdio.h>
#include <string>

extern void (*print)(
    FILE *fpo, const char *s);

class basefrm;
class functionTree;
class Word;

typedef void (basefrm::*bfn)(void) const;
typedef std::string (basefrm::*bfns)(void) const;

class baseformpointer
{
private:
        basefrm *bf;
        baseformpointer *next;
#if PFRQ
        unsigned int pfrq;
#endif
        bool owning;
        bool hidden;
        bool hasDuplicateLemma(baseformpointer *startOfList, baseformpointer *current);

public:
        static int UseLemmaFreqForDisambiguation;
        int count()
        {
                return (hidden ? 0 : 1) + (next ? next->count() : 0);
        }

        void printfbf(FILE *fp, functionTree *fns, const char *sep);
        void printFn(FILE *fp, bfn Fn, const char *sep);
        void printFn(FILE *fp, bfns Fn, const char *sep);
        void writefbf(std::string &str, functionTree *fns, const char *sep);
        void writeFn(std::string &str, bfn Fn, const char *sep);
#if PRINTRULE
        void printfrule(FILE *fp, functionTree *fns, const char *sep);
#endif
#if PRINTRULE
        void P();
        void R();
        void PString(std::string &str);
        void RString(std::string &str);
#endif
#if PFRQ || FREQ24
        baseformpointer(const char *s, const char *t, size_t len, unsigned int frequency);
#else
        baseformpointer(const char *s, const char *t, size_t len);
#endif
        ~baseformpointer();
        void reassign(basefrm *bf);
#if PFRQ || FREQ24
        int addBaseForm(const char *s, const char *t, size_t len, unsigned int frequency);
#else
        int addBaseForm(const char *s, const char *t, size_t len);
#endif
        void assignTo(basefrm **&pbf)
        {
                *pbf = bf;
                ++pbf;
                if (next)
                        next->assignTo(pbf);
        }
        void addFullForm(Word *word);
        void DisambiguateByLemmaFrequency();
        void DisambiguateByTagFriends(const char *tag);
        void testPrint();
        void decFreq(Word *w);
};

#endif
#endif
