/*
CSTLEMMA - trainable lemmatiser

Copyright (C) 2002, 2014, 2009  Center for Sprogteknologi, University of Copenhagen

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

#include "flattext.h"
#if defined PROGLEMMATISE
#include "word.h"
#include "field.h"
#include "caseconv.h"
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#include <string>

using namespace std;

static int sanityCheck(int slashFound, const char *buf)
{
    if (!slashFound)
        return 0;
    if (*buf == '/')
        return 0; // list of alternatives does not start with "/"
    const char *p = buf;
    while (is_Alpha(*p)                     // list of alternatives does only have alphabetic characters
           || (*p == '/' && *(p - 1) != '/' // list of alternatives does not contain "//"
               ))
        ++p;
    if (*p)
        return 0;
    if (*(p - 1) == '/')
        return 0; // list of alternatives does not end with "/"
    return slashFound;
}

static bool spaces(int kar, FILE *fp, unsigned long &newlines, int &eof, int &prevkar)
{
    if (isSpace(kar))
    {
        if (kar == '\n')
        {
            ++newlines;
        }
        // We need to look for new line after the blank. If we wait, the first
        // word of the next line will become the last word of the current line.
        // If there is no new line character, the first character of the next
        // word will be read. Put this in a safe place: prevkar.
        else
        {
            do
            {
                kar = fgetc(fp);
                if (kar == EOF)

                {
                    eof = true;
                    break;
                }
            } while (isSpace(kar) && kar != '\n');
            if (kar == '\n')
            {
                ++newlines;
            }
            else if (!eof)
                prevkar = kar;
        }
        return true;
    }
    return false;
}

static bool spaces(int kar, string str, string::size_type &i, unsigned long &newlines, int &eof, int &prevkar)
{
    if (isSpace(kar))
    {
        if (kar == '\n')
        {
            ++newlines;
        }
        else
        {
            do
            {
                i++;
                kar = str[i];
                if (kar == EOF)
                {
                    eof = true;
                    break;
                }
            } while (isSpace(kar) && kar != '\n');
            if (kar == '\n')
            {
                ++newlines;
            }
            else if (!eof)
                prevkar = kar;
        }
        return true;
    }
    return false;
}

static char *getword(FILE *fp, const char *&tag, bool InputHasTags, int keepPunctuation, int &slashFound, unsigned long &newlines)
// newlines is incremented when the current word is followed by a new line \n
{
    static int punct = 0;
    static char buf[1000];
    static char buf2[256]; // tag
    static int eof = false;
    static int prevkar = 0;
    newlines = 0;
    slashFound = 0;
    if (punct)
    {
        buf[0] = (char)punct;
        buf[1] = '\0';
        punct = 0;
        return buf;
    }
    int kar;
    char *p;
    p = buf;
    tag = 0;
    if (eof)
    {
        eof = false;
        return 0;
    }
    for (;;)
    {
        if (prevkar)
        {
            kar = prevkar;
            prevkar = 0;
        }
        else
        {
            kar = fgetc(fp);
            if (kar == EOF)

            {
                eof = true;
                break;
            }
        }
        if (InputHasTags)
        {
            if (kar == '/')
            {                    // tag follows (or maybe not, see call to sanityCheck at end).
                char *slash = p; // slash points at / in buf (the word)
                *p = '\0';       // the / is replaced by zero
                p = buf2;        // p points at the start of the tag buffer
                tag = buf2;
                for (;;)
                {
                    kar = fgetc(fp);
                    if (kar == EOF)

                    {
                        eof = true;
                        break;
                    }
                    if (spaces(kar, fp, newlines, eof, prevkar))
                    {
                        break;
                    }
                    if (kar == '/') // oops, word contains slash
                    {
                        ++slashFound;            // Token may need special treatment as "/"-separated alternatives.
                        *slash = '/';            // put the slash back into the word (somewhere in buf)
                        *p = '\0';               // prepare buf2 for being copied back to buf
                        strcpy(slash + 1, buf2); // do the copying
                        slash += p - buf2 + 1;   // let slash point at end of buf (the nul byte)
                        p = buf2;                // let p start again at start of buf2
                    }
                    else
                    {
                        if (p - buf2 == sizeof(buf2) - 1)
                        {
                            fprintf(stderr, "BUFFER OVERFLOW A [%s]\n", buf2);

                            break;
                        }
                        *p++ = (char)kar;
                    }
                }
                break;
            }
        }
        else if (keepPunctuation != 1 && p > buf && ispunct(kar) && kar != '-' /*g�r-det-selv*/
                 && kar != '\''                                                /*bli'r*/
        )
        {
            if (keepPunctuation != 0)
                punct = kar;
            break;
        }
        if (spaces(kar, fp, newlines, eof, prevkar))
            break;
        if (p - buf == sizeof(buf) - 1)
        {
            fprintf(stderr, "BUFFER OVERFLOW B [%s]\n", buf);

            break; // overflow?
        }
        if (kar == '/')
            ++slashFound;
        *p++ = (char)kar;
    }
    *p = '\0';
    slashFound = sanityCheck(slashFound, buf);
    return buf;
}

