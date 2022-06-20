import cLemmatiser

class CstLemmatiser:
    def __init__(self):
        self.construct()
    
    def construct(self):
        self.lemmatiser_capsule = cLemmatiser.construct()

    def lemmatise_strings(self, strings):
        return cLemmatiser.lemmatiseStrings(self.lemmatiser_capsule, strings)

    def __delete__(self):
        cLemmatiser.delete_object(self.lemmatiser_capsule)

    def __getstate__(self):
        return []

    def __setstate__(self, state):
        self.construct()
        return