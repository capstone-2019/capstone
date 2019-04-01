from __future__ import print_function
import sys

def main():
    for line in sys.stdin:
        print("Child Got Message: {}".format(line), end="")

if __name__ == '__main__':
    main()
