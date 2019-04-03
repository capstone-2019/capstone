import argparse

def hi(x, y):
    return x + y

parser = argparse.ArgumentParser(description='process some integers')
parser.add_argument('x')
parser.add_argument('y')

args = parser.parse_args()
print(args.x)
print(args.y)
print(hi(args.x, args.y))
