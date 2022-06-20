import pycstlemma

class CstLemmatiser:
    def __init__(self):
        self.construct()
    
    def construct(self):
        self.lemmatiser_capsule = pycstlemma.construct()

    def lemmatise_strings(self, strings):
        return pycstlemma.lemmatiseStrings(self.lemmatiser_capsule, strings)

    def __delete__(self):
        pycstlemma.delete_object(self.lemmatiser_capsule)

    def __getstate__(self):
        return []

    def __setstate__(self, state):
        self.construct()
        return