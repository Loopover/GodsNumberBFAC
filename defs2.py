# import pyximport; pyximport.install(pyimport=True)
import itertools
import linecache
import os
import tracemalloc


def switch_the_stuff(board, current_num, start, j, side):
    is_side = current_num == ((-side ^ 1) % 3) * (len(board) - 1)
    return start[:j] + (board[int(is_side) * (-((side ^ 1) % 3)) +
                              int(not is_side) * (current_num + side)][j],) + start[j + 1:]


def get_new_positions(board):
    loc_number = 0
    found = set()
    for k in [1, -1]:
        for j in range(len(board)):
            loc_number += 2
            c = board[:j] + (board[j][k:] + board[j][:k],) + board[j + 1:]
            found.add(tuple(c))
            c = tuple(switch_the_stuff(board, currentNum, start, j, k) for currentNum, start in enumerate(board))
            found.add(tuple(c))
    return found, loc_number


def and_them(board, a_board):
    return board & a_board


def display_top(snapshot, key_type='lineno', limit=3):
    snapshot = snapshot.filter_traces((
        tracemalloc.Filter(False, "<frozen importlib._bootstrap>"),
        tracemalloc.Filter(False, "<unknown>"),
    ))
    top_stats = snapshot.statistics(key_type)

    print("Top %s lines" % limit)
    for index, stat in enumerate(top_stats[:limit], 1):
        frame = stat.traceback[0]
        # replace "/path/to/module/file.py" with "module/file.py"
        filename = os.sep.join(frame.filename.split(os.sep)[-2:])
        print("#%s: %s:%s: %.1f KiB" % (index, filename, frame.lineno, stat.size / 1024))
        line = linecache.getline(frame.filename, frame.lineno).strip()
        if line:
            print('    %s' % line)

    other = top_stats[limit:]
    if other:
        size = sum(stat.size for stat in other)
        print("%s other: %.1f KiB" % (len(other), size / 1024))
    total = sum(stat.size for stat in top_stats)
    print("Total allocated size: %.1f KiB" % (total / 1024))


class InChunks:
    def __init__(self, list_in, chunks):
        self.listIn = list_in
        self.length = len(list_in)
        self.chunkSize = int(self.length / chunks) + 1
        self.num = 0
        self.done = False

    def __iter__(self):
        return self

    def __next__(self):
        if not self.done:
            if self.num + self.chunkSize <= self.length:
                old_num = self.num
                self.num += self.chunkSize
                return set(itertools.islice(self.listIn, old_num, self.num))
            else:
                old_num = self.num
                self.num = self.length
                self.done = True
                return set(itertools.islice(self.listIn, old_num, self.num))
        else:
            raise StopIteration
