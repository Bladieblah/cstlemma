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
#ifndef FLATTEXT_H
#define FLATTEXT_H

#include "text.h"
#include <string>
#if defined PROGLEMMATISE

class flattext : public text
{
public:
    flattext(
        FILE *fpi, bool InputHasTags, char *Iformat, int keepPunctuation, bool nice, unsigned long int size, bool treatSlashAsAlternativeSeparator);
    flattext(
        std::string str, int keepPunctuation, bool nice, unsigned long int size, bool treatSlashAsAlternativeSeparator);
    ~flattext() {}
    virtual const char *convert(const char *s, char *buf, const char *lastBufByte);
    virtual void DoYourWork(
        FILE *fpi, optionStruct &Option){};

    virtual void printUnsorted(
        FILE *fpo);
};

#endif
#endif
