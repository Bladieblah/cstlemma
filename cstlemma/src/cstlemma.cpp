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

#include "lemmatiser.h"
#include "option.h"
#if defined PROGLEMMATISE
#include "word.h"
#include "caseconv.h"
#endif

#include <stdio.h>
#include <stddef.h>
#include <string>
#include <iostream>

using namespace std;

// int main(int argc, char *argv[])
// {
//     if (argc == 1)
//     {
//         LOG1LINE("");
//         LOG1LINE("CSTLEMMA version " CSTLEMMAVERSION " (" CSTLEMMADATE ")");
//         LOG1LINE("Copyright (C) " CSTLEMMACOPYRIGHT);
//         if (sizeof(ptrdiff_t) == 8)
//             LOG1LINE("64-bit");
//         else
//             LOG1LINE("32-bit");
//         LOG1LINE("CSTLEMMA comes with ABSOLUTELY NO WARRANTY; for details use option -w.");
//         LOG1LINE("This is free software, and you are welcome to redistribute it under");
//         LOG1LINE("certain conditions; use option -r for details.");
//         LOG1LINE("");
//         LOG1LINE("");
//         LOG1LINE("Use option -h for usage.");
//         return 0;
//     }

//     optionStruct Option;
//     int ret;

//     OptReturnTp optResult = Option.readArgs(argc, argv);
//     if (optResult == OptReturnTp::Error)
//     {
//         return 1;
//     }

//     if (optResult == OptReturnTp::Leave)
//     { // option -r, -w, -? or -h
//         return 0;
//     }

//     Lemmatiser theLemmatiser(Option);
//     if ((ret = theLemmatiser.getStatus()) == 0)
//     {
//         switch (Option.whattodo)
//         {
//         case whattodoTp::MAKEDICT:
//         {
//             break;
//         }
//         case whattodoTp::MAKEFLEXPATTERNS:
//         {
//             break;
//         }
//         default:
//         {
//             ret = theLemmatiser.LemmatiseFile();
//         }
//         }
//     }
//     Word::deleteStaticMembers();
//     return ret;
// }

int main(int argc, char *argv[])
{
    char options[6][50] = {"./cstlemma", "-L", "-f", "rules/flexrules_nl", "-d", "rules/dict_nl"};

    optionStruct Option;

    cout << "reading options\n";
    // OptReturnTp optResult = Option.readArgs(6, (char **)&options);
    // OptReturnTp optResult = Option.readArgs(argc, argv);
    Option.doSwitch('L', "", argv[0]);
    Option.doSwitch('y', "", argv[0]);
    Option.doSwitch('f', "rules/flexrules_nl", argv[0]);
    Option.doSwitch('d', "rules/dict_nl", argv[0]);
    cout << "done options\n";

    cout << "lemmatiser\n";
    Lemmatiser theLemmatiser(Option);
    cout << "lemmatising\n";
    cout << theLemmatiser.LemmatiseString(string("hoi ik ga een abonnement opzeggen heb ik opgezegd ik zeg op")) << endl;
    cout << "done\n";
    Word::deleteStaticMembers();

    return 0;
}