static char *getword(string str, string::size_type &pos, const char *&tag, int keepPunctuation, int &slashFound, unsigned long &newlines)
// newlines is incremented when the current word is followed by a new line \n
{
    static int punct = 0;
    static char buf[1000];
    static int eof = false;
    static int prevkar = 0;
    newlines = 0;
    slashFound = 0;
    if (punct)
    {
        buf[0] = (char)punct;
        buf[1] = '\0';
        punct = 0;
        return buf;
    }
    int kar;
    char *p;
    p = buf;
    tag = 0;
    if (eof)
    {
        eof = false;
        return 0;
    }
    
    for (; pos < str.size(); pos++)
    {
        if (prevkar)
        {
            kar = prevkar;
            prevkar = 0;
        }
        else
        {
            kar = str[pos];
            if (kar == EOF)
            {
                eof = true;
                break;
            }
        }
        if (keepPunctuation != 1 && p > buf && ispunct(kar) && kar != '-' && kar != '\'')
        {
            if (keepPunctuation != 0)
                punct = kar;
            break;
        }
        if (spaces(kar, str, pos, newlines, eof, prevkar))
            break;
        if (p - buf == sizeof(buf) - 1)
        {
            fprintf(stderr, "BUFFER OVERFLOW B [%s]\n", buf);

            break;
        }
        if (kar == '/')
            ++slashFound;
        *p++ = (char)kar;
    }
    *p = '\0';
    slashFound = sanityCheck(slashFound, buf);
    return buf;
}

static char * getwordI(FILE *fpin, const char *&tag, field *format, field *wordfield, field *tagfield, unsigned long &newlines, char *Iformat)
{
    format->reset();
    assert(wordfield);
    int kar = EOF;
    char kars[2];
    kars[1] = '\0';
    static int lastkar = '\0';
    field *nextfield = format;
    newlines = 0;
    if (lastkar)
    {
        if (lastkar == EOF)
        {
            lastkar = 0;
            tag = 0;
            format->reset();
            return 0;
        }
        if (lastkar == '\n')
            ++newlines;
        kars[0] = (char)lastkar;
        lastkar = 0;
        nextfield->read(kars, nextfield);
    }
    int iterations = 0;
    for (iterations = 0; nextfield; ++iterations)
    {
        kar = fgetc(fpin);

        if (kar == '\n')
        {
            ++newlines;
        }
        kars[0] = kar == EOF ? '\0' : (char)kar;

        char *plastkar = nextfield->read(kars, nextfield);
        if (kar == EOF)
        {
            lastkar = EOF;
            break;
        }
        else if (plastkar)
        {
            lastkar = *plastkar;
        }
    }
    if (nextfield && iterations != 0)
    {
        kars[0] = '\0';
        nextfield->read(kars, nextfield);
        if (nextfield)
        {
            int parts = nextfield->noOfFields();
            fprintf(stderr, "ERROR after reading %d bytes. When reaching the end of the input file, %d part%s of the input format specification string %s %s left unmatched.\n", iterations, parts, Iformat, parts > 1 ? "s" : "", parts > 1 ? "are" : "is");

            exit(0);
        }
    }
    if (tagfield)
    {
        tag = tagfield->getString();
    }
    return wordfield->getString();
}

