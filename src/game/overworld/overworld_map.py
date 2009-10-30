"""
Library for loading overworld maps in python.

If called as a script, prints the map in JSON.
"""

from lib import *

def read_map(input_stream):
    result = {'edges': [], 'name': None, 'image': None, 'zones': []}
    for words in read_lines_of_words(input_stream):
        if len(words) == 0: continue
        command = words[0]
        if command == 'name':
            command, map_name = words
            result['name'] = map_name
            continue
        if command == 'zone':
            command, zone_name, x, y = words
            result['zones'].append((zone_name, int(x), int(y)))
            continue
        if command == 'edge':
            command, zone1, zone2 = words
            result['edges'].append((zone1, zone2))
            continue
        if command == 'image':
            command, filename = words
            result['image'] = filename
    return result

def load_map(filename):
    f = open(filename)
    result = read_map(f)
    f.close()
    return result

if __name__ == '__main__':
    import sys
    command, filename = sys.argv
    print load_map(filename)
