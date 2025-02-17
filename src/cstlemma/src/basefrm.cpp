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

#include "basefrm.h"
#if defined PROGLEMMATISE
#include "basefrmpntr.h"
#include "functio.h"
#include "functiontree.h"
#include "word.h"
#include "tags.h"
#include <assert.h>
#include <stdlib.h>
#include <string>

using namespace std;

int basefrm::index = 0;
functionTree *basefrm::bfuncs = 0; // used if -W option set
functionTree *basefrm::Bfuncs = 0; // used if -W option set
functionTree *basefrm::wfuncs = 0; // used if -W option set
const char *basefrm::sep;
bool basefrm::hasW = false;
tagpairs *TagFriends = 0;

FILE *basefrm::m_fp = 0;
void basefrm::setFile(FILE *a_fp)

{
    basefrm::m_fp = a_fp;
    functionString::fp = a_fp;
    index = 0;
}

formattingFunction *basefrm::getBasefrmFunction(int character, bool &DummySortInput, int &testType)
{
    REFER(DummySortInput)
    switch (character)
    {
    case 'f':
        return new functionNoArgB(&basefrm::F, &basefrm::FString);
#if FREQ24
    case 'n':
        return new functionNoArgB(&basefrm::N);
#endif
        /*case 'p':
        return new functionNoArgB(&basefrm::P);*/
    case 'w':
        return new functionNoArgB(&basefrm::W, &basefrm::WString);
    case 'W':
        hasW = true;
        testType |= NUMBERTEST;
        return new functionNoArgW(&basefrm::L, &basefrm::LString, &basefrm::countFullForms);
        //            return new functionNoArgB(&basefrm::L);
    case 't':
        return new functionNoArgB(&basefrm::T, &basefrm::TString);
    }
    return 0;
}

formattingFunction *basefrm::getBasefrmFunctionNoW(int character, bool &DummySortInput, int &testType)
{
    REFER(DummySortInput)
    REFER(testType)
    switch (character)
    {
    case 'f':
        basefrm::hasW = true; // Lemma frequency is sum of
                              // full form frequencies, so full form
                              // pointers must be added to each base form.
        return new functionNoArgB(&basefrm::F, &basefrm::FString);
#if FREQ24
    case 'n':
        return new functionNoArgB(&basefrm::N);
#endif
    case 'w':
        return new functionNoArgB(&basefrm::W, &basefrm::WString);
    case 't':
        return new functionNoArgB(&basefrm::T, &basefrm::TString);
#if PRINTRULE
    case 'p':
        return new functionNoArgB(&basefrm::P, &basefrm::PString);
    case 'r':
        return new functionNoArgB(&basefrm::R, &basefrm::RString);
#endif
    }
    return 0;
}

functionTree *basefrm::Format(const char *format)
{
    functionTree *ret = new functionTree();
    bool DummySortInput;
    int testType = 0;
    OutputClass::Format(format, getBasefrmFunctionNoW, *ret, format, DummySortInput, testType);
    return ret;
}

void basefrm::getAbsorbedBy(basefrm *other)
{
    m_owner.reassign(other);
}

basefrm::~basefrm()
{
    delete[] m_s; // Also deletes m_p ! (They are allocated as one string.)
    delete[] m_t;
    delete[] fullForm;
#ifdef COUNTOBJECTS
    --COUNT;
#endif
}

void basefrm::addFullForm(Word *word)
{
    assert(basefrm::hasW);
    if (nfullForm > 0)
    {
        Word **nwlist = new Word *[++nfullForm];
        unsigned int i;
        for (i = 0; i < (unsigned int)(nfullForm - 1); ++i)
            nwlist[i] = fullForm[i];
        nwlist[i] = word;
        delete[] fullForm;
        fullForm = nwlist;
    }
    else
    {
        nfullForm = 1;
        fullForm = new Word *[1];
        fullForm[0] = word;
    }
}

void basefrm::L() const
{
    assert(basefrm::hasW);
    for (unsigned int i = 0; i < nfullForm; ++i)
    {
        wfuncs->printIt(fullForm[i]);
        if (i < (unsigned int)(nfullForm - 1))
            print(m_fp, sep);
    }
}

void basefrm::LString(string &str) const
{
    assert(basefrm::hasW);
    for (unsigned int i = 0; i < nfullForm; ++i)
    {
        wfuncs->writeIt(fullForm[i], str);
        if (i < (unsigned int)(nfullForm - 1))
            str.append(sep);
    }
}