const char *flattext::convert(const char *s, char *buf, const char *lastBufByte)
{
    REFER(buf)
    REFER(lastBufByte)
    return s;
}

void flattext::printUnsorted(FILE *fpo)
{
    unsigned long line = 0;
    unsigned long k;
    REFER(fpo) // unused
    for (k = 0; k < total; ++k)
    {
        while (k >= Lines[line] && line <= lineno)
        {
            Word::NewLinesAfterWord++;
            ++line;
        }
        if (tunsorted[k])
        {
            tunsorted[k]->print();
        }
        Word::NewLinesAfterWord = 0;
    }
}

void flattext::writeUnsorted(string &str)
{
    unsigned long line = 0;
    unsigned long k;
    REFER(fpo) // unused
    for (k = 0; k < total; ++k)
    {
        while (k >= Lines[line] && line <= lineno)
        {
            Word::NewLinesAfterWord++;
            ++line;
        }
        if (tunsorted[k])
        {
            tunsorted[k]->write(str);
        }
        Word::NewLinesAfterWord = 0;
    }
}

flattext::flattext(FILE *fpi, bool a_InputHasTags, char *Iformat, int keepPunctuation, bool nice,
                   unsigned long int size, bool treatSlashAsAlternativesSeparator)
    : text(a_InputHasTags, nice)
{
    StartOfLine = true;
    fields = 0;
    const char *Tag;
    field *wordfield = 0;
    field *tagfield = 0;
    field *format = 0;
    int slashFound = 0;
    if (nice)
        LOG1LINE("counting words");
    lineno = 0;
    unsigned long newlines;
        char *w;
    if (Iformat)
    {
        format = translateFormat(Iformat, wordfield, tagfield);
        if (!wordfield)
        {
            fprintf(stderr, "Input format %s must specify '$w'.\n", Iformat);

            exit(0);
        }
        while (total < size && (w = getwordI(fpi, Tag, format, wordfield, tagfield, newlines, Iformat)) != 0)
        {
            lineno += newlines;
            if (*w)
            {
                ++total;
                if (treatSlashAsAlternativesSeparator)
                {
                    total += findSlashes(w);
                }
            }
        }
    }
    else
    {
        while (total < size && (w = getword(fpi, Tag, InputHasTags, keepPunctuation, slashFound, newlines)) != 0)
        {
            lineno += newlines;
            if (*w)
            {
                ++total;
                if (slashFound && treatSlashAsAlternativesSeparator)
                    total += slashFound;
            }
        }
    }
    if (nice)
    {
        printf("... %lu words counted in %lu lines\n", total, lineno);

    }
    rewind(fpi);

    if (nice)
        LOG1LINE("allocating array of pointers to words");
    tunsorted = new const Word *[total];
    if (nice)
        LOG1LINE("allocating array of line offsets");
    Lines = new unsigned long int[lineno + 1];
    unsigned long L = lineno + 1;
    do
    {
        --L;
        Lines[L] = 0;
    } while (L != 0);
    if (nice)
        LOG1LINE("...allocated array");

    total = 0;
    if (nice)
        LOG1LINE("reading words");
    lineno = 0;
    if (InputHasTags)
    {
        if (format)
        {
            while (total < size && (w = getwordI(fpi, Tag, format, wordfield, tagfield, newlines, Iformat)) != 0)
            {
                if (Tag == 0)
                {
                    fprintf(stderr, "No POS-tag found. Is input really flat text? (Word: '%s' )", w);
                    exit(-7);
                }
                if (treatSlashAsAlternativesSeparator && findSlashes(w))
                    createTaggedAlternatives(w, Tag);
                else if (*w)
                    createTagged(w, Tag);
                lineno += newlines;
                StartOfLine = newlines != 0;
            }
        }
        else
        {
            while (total < size && (w = getword(fpi, Tag, true, true, slashFound, newlines)) != 0)
            {
                if (*w)
                {
                    if (!Tag)
                    {
                        if (total > 1 && lineno > 1)
                            fprintf(stderr, "Tag missing in word #%lu (\"%s\") (line #%lu).\n", total, w, lineno);
                        else
                            fprintf(stderr, "Tag missing in word #%lu (\"%s\") (line #%lu). (Is the input text tagged?)\n", total, w, lineno);

                        exit(0);
                    }
                    if (slashFound && treatSlashAsAlternativesSeparator)
                        createTaggedAlternatives(w, Tag);
                    else
                        createTagged(w, Tag);
                    StartOfLine = newlines != 0;
                }
                lineno += newlines;
            }
            reducedtotal = Word::reducedtotal;
        }
    }
    else
    {
        if (format)
        {
            while (total < size && (w = getwordI(fpi, Tag, format, wordfield, tagfield, newlines, Iformat)) != 0)
            {
                if (treatSlashAsAlternativesSeparator && findSlashes(w))
                    createUnTaggedAlternatives(w);
                else
                    createUnTagged(w);
                lineno += newlines;
                StartOfLine = newlines != 0;
            }
        }
        else
        {
            while (total < size && (w = getword(fpi, Tag, false, keepPunctuation, slashFound, newlines)) != 0)
            {
                if (slashFound && treatSlashAsAlternativesSeparator)
                    createUnTaggedAlternatives(w);
                else
                    createUnTagged(w);
                lineno += newlines;
                StartOfLine = newlines != 0;
            }
        }
    }
    rewind(fpi);

    makeList();
    if (nice)
        LOG1LINE("...read words from flat text");
}

