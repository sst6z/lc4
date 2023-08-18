import sys

from lc4.lc4 import LC4

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print('provide at least one image')
    else:
        lc4 = LC4(sys.argv[1])
        lc4.run()
