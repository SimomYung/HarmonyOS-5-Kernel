#!/usr/bin/env python3
# coding:utf-8

# Copyright (c) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Description: Check whether there is a circle in the dependency graph of the fine-grained locks
# Author: Huawei OS Kernel Lab
# Create: Sat Dec 25 10:25:17 2021

from __future__ import absolute_import

import sys


class KlockChecker:
    """ klock checker """
    def __init__(self):
        """ init """
        self.lock_orders = {}
        self.visited = {}

    def print_circle(self):
        """ print circle """
        for i in self.lock_orders:
            if self.visited.get(i) == 1:
                print(i)

    def dfs(self, c):
        """ topological sorting via DFS """
        # self.visited[n] == 0: never visited
        # self.visited[n] == 1: visited before but not all neighbours are visited
        # self.visited[n] == 2: visited before and all neighbours are visited
        self.visited[c] = 1
        for i in self.lock_orders.get(c):
            if i not in self.visited:
                print("Warn: lack of dependency about \"%s\"" % (i))
                self.visited[i] = 2
            if self.visited.get(i) == 1:
                print("there is a lock circle in:")
                self.print_circle()
                return 0
            elif self.visited.get(i) == 0:
                if self.dfs(i) == 0:
                    return 0
        self.visited[c] = 2
        return 1


    def init_order_line(self, line):
        """ init order line """
        orderline = line[11:(len(line) - 1)]
        orders = orderline.split(',')
        depth = int(orders[0].strip())
        node = orders[1].strip()
        if orders[2] == ' NULL':
            self.lock_orders[node] = []
        else:
            depth_tmp = 0
            tmp = []
            for i in orders[2:]:
                tmp.append(i.strip())
                self.lock_orders[node] = tmp
                depth_tmp += 1
            if depth_tmp != depth:
                print("Unconsistent lock order depth, should be ", depth_tmp)
                print(line)
                return 0
        self.visited[node] = 0
        return 1


    def process_lines(self, fklocks):
        """ read and process file """
        orderline = ""
        validline = False
        parse_valid = True
        for line in fklocks.readlines():
            line = line.rstrip()
            line = line.strip()
            sameline = False
            if len(line) > 11 and line[0:11] == 'lock_order(':
                orderline = ""
                validline = True
                line = line.replace("\\", "")
                orderline = orderline.join(line)
                sameline = True
            elif line.find("\\") != -1 and validline:
                if not sameline:
                    line = line.replace("\\", "")
                    orderline = ''.join([orderline, line])

            if line.find(")") != -1 and validline:
                if not sameline:
                    orderline = ''.join([orderline, line])
                if self.init_order_line(orderline) == 0:
                    parse_valid = False
                validline = False
        return parse_valid

    def process(self, lock_file):
        """ process check """
        with open(lock_file, "r") as fklocks:
            if self.process_lines(fklocks) == 0:
                return 1

        fklocks.close()

        for i in self.lock_orders:
            if self.dfs(i) == 0:
                return 1
        return 0


def main():
    """ entry of the script """
    if len(sys.argv) < 2:
        print("Error: lack of the argv of lock dependency file")
        sys.exit(1)

    check = KlockChecker()
    if check.process(sys.argv[1]):
        sys.exit(1)


if __name__ == "__main__":
    main()
