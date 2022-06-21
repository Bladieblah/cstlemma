/*
CSTLEMMA - trainable lemmatiser

Copyright (C) 2002, 2007  Center for Sprogteknologi, University of Copenhagen

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
#define CSTLEMMAVERSION "8.2.0"
#define CSTLEMMADATE "2021.12.03"
#define CSTLEMMACOPYRIGHT "2002-2021 Center for Sprogteknologi"

#include <iostream>
#include "option.h"
#include "lemmatiser.h"
#include "word.h"

using namespace std;

int main(int argc, char *argv[])
{   
    optionStruct Option;
    
    cout << "reading options\n";
    // OptReturnTp optResult = Option.readArgs(6, (char **)&options);
    // OptReturnTp optResult = Option.readArgs(argc, argv);
    Option.doSwitch('L', "", argv[0]);
    Option.doSwitch('f', "rules/flexrules_nl", argv[0]);
    Option.doSwitch('d', "rules/dict_nl", argv[0]);
    Option.doSwitch('c', (char *)"$b ", "");
    Option.doSwitch('b', (char *)"$w", "");
    cout << "done options\n";
    
    cout << "lemmatiser\n";
    Lemmatiser theLemmatiser(Option);
    cout << "lemmatising\n";
    cout << theLemmatiser.LemmatiseString(string("hoi ik ga een abonnement opzeggen heb ik opgezegd ik zeg op")) << endl;
    cout << "done\n";
    Word::deleteStaticMembers();
    
    return 0;
}