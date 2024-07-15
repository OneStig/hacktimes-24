import string
import functools

from typing import List, Optional, Iterable

from rich.console import Console
from rich.table import Table
# import asciichartpy

atoi = {chr(i + ord('A')): i for i in range(26)}

class TrieNode:
    def __init__(self, prefix = ''):
        self.children: List[Optional['TrieNode']] = [None] * 26
        self.cur: str = prefix
        self.is_end_of_word = False

class Trie:
    def __init__(self):
        self.root = TrieNode()

    def insert(self, word):
        node = self.root
        for char in word:
            cx: int = atoi[char]
            if node.children[cx] is None:
                node.children[cx] = TrieNode(node.cur + char)
            node = node.children[cx]
        node.is_end_of_word = True


def _build_trie(dictionary: Iterable[str]) -> Trie:
    trie = Trie()
    for word in dictionary:
        trie.insert(word)
    return trie

GRAPH = [
    [1, 3, 4],
    [0, 2, 4, 5],
    [1, 5, 6],
    [0, 4, 7, 8],
    [0, 1, 3, 5, 8, 9],
    [1, 2, 4, 6, 9, 10],
    [2, 5, 10, 11],
    [3, 8, 12],
    [3, 4, 7, 9, 12, 13],
    [4, 5, 8, 10, 13, 14],
    [5, 6, 9, 11, 14, 15],
    [6, 10, 15],
    [7, 8, 13, 16],
    [8, 9, 12, 14, 16, 17],
    [9, 10, 13, 15, 17, 18],
    [10, 11, 14, 18],
    [12, 13, 17],
    [13, 14, 16, 18],
    [14, 15, 17]
]

class Scoring:
    def __init__(self):
        self.words = set()
        self.words_by_len = [[] for _ in range(20)]

        letter_frequencies = [0] * 26
        total_len = 0

        with open('enable1.txt') as file:
            for word in file:
                word = word.strip().upper()

                if len(word) <= 19:
                    self.words.add(word)
                    self.words_by_len[len(word)].append(word)
                    total_len += len(word)

                    for letter in word:
                        letter_frequencies[atoi[letter]] += 1

        letters: List[(str, float)] = []

        for letter in string.ascii_uppercase:
            letters.append((letter, letter_frequencies[atoi[letter]] / total_len * 100))

        self.letter_list, self.weight_list = zip(*letters)
        self.trie_root = _build_trie(self.words)

        console = Console()

        table = Table(title = 'Words Per Length')
        table.add_column('Metric', justify = 'left')
        table.add_column('Value')

        graph_data = []
        for length in range(1, 20):
            graph_data.append(len(self.words_by_len[length]))
            table.add_row(f'{length}', f'{graph_data[-1]}')

        console.print(f'Total word count: {len(self.words)}')
        console.print(table)

    @functools.cache
    def score(self, node_labels: str):
        valid_words = set()
        visited = [False] * len(GRAPH)

        def dfs(node, cur_trie: TrieNode):
            if cur_trie.is_end_of_word:
                valid_words.add(cur_trie.cur)

            visited[node] = True

            for nb in GRAPH[node]:
                nbc = node_labels[nb]
                if not visited[nb] and cur_trie.children[atoi[nbc]] is not None:
                    dfs(nb, cur_trie.children[atoi[nbc]])

            visited[node] = False

        for i in range(19):
            dfs(i, self.trie_root.root.children[atoi[node_labels[i]]])

        count = [0] * 20
        for found in valid_words:
            count[len(found)] += 1

        total_score = 0

        for i in range(1, 20):
            total_score += i * count[i]

        return total_score



