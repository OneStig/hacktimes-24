//
// Created by Steven He on 7/17/24.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <string>
#include <array>
#include <functional>

using namespace std;

class TrieNode {
public:
    array<TrieNode*, 26> children;
    string cur;
    bool is_end_of_word;

    TrieNode(const string& prefix = "") : cur(prefix), is_end_of_word(false) {
        children.fill(nullptr);
    }
};

class Trie {
public:
    TrieNode* root;

    Trie() {
        root = new TrieNode();
    }

    void insert(const string& word) {
        TrieNode* node = root;
        for (char charx : word) {
            int cx = charx - 'A';
            if (node->children[cx] == nullptr) {
                node->children[cx] = new TrieNode(node->cur + charx);
            }
            node = node->children[cx];
        }
        node->is_end_of_word = true;
    }
};

inline Trie* build_trie(const unordered_set<string>& dictionary) {
    Trie* trie = new Trie();
    for (const string& word : dictionary) {
        trie->insert(word);
    }
    return trie;
}

const array<string, 26> ALPHABET = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};

const array<vector<int>, 19> GRAPH = {
    vector<int>{1, 3, 4}, vector<int>{0, 2, 4, 5}, vector<int>{1, 5, 6},
    vector<int>{0, 4, 7, 8}, vector<int>{0, 1, 3, 5, 8, 9}, vector<int>{1, 2, 4, 6, 9, 10},
    vector<int>{2, 5, 10, 11}, vector<int>{3, 8, 12}, vector<int>{3, 4, 7, 9, 12, 13},
    vector<int>{4, 5, 8, 10, 13, 14}, vector<int>{5, 6, 9, 11, 14, 15}, vector<int>{6, 10, 15},
    vector<int>{7, 8, 13, 16}, vector<int>{8, 9, 12, 14, 16, 17}, vector<int>{9, 10, 13, 15, 17, 18},
    vector<int>{10, 11, 14, 18}, vector<int>{12, 13, 17}, vector<int>{13, 14, 16, 18}, vector<int>{14, 15, 17}
};

class Scoring {
private:
    unordered_set<string> words;
    vector<vector<string>> words_by_len;
    vector<double> weight_list;
    Trie* trie_root;

public:
    Scoring() : words_by_len(20) {
        ifstream file("enable1.txt");
        string word;
        while (getline(file, word)) {
            transform(word.begin(), word.end(), word.begin(), ::toupper);
            if (word.length() <= 19) {
                words.insert(word);
                words_by_len[word.length()].push_back(word);
            }
        }
        file.close();
        trie_root = build_trie(words);
    }

    int score(const string& node_labels) {
        unordered_set<string> valid_words;
        vector<bool> visited(GRAPH.size(), false);

        function<void(int, TrieNode*)> dfs = [&](int node, TrieNode* cur_trie) {
            if (cur_trie->is_end_of_word) {
                valid_words.insert(cur_trie->cur);
            }
            visited[node] = true;
            for (int nb : GRAPH[node]) {
                char nbc = node_labels[nb];
                if (!visited[nb] && cur_trie->children[nbc - 'A'] != nullptr) {
                    dfs(nb, cur_trie->children[nbc - 'A']);
                }
            }
            visited[node] = false;
        };

        for (int i = 0; i < 19; ++i) {
            if (trie_root->root->children[node_labels[i] - 'A']) {
                dfs(i, trie_root->root->children[node_labels[i] - 'A']);
            }
        }

        array<int, 20> count = {0};
        for (const string& found : valid_words) {
            count[found.length()]++;
        }

        int total_score = 0;
        for (int i = 1; i < 20; ++i) {
            total_score += i * count[i];
        }

        return total_score;
    }
};
