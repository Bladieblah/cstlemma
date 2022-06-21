import cLemmatiser

class CstLemmatiser:
    def __init__(self, flex_file, dict_file):
        self.flex_file = flex_file
        self.dict_file = dict_file
        self.construct()
    
    def construct(self):
        self.lemmatiser_capsule = cLemmatiser.construct(self.flex_file, self.dict_file)

    def lemmatise_string(self, string):
        return cLemmatiser.lemmatiseString(self.lemmatiser_capsule, string)

    def lemmatise_strings(self, strings):
        return cLemmatiser.lemmatiseStrings(self.lemmatiser_capsule, strings)

    def __delete__(self):
        cLemmatiser.delete_object(self.lemmatiser_capsule)

    def __getstate__(self):
        return [self.flex_file, self.dict_file]

    def __setstate__(self, state):
        self.flex_file, self.dict_file = state
        self.construct()
        return