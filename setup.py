from setuptools import Extension, setup

# Definition of extension modules
pycstlemma = Extension('pycstlemma',
                 sources = [
                    'src/pycstlemma/src/applyrules.cpp',
                    'src/pycstlemma/src/argopt.cpp',
                    'src/pycstlemma/src/basefrm.cpp',
                    'src/pycstlemma/src/basefrmpntr.cpp',
                    'src/pycstlemma/src/caseconv.cpp',
                    'src/pycstlemma/src/cstlemma.cpp',
                    'src/pycstlemma/src/dictionary.cpp',
                    'src/pycstlemma/src/field.cpp',
                    'src/pycstlemma/src/flattext.cpp',
                    'src/pycstlemma/src/flex.cpp',
                    'src/pycstlemma/src/freqfile.cpp',
                    'src/pycstlemma/src/function.cpp',
                    'src/pycstlemma/src/functiontree.cpp',
                    'src/pycstlemma/src/lemmatise.cpp',
                    'src/pycstlemma/src/lemmatiser.cpp',
                    'src/pycstlemma/src/lemmtags.cpp',
                    'src/pycstlemma/src/lext.cpp',
                    'src/pycstlemma/src/makedict.cpp',
                    'src/pycstlemma/src/makesuffixflex.cpp',
                    'src/pycstlemma/src/option.cpp',
                    'src/pycstlemma/src/outputclass.cpp',
                    'src/pycstlemma/src/readfreq.cpp',
                    'src/pycstlemma/src/readlemm.cpp',
                    'src/pycstlemma/src/tags.cpp',
                    'src/pycstlemma/src/text.cpp',
                    'src/pycstlemma/src/word.cpp',
                    'src/pycstlemma/src/wordReader.cpp',
                    'src/pycstlemma/src/XMLtext.cpp',

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
