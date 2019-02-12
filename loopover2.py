# import pyximport; pyximport.install(pyimport=True)
# Uncomment the line above to have cython try to compile things. It may be unstable but could be faster.

import copy
from multiprocessing import Pool
import multiprocessing
import itertools
import defs2
import tracemalloc


# import os


def split_boards(new_boards):
    return set().union(*[newBoardsS[0] for newBoardsS in new_boards]), sum([countS[1] for countS in new_boards])


# Real code starts below


if __name__ == "__main__":

    # boards = {((1,2,3,4,5),(6,7,8,9,10),(11,12,13,14,15),(16,17,18,19,20),(21,22,23,24,25))}
    boards = {((1, 2, 3, 4), (5, 6, 7, 8), (9, 10, 11, 12), (13, 14, 15, 16))}
    # boards = {((1,2,3),(4,5,6),(7,8,9))}
    # boards = {((1,2),(3,4))}

    # boards = {((1,2,3,4,5),(6,7,8,9,10))}
    # boards = {((1,2,3,4,5,6),(7,8,9,10,11,12))}

    checkMem = False
    pool = Pool(multiprocessing.cpu_count())
    # pool = Pool(1)

    from datetime import datetime

    timeStart = datetime.today()
    found = True
    number = 0
    depth = 0
    newBoards = copy.deepcopy(boards)
    # oldBoards = set()

    if checkMem:
        tracemalloc.start()
    timeLast = datetime.today()
    print("Init took: " + str(datetime.today() - timeStart))
    while found:
        # oldBoards = copy.deepcopy(newBoards)
        newBoards, newCount = split_boards(
            pool.map(defs2.get_new_positions, newBoards, int(len(newBoards) / multiprocessing.cpu_count()) + 1))

        sames = set()
        for board in defs2.InChunks(newBoards, multiprocessing.cpu_count()):
            same = []
            # for aBoard in defs2.InChunks(boards,multiprocessing.cpu_count()):
            # same.append(board & aBoard)
            sames.update(*pool.starmap(defs2.and_them, zip(itertools.repeat(board),
                                                           defs2.InChunks(boards, multiprocessing.cpu_count())),
                                       int(len(newBoards) / multiprocessing.cpu_count()) + 1))
        # same = set().union(*same)
        # sames.update(same)
        newBoards -= sames

        newBoards = newBoards - boards
        boards.update(newBoards)
        number += newCount
        depth += 1
        if not bool(newBoards):
            found = False

        print("Number of combos tested: " + str(number))
        print("Number of combos unique: " + str(len(boards)))
        print("Depth: " + str(depth))
        tmpTime = datetime.today() - timeLast
        print("Took: " + str(tmpTime))
        print("Boards per seccond: " + str(newCount / (tmpTime).total_seconds()))
        timeLast = datetime.today()
        tmpTime = datetime.today() - timeStart
        print("Total: " + str(tmpTime))
        print("Overall boards per seccond: " + str(number / (tmpTime).total_seconds()))
        print()

    pool.close()

    print("Number of combos tested:" + str(number))
    print("Number of unique combos:" + str(len(boards)))
    print("Depth:" + str(depth - 1))
    tmpTime = datetime.today() - timeStart
    print("Total Time:" + str(str(tmpTime)))
    print("Overall boards per seccond: " + str(number / (tmpTime).total_seconds()))
    print("Total size of boards: " + str(boards.__sizeof__()) + " bytes")
    if checkMem:
        snapshot = tracemalloc.take_snapshot()
        defs2.display_top(snapshot)

# file_object  = open("tmp", "w")
# [file_object.write(str(i)) for i in oldBoards]
