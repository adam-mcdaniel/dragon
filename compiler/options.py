from sys import stdin
from argparse import ArgumentParser, FileType


parser = ArgumentParser(
    description='Compiler for a mini language that targets the Dragon VM')

parser.add_argument('-f', '--file', dest="file", metavar='script path', type=str,
                    default=None, help='The path to a file to compile')

parser.add_argument('script', metavar='script text', nargs='*', type=FileType('r'),
                     default=stdin, help='The script text to compile')


args = parser.parse_args()

SCRIPT = ""
if args.file:
    SCRIPT = open(args.file).read()
elif args.script:
    SCRIPT = '\n'.join(map(str, args.script))


