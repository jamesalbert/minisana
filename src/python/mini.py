#!/usr/bin/env python

from copy import deepcopy
import numpy as np


net = np.array([
    [0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 0]
])

sol = {
    '1': [3, 4],
    '2': [3, 6],
    '3': [5, 4],
    '4': [5, 6]
}


class State(object):
    def __init__(self, coords=None):
        self.coords = coords or {
            '1': [0, 0],
            '2': [3, 5],
            '3': [4, 1],
            '4': [5, 2]
        }
        self.net = self.gennet()

    def gennet(self):
        local_net = deepcopy(net)
        for node, coords in self.coords.items():
            local_net[coords[0]][coords[1]] = int(node)
        return local_net

    def swap(self, n1, n2):
        new_coords = deepcopy(self.coords)
        old_n1_coords = deepcopy(self.coords[n1])
        new_coords[n1] = new_coords[n2]
        new_coords[n2] = old_n1_coords
        s = State(new_coords)
        return s

    def move(self, node, coords):
        new_coords = deepcopy(self.coords)
        new_coords[node] = coords
        s = State(new_coords)
        return s

    def neighbors(self):
        neighbors = list()
        for n1, coords in self.coords.items():
            for n2, coords in self.coords.items():
                if n1 != n2:
                    neighbors.append(self.swap(n1, n2))
        for node, coords in self.coords.items():
            for i in range(0, len(net)):
                for j in range(0, len(net[0])):
                    if [i, j] not in self.coords.values():
                        neighbors.append(self.move(node, [i, j]))
        return neighbors


def temperature(k):
    # T ( k ) = T_0 exp ( - D k / k_max ); T_0 = 1, D = 1
    return 1 * np.exp(-1 * (k / 100))


def random_neighbor(s):
    return max(s.neighbors(), key=lambda x: edge_cover(x))


def common_entries(*dcts):
    for i in set(dcts[0]).intersection(*dcts[1:]):
        yield (i,) + tuple(d[i] for d in dcts)


def edge_cover(s):
    passed = int()
    # for node in common_entries(s.coords, sol):
    #     if node[1] == node[2]:
    #         passed += 1
    for node, coords in s.coords.items():
        for sol_node, sol_coords in sol.items():
            if coords[0] == sol_coords[0] and coords[1] == sol_coords[1]:
                passed += 1
    return passed / 4


def probability(es, es_new, t):
    return np.exp(-(es_new - es) / t)


if __name__ == '__main__':
    s_sol = State(sol)
    s = State()
    print(f"starting with:\n{s.net}")
    print(f"solution:\n{s_sol.net}")
    for i in range(0, 1000):
        t = temperature(i)
        s_new = random_neighbor(s)
        es = edge_cover(s)
        es_new = edge_cover(s_new)
        if es - es_new >= 0:
            s = s_new
        else:
            p = probability(es, es_new, t)
            accept = np.random.choice(np.arange(0, 2), p=[1 - p, p])
            if accept:
                s = s_new
    print(f"found:\n{s.net}")
