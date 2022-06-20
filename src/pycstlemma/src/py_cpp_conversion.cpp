#include <stdio.h>
#include <iostream>

#include "Python.h"
#include "py_cpp_conversion.hpp"

using namespace std;

PyObject *vectorStringToList(const vector<string> &data) {
    PyObject *listObj = PyList_New(data.size());
	if (!listObj) throw logic_error("Unable to allocate memory for Python list 1");
	
	for (unsigned int i = 0; i < data.size(); i++) {
		PyObject *num = PyUnicode_FromString(data[i].c_str());
		if (!num) {
			Py_DECREF(listObj);
			throw logic_error("Unable to allocate memory for Python list 2");
		}
		PyList_SET_ITEM(listObj, i, num);
	}
    
	return listObj;
}

vector<string> listToVectorString(PyObject *incoming) {
	vector<string> data;
    
    if (PyList_Check(incoming)) {
        for(Py_ssize_t i = 0; i < PyList_Size(incoming); i++) {
            PyObject *value = PyList_GetItem(incoming, i);
            Py_ssize_t size;
            const char *ptr = PyUnicode_AsUTF8AndSize(value, &size);
            string s = ptr;
            data.push_back(s);
        }
    } 
    else {
        throw logic_error("Passed PyObject pointer was not a list!");
    }

	return data;
}
