from functools import *

def read_lines_of_words(stream):
    return [line.strip().split() for line in stream.readlines()]

def combine_dicts2(x, y):
    result = {}
    for key in x: result[key] = x[key]
    for key in y: result[key] = y[key]
    return result

def combine_dicts(*xs):
    return reduce(combine_dicts2, xs)

def assoc(item, alist):
    for entry in alist:
        if entry[0] == item:
            return entry
