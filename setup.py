from setuptools import Extension, setup

import os

print('CWD =', os.getcwd())

# Definition of extension modules
pycstlemma = Extension('cLemmatiser',
                 sources = [
                    'src/cstlemma/src/cstlemma_wrapper.cpp',
                    'src/cstlemma/src/py_cpp_conversion.cpp',
                    
                    'src/cstlemma/src/applyrules.cpp',
                    'src/cstlemma/src/argopt.cpp',
                    'src/cstlemma/src/basefrm.cpp',
                    'src/cstlemma/src/basefrmpntr.cpp',
                    'src/cstlemma/src/caseconv.cpp',
                    'src/cstlemma/src/dictionary.cpp',
                    'src/cstlemma/src/field.cpp',
                    'src/cstlemma/src/flattext.cpp',
                    'src/cstlemma/src/flex.cpp',
                    'src/cstlemma/src/freqfile.cpp',
                    'src/cstlemma/src/function.cpp',
                    'src/cstlemma/src/functiontree.cpp',
                    'src/cstlemma/src/lemmatise.cpp',
                    'src/cstlemma/src/lemmatiser.cpp',
                    'src/cstlemma/src/lemmtags.cpp',
                    'src/cstlemma/src/lext.cpp',
                    'src/cstlemma/src/makedict.cpp',
                    'src/cstlemma/src/makesuffixflex.cpp',
                    'src/cstlemma/src/option.cpp',
                    'src/cstlemma/src/outputclass.cpp',
                    'src/cstlemma/src/readfreq.cpp',
                    'src/cstlemma/src/readlemm.cpp',
                    'src/cstlemma/src/tags.cpp',
                    'src/cstlemma/src/text.cpp',
                    'src/cstlemma/src/word.cpp',
                    'src/cstlemma/src/wordReader.cpp',
                    'src/cstlemma/src/XMLtext.cpp',

                    'src/hashmap/src/hashmap.cpp',

                    'src/letterfunc/src/utf8func.cpp',
                    'src/letterfunc/src/entities.cpp',
                    'src/letterfunc/src/letter.cpp',
                    'src/letterfunc/src/letterfunc.cpp',

                    'src/parsesgml/src/parsesgml.cpp',
                ],
                 include_dirs = [
                    'src/cstlemma/src',
                    'src/hashmap/src',
                    'src/letterfunc/src',
                    'src/parsesgml/src',
                ],
                extra_compile_args=[
                    '-std=c++11',
                    '-Wno-reorder'
                ]
            )

setup(ext_modules = [pycstlemma])