flattext::flattext(string str, int keepPunctuation, bool nice,
                   unsigned long int size, bool treatSlashAsAlternativesSeparator)
    : text(false, nice)
{
    StartOfLine = true;
    fields = 0;
    const char *Tag;
    
    int slashFound = 0;
    
    if (nice)
        LOG1LINE("counting words");
    
    lineno = 0;
    string::size_type pos = 0;
    
    unsigned long newlines;
    char *w;

    while (total < size && (w = getword(str, pos, Tag, keepPunctuation, slashFound, newlines)) != 0)
    {
        lineno += newlines;
        if (*w)
        {
            ++total;
            if (slashFound && treatSlashAsAlternativesSeparator)
                total += slashFound;
        }
    }
    if (nice)
    {
        printf("... %lu words counted in %lu lines\n", total, lineno);
    }

    if (nice)
        LOG1LINE("allocating array of pointers to words");
    tunsorted = new const Word *[total];
    if (nice)
        LOG1LINE("allocating array of line offsets");
    Lines = new unsigned long int[lineno + 1];
    unsigned long L = lineno + 1;
    do
    {
        --L;
        Lines[L] = 0;
    } while (L != 0);
    
    if (nice)
        LOG1LINE("...allocated array");

    total = 0;
    if (nice)
        LOG1LINE("reading words");
    
    lineno = 0;
    pos = 0;

    while (total < size && (w = getword(str, pos, Tag, keepPunctuation, slashFound, newlines)) != 0)
    {
        if (slashFound && treatSlashAsAlternativesSeparator)
            createUnTaggedAlternatives(w);
        else
            createUnTagged(w);
        lineno += newlines;
        StartOfLine = newlines != 0;
    }

    makeList();
    if (nice)
        LOG1LINE("...read words from flat text");
}
#endif
