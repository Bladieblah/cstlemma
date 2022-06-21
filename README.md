# PyCSTLemma

A hacky python wrapper in/around the original cstlemma library. It took quite some nasty refactoring to get it working, I might clean it up and do it properly some day but for now I can't be bothered.

## Installing

Install by either cloning the repo and running

```bash
pip install .
```

or running

```
pip install git+
```

## Usage

In order to use the package, you will need flexrule and dictionary files for the desired language. You can find those files [here](https://github.com/kuhumcst/texton-linguistic-resources). Once downloaded, you can reference them when constructing the lemmatiser.

```python
import pycstlemma.cst_lemmatiser

lemmatiser = cst_lemmatiser.CstLemmatiser('flexrules', 'dict')
lemmatiser.lemmatise_strings([
    'i am an example sentence',
    'me too, how coincidental'
])
```