bool basefrm::setFormat(const char *Wformat, const char *bformat, const char *Bformat, bool InputHasTags)
{
    bool SortInput = false;
    if (bformat)
    {
        bool DummySortInput = false;
        if (bfuncs)
            delete bfuncs;
        bfuncs = new functionTree();
        int testType = 0;
        OutputClass::Format(bformat, basefrm::getBasefrmFunction, *bfuncs, bformat, DummySortInput, testType);
    }

    if (Bformat)
    {
        bool DummySortInput = false;
        if (Bfuncs)
            delete Bfuncs;
        Bfuncs = new functionTree();
        int testType = 0;
        OutputClass::Format(Bformat, basefrm::getBasefrmFunction, *Bfuncs, Bformat, DummySortInput, testType);
    }

    if (hasW)
    {
        if (!Wformat)
        {
            LOG1LINE("You need to specify -W<full form format> if you use the $W variable in -B<base form format> or -b<base form format>");
            exit(0);
        }
        if (wfuncs)
            delete wfuncs;
        wfuncs = new functionTree();
        int testType = 0;
        if (InputHasTags)
            OutputClass::Format(Wformat, taggedWord::getTaggedWordFunctionNoBb, *wfuncs, Wformat, SortInput, testType);
        else
            OutputClass::Format(Wformat, Word::getUnTaggedWordFunctionNoBb, *wfuncs, Wformat, SortInput, testType);
    }
    return SortInput;
}

void basefrm::printb() const
{
    //    testPrint();
    bfuncs->printIt(this);
}

void basefrm::printB() const
{
    Bfuncs->printIt(this);
}

int basefrm::countFullForms() const
{
    assert(basefrm::hasW);
    return nfullForm;
}

void basefrm::addFullForms(basefrm *other)
{
    assert(basefrm::hasW);
    int nnnfullForm = nfullForm + other->nfullForm;
    int nnnfullFormOrg = nnnfullForm;
    if (nnnfullForm)
    {
        Word **nwlist = new Word *[nnnfullForm];
        unsigned int i, j, k;
        for (i = 0, j = 0, k = 0; i < nfullForm && j < other->nfullForm;)
        {
            int cmp = fullForm[i]->cmpword(other->fullForm[j]);
            if (cmp < 0)
            {
                nwlist[k++] = fullForm[i++];
            }
            else
            {
                if (cmp == 0) // Without this block, duplicate
                // fullforms sometimes enter the list.
                // This happens especially when the dictionary is used and
                // it contains homographs:
                // for    for    N_INDEF_PLU
                // for    for    N_INDEF_SING
                // for    for,3    PR�P
                // for    for,4    ADV
                // for    for,5    SKONJ
                // With input without POS tags, the first two homographs
                // merge together, but four different readings remain.
                // This resulted in output like
                //          16 for (4 for|4 for|4 for|4 for)
                {
                    if (fullForm[i] == other->fullForm[j]) // This check is
                    // necessary because the full forms may have different
                    // POS tags.
                    {
                        ++i;
                        --nnnfullForm;
                    }
                }
                nwlist[k++] = other->fullForm[j++];
            }
        }
        for (; i < nfullForm;)
            nwlist[k++] = fullForm[i++];
        for (; j < other->nfullForm;)
            nwlist[k++] = other->fullForm[j++];
        delete[] fullForm;
        if (nnnfullForm != nnnfullFormOrg)
        {
            Word **nnwlist = new Word *[nnnfullForm];
            for (int ind = 0; ind < nnnfullForm; ++ind)
                nnwlist[ind] = nwlist[ind];
            delete[] nwlist;
            fullForm = nnwlist;
        }
        else
            fullForm = nwlist;
        nfullForm = nnnfullForm;
    }
}

int basefrm::lemmaFreq() const
{
    assert(basefrm::hasW);
    int ret = 0;
    for (unsigned int i = 0; i < nfullForm; ++i)
    {
        ret += fullForm[i]->itsCnt();
    }
    return ret;
}

void basefrm::removeFullForm(Word *w)
{
    assert(basefrm::hasW);
    unsigned int i;
    for (i = 0; i < nfullForm; ++i)
    {
        if (fullForm[i] == w)
        {
            nfullForm = nfullForm - 1;
            do
            {
                fullForm[i] = fullForm[i + 1];
            } while (++i < nfullForm);
            return;
        }
    }
}

int basefrm::Closeness(const char *tag)
{
    if (!strcmp(tag, m_t))
        return 0;

    if (TagFriends)
        return TagFriends->Closeness(tag, m_t);
    else
        return -1;
}

void basefrm::testPrint() const
{
#if PRINTRULE
    printf("%s/%s/%s\n", m_s, m_t, m_p);

#else
    printf("%s/%s\n", m_s, m_t);
#endif
}

static void printOther(FILE *fpo, const char *s)
{
    if (s)
        fprintf(fpo, "%s", s);
}

void basefrm::printToFile(const char *s) const
{
    if (s)
        fprintf(m_fp, "%s", s);
}

void (*print)(FILE *fpo, const char *s) = printOther;

void basefrm::T() const
{
    print(basefrm::m_fp, m_t);
}
void basefrm::TString(string &str) const
{
    str.append(m_t);
}

void basefrm::W() const
{
    print(basefrm::m_fp,m_s);
}

void basefrm::WString(string &str) const
{
    str.append(m_s);
}

#if PRINTRULE
void basefrm::P() const
{
    print(basefrm::m_fp, m_p);
}
void basefrm::R() const
{
    print(basefrm::m_fp, m_r());
}
void basefrm::PString(string &str) const
{
    str.append(m_p);
}
void basefrm::RString(string &str) const
{
    str.append(m_r());
}
#endif

#endif
