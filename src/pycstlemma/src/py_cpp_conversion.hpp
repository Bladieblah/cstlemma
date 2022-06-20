#ifndef PY_CPP_CONVERSION_H
#define PY_CPP_CONVERSION_H

#include <string>
#include <vector>

#include "Python.h"

PyObject *vectorStringToList(const std::vector<std::string> &data);

std::vector<std::string> listToVectorString(PyObject *incoming);

#endif