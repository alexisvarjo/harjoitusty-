#ifndef SRC_HUFFMAN_H
#define SRC_HUFFMAN_H

#include <bitset>
#include <iomanip>
#include <iostream>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <tuple>
#include <vector>

// Tämä otsake ja huffman.cpp sisältää suoraan
// Huffmanin puun algoritmiin liittyvät funktiot ja luokat.


struct Node {
    int frequency;  // kuinka monta kertaa merkki esiintyy
    char32_t symbol;  // merkki
    Node* left;  // vasen alasolmu
    Node* right;  // oikea alasolmu

    // Konstruktori lehtisolmulle
    Node(char32_t sym, int freq) : frequency(freq), symbol(sym), left(nullptr), right(nullptr) {}

    // Konstruktori sisäsolmulle
    Node(int freq, Node* l, Node* r) : frequency(freq), symbol('\0'), left(l), right(r) {}
};

class HuffmanTree {
    Node* root;  // juuri
    void deleteTree(Node* node);  // poistaa puun

    // luo jokaista merkkiä vastaavan lyhytkoodin (binäärin) huffman-puuhun
    void generateCodes(Node* node, const std::u32string& code,
        std::unordered_map<char32_t, std::u32string>& codes);

    // luo puun uudelleen merkkijonosta, joka sisältää puun informaation binäärinä
    Node* buildFromSerializedTree(const std::u32string& serialized_tree, size_t& index);

 public:
    HuffmanTree();  // konstruktori
    ~HuffmanTree();  // destruktori
    Node* getRoot();  // julkinen funktio juuren osoittimen hakemista varten
    // rakentaa huffman-puun sanakirjan perusteella, jossa on merkkien esiintymismäärät
    void build(const std::unordered_map<char32_t, int>& frequencies);

    // rakentaa puun merkkijonosta, jossa puu on binäärimuodossa, kutsuu yksityistä metodia
    // buildFromSerializedTree
    void rebuildTree(const std::u32string& serialized);
    // luo sanakirjan, jossa jokaiselle merkille on vastaava lyhytkoodi. kutsuu yksityistä metodia
    // generateCodes
    std::unordered_map<char32_t, std::u32string> getCodes(void);
};

// laskee merkkien esiintymismäärät
std::unordered_map<char32_t, int> get_frequencies(const std::u32string& text);

// muuttaa merkkijonon huffman-puun mukaisiksi lyhytkoodeiksi
std::u32string encode(std::u32string const& original_text, HuffmanTree& huffman_tree);

// muuttaa huffman-puun merkkijonoksi, jossa puu on binäärimuodossa
std::u32string serializeTree(Node* root);

// erittelee datan huffman-koodauksen purkamiseksi, palauttaa merkkijonot jotka esittävät
// itse puuta ja koodattua merkkijonoa
std::tuple<std::u32string, size_t, std::string> stripData(const std::string& raw_data);

// purkaa koodatun merkkijonon huffman-puun avulla
std::u32string decode(const std::u32string& text, Node* root);

// ylätason funktio, joka kutsuu kaikkia yllämainittuja funktioita ja luokkia.
// Ottaa parametrina koodattavan merkkijonon ja palauttaa koodatun merkkijonon
// @param string_to_encode pakattava merkkijono
// @return pakattu merkkijono
std::string huffman_encode(const std::u32string& string_to_encode);

// ylätason funktio, joka kutsuu kutsuu muita funktioita purkaakseen pakatusta merkkijonosta
// @param bitstream pakattu merkkijono
// @return purettu merkkijono
std::u32string huffman_decode(const std::string& bitstream);


#endif  // SRC_HUFFMAN_H_
