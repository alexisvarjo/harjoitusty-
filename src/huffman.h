#ifndef huffman_h
#define huffman_h


#include <iostream>
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

struct Node {
    int frequency;
    char32_t symbol;
    Node* left;
    Node* right;

    //Konstruktori lehtisolmulle
    Node(char32_t sym, int freq) : symbol(sym), frequency(freq), left(nullptr), right(nullptr) {}

    //Konstruktori sisäsolmulle
    Node(int freq, Node* l, Node* r) : symbol('\0'), frequency(freq), left(l), right(r) {}
};

std::unordered_map<char32_t, int> get_frequencies(const std::u32string& text);


#endif
