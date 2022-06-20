#include <stdio.h>
#include <iostream>

#include "Python.h"
#include "py_cpp_conversion.hpp"

#include "lemmatiser.h"
#include "option.h"
#if defined PROGLEMMATISE
#include "word.h"
#include "caseconv.h"
#endif

#include <stdio.h>
#include <stddef.h>
#include <string>

using namespace std;

using namespace std;

// ------------------- Class wrappers -------------------

PyObject *construct(PyObject *self, PyObject *args) {
    vector<string> s;
    PyObject *pobj;
    PyArg_ParseTuple(args, "O", &pobj);
    
    s = listToVectorString(pobj);

    optionStruct Option;

    Option.doSwitch('L', (char *)"", "");
    Option.doSwitch('f', (char *)"rules/flexrules_nl", "");
    Option.doSwitch('d', (char *)"rules/dict_nl", "");
    Option.doSwitch('c', (char *)"$b ", "");
    Option.doSwitch('b', (char *)"$w", "");

    Lemmatiser *lemmatiser = new Lemmatiser(Option);
    
    PyObject *lemmatiserCapsule = PyCapsule_New((void *)lemmatiser, "lemmaPtr", NULL);
    PyCapsule_SetPointer(lemmatiserCapsule, (void *)lemmatiser);
    
    return Py_BuildValue("O", lemmatiserCapsule);
}

PyObject *lemmatiseStrings(PyObject *self, PyObject *args) {
    vector<string> s, result;
    PyObject *pobj;
    
    PyObject *lemmatiserCapsule_;
    PyArg_ParseTuple(args, "OO", &lemmatiserCapsule_, &pobj);
    
    s = listToVectorString(pobj);
    
    Lemmatiser *lemmatiser = (Lemmatiser *)PyCapsule_GetPointer(lemmatiserCapsule_, "lemmaPtr");

    for (size_t i = 0; i < s.size(); i++) {
        result.push_back(lemmatiser->LemmatiseString(s[i]));
    }
    
    return vectorStringToList(result);
}

PyObject *delete_object(PyObject *self, PyObject *args) {
    PyObject *lemmatiserCapsule_;
    PyArg_ParseTuple(args, "O", &lemmatiserCapsule_);
    
    Lemmatiser *lemmatiser = (Lemmatiser *)PyCapsule_GetPointer(lemmatiserCapsule_, "lemmaPtr");
    
    Word::deleteStaticMembers();
    
    delete lemmatiser;
    
    return Py_BuildValue("");
}

PyMethodDef LemmatiserFunctions[] = {
    {"construct",
      construct, METH_VARARGS,
     "Create `Lemmatiser` object"},
    
    {"lemmatiseStrings",
      lemmatiseStrings, METH_VARARGS,
     "Lemmatise a list of strings"},
    
    {"delete_object",
      delete_object, METH_VARARGS,
     "Delete `Lemmatiser` object"},

    {NULL, NULL, 0, NULL}      // Last function description must be empty.
                               // Otherwise, it will create seg fault while
                               // importing the module.
};


struct PyModuleDef LemmatiserModule = {
/*
 *  Structure which defines the module.
 *
 *  For more info look at: https://docs.python.org/3/c-api/module.html
 *
 */
   PyModuleDef_HEAD_INIT,
   "pycstlemma",
   "Lemmatiser for Dutch sentences", // Docstring for the module - in this case empty.
   -1,                   // Used by sub-interpreters, if you do not know what
                         // it is then you do not need it, keep -1 .
   LemmatiserFunctions
};


PyMODINIT_FUNC PyInit_Lemmatiser(void) {
    return PyModule_Create(&LemmatiserModule);
}