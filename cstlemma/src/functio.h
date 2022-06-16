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
#ifndef FUNCTION_H
#define FUNCTION_H

#include "defines.h"
#if defined PROGLEMMATISE

#include <stdio.h>
#include <string>

class OutputClass;
class basefrm;
class Word;
class taggedWord;

typedef std::string (basefrm::*fptrbf)() const;
typedef void (Word::*fptr)() const;
typedef void (Word::*fptrs)(std::string &str) const;
typedef void (taggedWord::*fptrt)() const;
typedef void (taggedWord::*fptrts)(std::string &str) const;
typedef int (Word::*fptcount)() const;
typedef int (basefrm::*fptcountbf)() const;

class formattingFunction
{
public:
    virtual void doIt(const OutputClass *outputObj) const = 0;
    virtual void toString(const OutputClass *outputObj, std::string &str);
    virtual int count(const OutputClass *outputObj) const
    {
        REFER(outputObj)
        return -1;
    }
    virtual ~formattingFunction()
    {}
    virtual bool skip(const basefrm *bf) const = 0;
};

class functionNoArgB : public formattingFunction
{
private:
    fptrbf m_fn;

public:
    functionNoArgB(fptrbf fn) : m_fn(fn) {}
    void doIt(const OutputClass *outputObj) const
    {
        (((const basefrm *)outputObj)->printToFile)((((const basefrm *)outputObj)->*m_fn)().c_str());
    }
    void toString(const OutputClass *outputObj, std::string &str) const
    {
        str.append((((const basefrm *)outputObj)->*m_fn)());
    }
    virtual bool skip(const basefrm *bf) const
    {
        REFER(bf)
        return false;
    }
};

class functionString : public formattingFunction
{
private:
    char *arg;

public:
    static FILE *fp;

    functionString(char *arg);
    ~functionString()
    {
        delete[] arg;
    }
    void doIt(const OutputClass *outputObj) const
    {
        REFER(outputObj)
        for (char *s = arg; *s; ++s)
            fputc(*s, fp);
    }
    void toString(const OutputClass *outputObj, std::string &str) const
    {
        str.append(arg);
    }
    virtual bool skip(const basefrm *bf) const
    {
        REFER(bf)
        return false;
    }
};

class functionNoArg : public formattingFunction
{
private:
    fptr m_fn;
    fptrs m_fns;
    fptcount m_fncount;

public:
    functionNoArg(fptr fn, fptrs fns, fptcount fncount) : m_fn(fn), m_fns(fns), m_fncount(fncount) {}
    void doIt(const OutputClass *outputObj) const
    {
        const Word *tmp = (const Word *)outputObj;
        (tmp->*m_fn)();
    }
    void toString(const OutputClass *outputObj, std::string &str) const
    {
        const Word *tmp = (const Word *)outputObj;
        (tmp->*m_fns)(str);
    }
    virtual bool skip(const basefrm *bf) const
    {
        REFER(bf)
        return false;
    }
    int count(const OutputClass *u) const
    {
        if (m_fncount)
        {
            const Word *tmp = (const Word *)u;
            return (tmp->*m_fncount)();
        }
        else
            return formattingFunction::count(u);
    }
};

class functionNoArgW : public formattingFunction
{
private:
    fptrbf m_fn;
    fptcountbf m_fncount;

public:
    functionNoArgW(fptrbf fn, fptcountbf fncount) : m_fn(fn), m_fncount(fncount) {}
    void doIt(const OutputClass *outputObj) const
    {
        const basefrm *tmp = (const basefrm *)outputObj;
        (tmp->*m_fn)();
    }
    void toString(const OutputClass *outputObj, std::string &str) const
    {
        str.append((((const basefrm *)outputObj)->*m_fn)());
    }
    virtual bool skip(const basefrm *bf) const
    {
        REFER(bf)
        return false;
    }
    int count(const OutputClass *u) const
    {
        if (m_fncount)
        {
            const basefrm *tmp = (const basefrm *)u;
            return (tmp->*m_fncount)();
        }
        else
            return formattingFunction::count(u);
    }
};

class functionNoArgT : public formattingFunction
{
private:
    fptrt m_fn;
    fptrts m_fns;

public:
    functionNoArgT(fptrt fn, fptrts fns) : m_fn(fn), m_fns(fns) {}
    void doIt(const OutputClass *outputObj) const
    {
        ((const taggedWord *)outputObj->*m_fn)();
    }
    void toString(const OutputClass *outputObj, std::string &str) const
    {
        (((const taggedWord *)outputObj)->*m_fns)(str);
    }
    virtual bool skip(const basefrm *bf) const
    {
        REFER(bf)
        return false;
    }
};

#endif
#endif
