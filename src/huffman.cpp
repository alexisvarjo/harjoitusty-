#include "huffman.h"
#include "utils.h"


// Huffmantree-luokan toteutus ja metodit

void HuffmanTree::deleteTree(Node* node) {
    if (node == nullptr || !node) {
        return;
    }
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void HuffmanTree::generateCodes(Node* node, const std::u32string& code,
    std::unordered_map<char32_t, std::u32string>& codes) {
    if (!node) return;

    // lehti
    if (!node->left && !node->right) {
        codes[node->symbol] = code;
    }

    // käy rekursiivisesti oikeassa ja vasemmassa
    // jonossa, ja lisää koodiin 0 tai 1
    generateCodes(node->left, code + charToU32String("0"), codes);
    generateCodes(node->right, code + charToU32String("1"), codes);
}

Node* HuffmanTree::buildFromSerializedTree(const std::u32string& serialized_tree, size_t& index) {
    if (index >= serialized_tree.size()) return nullptr;

    char32_t marker = serialized_tree[index++];

    if (marker == U'1') {
        // lehti
        return new Node(serialized_tree[index++], 0);
    } else if (marker == U'0') {
        // sisäsolmu
        Node* left = buildFromSerializedTree(serialized_tree, index);
        Node* right = buildFromSerializedTree(serialized_tree, index);
        return new Node(0, left, right);
    }

    return nullptr;
}

HuffmanTree::HuffmanTree() : root(nullptr) {}

HuffmanTree::~HuffmanTree() {
    deleteTree(root);
}

Node* HuffmanTree::getRoot() {
    return root;
}

void HuffmanTree::build(const std::unordered_map<char32_t, int>& frequencies) {
    auto compare = [](Node* a, Node* b) { return a->frequency > b->frequency; };
    // min-heap
    std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> pq(compare);

    // uusi solmu jokaiselle merkille
    for (const std::pair<const char32_t, int>& pair : frequencies) {
        pq.push(new Node(pair.first, pair.second));
    }

    // tässä kohtaa rakennetaan itse puu
    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();

        Node* parent = new Node(left->frequency + right->frequency, left, right);
        pq.push(parent);
    }

    root = pq.top();
}

std::unordered_map<char32_t, std::u32string> HuffmanTree::getCodes() {
    std::unordered_map<char32_t, std::u32string> codes;
    generateCodes(root, charToU32String(""), codes);
    return codes;
}

void HuffmanTree::rebuildTree(const std::u32string& serialized) {
    size_t index = 0;
    root = buildFromSerializedTree(serialized, index);
}

// Muut funktiot
std::unordered_map<char32_t, int> get_frequencies(const std::u32string& text) {
    std::unordered_map<char32_t, int> frequencies;
    for (char32_t c : text) {
        frequencies[c]++;
    }
    return frequencies;
}

std::u32string encode(const std::u32string& original_text, HuffmanTree& huffman_tree) {
    std::u32string encoded_text = charToU32String("");
    std::unordered_map<char32_t, std::u32string> codes = huffman_tree.getCodes();
    for (char32_t c : original_text) {
        encoded_text += codes[c];
    }
    return encoded_text;
}

std::u32string serializeTree(Node* root) {
    if (!root) return charToU32String("");
    // Lehti:
    if (!root->left && !root->right) {
        return charToU32String("1") + std::u32string(1, root->symbol);
    }
    // sisäsolmu:
    return charToU32String("0") + serializeTree(root->left) + serializeTree(root->right);
}

std::pair<std::u32string, std::string> stripData(const std::string& raw_data) {
    // ensimmäiset 8 merkkiä sisältää puun bittien lukumäärän
    std::string metaData = raw_data.substr(0, 8);
    int metaDataSize = std::stoi(metaData);

    // puun bittijono
    std::string treeUtf8 = raw_data.substr(8, metaDataSize);

    // puun bittijono u32string-muotoon
    std::u32string tree = utf8ToU32(treeUtf8);

    // koodattu teksti on kaikki loput bitit
    std::string text = raw_data.substr(8 + metaDataSize);
    return std::make_pair(tree, text);
}

std::u32string decode(const std::string& text, Node* root) {
    std::u32string decoded_text = charToU32String("");
    Node* current = root;

    for (char c : text) {
        // puun bittiesityksessä 0 tarkoittaa vasempaan haaraan siirtymistä
        if (c == '0') {
            current = current->left;
        } else {  // 1 vastaa oikeaan haaraan siirtymistä
            current = current->right;
        }
        // jos solmulla ei ole lapsia, sillä on symbol-muuttujassa sen merkki
        if (!current->left && !current->right) {
            // ja tähän asti kerätyt 0 1 -bitit muodostavat sen koodin
            decoded_text += current->symbol;
            current = root;
        }
    }
    return decoded_text;
}

std::string huffman_encode(const std::u32string& string_to_encode) {
    // lasketaan merkkien esiintymistiheydet
    std::unordered_map<char32_t, int> frequencies = get_frequencies(string_to_encode);
    HuffmanTree tree;
    // rakennetaan puu merkkien esiintymisten perusteella
    tree.build(frequencies);

    // std::unordered_map<char32_t, std::u32string> code = tree.getCodes();
    // merkkien koodit, jos haluaa tarkastella mitkä lyhytkoodit huffmanpuu on antanut merkeille

    // pakataan teksti merkkien perusteella
    std::u32string encoded = encode(string_to_encode, tree);

    Node* root = tree.getRoot();

    // haetaan juuren perusteella bittiesitys
    std::u32string serialized_tree = serializeTree(root);

    // bittiesitys utf8-muotoon
    std::string serializedTreeUtf8 = u32ToUtf8(serialized_tree);

    // haetaan puun bittiesityksen koko
    size_t metaDataSize = serializedTreeUtf8.size();
    std::ostringstream oss;

    // muutetaan koko 8 merkkiä pitkäksi merkkijonoksi, sillä metaDataSize on aina 8 merkkiä pitkä
    oss << std::setw(8) << std::setfill('0') << metaDataSize;

    // muutetaan tietotyyppi merkkijonoksi
    std::string sizeHeader = oss.str();

    // muodostetaan lopullinen pakattu bittijono
    std::string final_bitstream = sizeHeader + serializedTreeUtf8 + u32ToUtf8(encoded);

    return final_bitstream;
}

std::u32string huffman_decode(const std::string& bitstream) {
    // erotellaan pakatusta bittijonosta kolme asiaa
    // ensimmäiset kahdeksan tavua sisältää puun bittien lukumäärän, sen jälkeen voidaan lukea puun bittiesitys ja loput ovat koodattu teksti
    // funktio palauttaa vain puun bittiesityksen ja koodatun tekstin, koska puun bittiesitys on aina 8 merkkiä pitkä
    std::pair<std::u32string, std::string> stripped = stripData(bitstream);


    std::u32string tree_str = stripped.first;  // puun bittiesitys
    std::string text = stripped.second;  // koodattu teksti

    HuffmanTree tree = HuffmanTree();  // init huffmantree
    tree.rebuildTree(tree_str);  // rakennetaan puu uudelleen bittiesityksen perusteella

    std::u32string decoded = decode(text, tree.getRoot());  // puretaan koodattu teksti puun avulla.

    return decoded;
}
