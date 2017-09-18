import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives): # __init__ has two input arguments and one self referring
        """Initialize Analyzer."""

        # TODO
        # Load postive and negative words
        
        # positive words
        # use "self" so that we referring to the same objects instance variable
        self.positives_set=set() # initialize an empty set for all postives words
        file = open(positives, 'r') # open channel for file reading, read positives dictionary
        for line in file: # read dictionary line by line
            if line.startswith(';') != True: # exclude the comments that starting with ';'
                self.positives_set.add(line.rstrip("\n")) # omit leading/trailing whitespace
        file.close() # after reading, close file channel

        # load negative words
        self.negatives_set=set()
        file = open(negatives, 'r')
        for line in file:
            if line.startswith(';')!=True:
                self.negatives_set.add(line.rstrip("\n"))
        file.close()

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""

        # TODO
        # use tokenizer to split sentences into words
        tokenizer = nltk.tokenize.TweetTokenizer()
        # in text analysis, we called splits words "tokens"
        tokens = tokenizer.tokenize(text)
        
        # Assign each word in text a value (-1, 0, 1)
        # Calculate text's total score
        score=0
        for word in tokens: # Loop search every word in tokens
            if word.lower() in self.positives_set: # if word is in positives_set, then score +1
                score += 1
            elif word.lower() in self.negatives_set: # if word is in negative_set, then score -1
                score += -1
            else:
                score += 0
        return score
