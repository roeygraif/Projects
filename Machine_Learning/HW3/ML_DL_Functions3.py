import numpy as np
from torch import nn as nn
def convert_words_to_indices(sents,vocab_stoi): # 10% grade
    """
    This function takes a list of sentences 
    input: list of list of words [[word,word,..,word],..,[word,..,word]] where each word is a string with no spaces
    and returns a new list with the same structure, but where each word is replaced by its index in `vocab_stoi`.
    output: list of lists of integers [[int,int,..,int],..,[int,..,int]] where each int is the idx of the word according to vocab_stoi

    Example:
    >>> convert_words_to_indices([['one', 'in', 'five', 'are', 'over', 'here'], ['other', 'one', 'since', 'yesterday'], ['you']])
    [[148, 98, 70, 23, 154, 89], [151, 148, 181, 246], [248]]
    """
    matrix = []
    for sentence in sents:
        new_row = [vocab_stoi[word] for word in sentence]
        matrix.append(new_row)
    return matrix

    # Write your code here

def generate_4grams(seqs): # 10% grade
    """
    This function takes a list of sentences (list of lists) and returns
    a new list containing the 4-grams (four consequentively occuring words)
    that appear in the sentences. Note that a unique 4-gram can appear multiple
    times, one per each time that the 4-gram appears in the data parameter `seqs`.

    Example:

    >>> generate_4grams([[148, 98, 70, 23, 154, 89], [151, 148, 181, 246], [248]])
    [[148, 98, 70, 23], [98, 70, 23, 154], [70, 23, 154, 89], [151, 148, 181, 246]]
    >>> generate_4grams([[1, 1, 1, 1, 1]])
    [[1, 1, 1, 1], [1, 1, 1, 1]]
    """
    matrix = []
    for sentence in seqs:
        for i in range(len(sentence) - 3):
            new_row = sentence[i:i + 4]
            matrix.append(new_row)
    return matrix
            

def make_onehot(data): # 10% grade
    """
    Convert one batch of data in the index notation into its corresponding onehot
    notation. Remember, the function should work for both xt and st.

    input - vector with shape D (1D or 2D)
    output - vector with shape (D,250)
    """

    # Write your code here
    flat_data = data.flatten()      #when its flattened the function will work for both 1D and 2D inputs
    onehot = np.zeros((flat_data.size, 250))
    onehot[np.arange(flat_data.size), flat_data] = 1    #this is an advanced way to place 1 in the index of all flat_data elements from 0 to flat_data.size
    return onehot.reshape(*data.shape, 250)

class PyTorchMLP(nn.Module): # 35% grade for each model
    def __init__(self):
        super(PyTorchMLP, self).__init__()
        self.num_hidden = 300 # TODO: choose number of hidden neurons
        self.layer1 = nn.Linear(750, self.num_hidden)
        self.layer2 = nn.Linear(self.num_hidden, 250)
        self.relu = nn.ReLU()
        
    def forward(self, inp):
        inp = inp.reshape([-1, 750])
        # TODO: complete this function
        # Note that we will be using the nn.CrossEntropyLoss(), which computes the softmax operation internally, as loss criterion

        x = self.layer1(inp)
        x = self.relu(x)            #only one hidden layer
        x = self.layer2(x)
        return x  