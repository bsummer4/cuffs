"""
A small python library for parsing all available information on game
maps.  The maps list should be in data/maps.

# Example Usage

    import map_parse, pprint, os
    os.chdir("../data")
    maps = map_parse.Maps("maps")
    print maps
"""

class Maps:
    def __repr__(self): return repr(self.maps)
    def __str__(self): return str(self.maps)
    def __init__(self, maplistfile):
        self.maps = {}
        f = open(maplistfile)
        for line_text in f.xreadlines():
            line = line_text.split()
            if not len(line): continue
            mapname, gravity_wind = (line[0],
                                     set([tuple(map(float, pair.split(":")))
                                          for pair in line[1:]]))
            self.maps[mapname] = Map(mapname, gravity_wind)
        f.close()

class Map:
    def tup(self): return (self.mapname,
                           self.imagefile,
                           self.gravity_wind,
                           self.spawnpoints)
    def __repr__(self): return repr(self.tup())
    def __str__(self): return str(self.tup())
    def __init__(self, mapname, gravity_wind):
        f = open(mapname + ".map")
        self.mapname = mapname
        self.gravity_wind = gravity_wind
        self.imagefile = None
        self.spawnpoints = set()
        for line_text in f.xreadlines():
            line = line_text.split()
            if not len(line): continue
            command, args = line[0], line[1:]
            if command == "map":
                (self.imagefile,) = args
            elif command == "spawn":
                (x, y) = map(int, args)
                self.spawnpoints.add((x, y))
            else:
                f.close()
                raise Exception (
                    "Invalid Command '%s' in map file on line:\n  %s" %
                    (command, line_text))
        f.close()
