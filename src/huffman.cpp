#include "huffman.h"
#include "utils.h"
#include <__config>


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
        if (code.empty()) {
            codes[node->symbol] = U"0";
        } else {
        codes[node->symbol] = code;
        }
        return;
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
    // vain yhdenlaisen merkin tapaus
    if (frequencies.size() == 1) {
        char32_t c = frequencies.begin()->first;
        root = new Node(c, frequencies.at(c));
        return;
    }

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

std::unordered_map<char32_t, std::u32string> HuffmanTree::getCodes(void) {
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

std::u32string encode(std::u32string const& original_text, HuffmanTree& huffman_tree) {
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

std::tuple<std::u32string, size_t, std::string> stripData(const std::string& raw_data) {
    // First 8 bytes: tree size.
    std::string treeSizeStr = raw_data.substr(0, 8);
    size_t treeSize = std::stoi(treeSizeStr);

    // Next: the serialized tree.
    std::string treeUtf8 = raw_data.substr(8, treeSize);
    std::u32string tree = utf8ToU32(treeUtf8);

    // Next 8 bytes: number of valid bits in the encoded data.
    std::string bitCountStr = raw_data.substr(8 + treeSize, 8);
    size_t encodedBitCount = std::stoi(bitCountStr);

    // The remainder: the packed encoded data.
    std::string packedEncoded = raw_data.substr(8 + treeSize + 8);

    return {tree, encodedBitCount, packedEncoded};
}

std::u32string decode(const std::u32string& text, Node* root) {
    if (!root->left && !root->right){
        if (text.empty()) {
            return std::u32string(1, root->symbol);
        }

        return std::u32string(text.size(), root->symbol);
    }


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
    if (string_to_encode.empty()) {
        std::cerr << "Error: Empty string" << std::endl;
        return "";
    }

    std::unordered_map<char32_t, int> frequencies = get_frequencies(string_to_encode);
    HuffmanTree tree;
    tree.build(frequencies);
    std::u32string encoded = encode(string_to_encode, tree);

    // Serialize the tree.
    Node* root = tree.getRoot();
    std::u32string serialized_tree = serializeTree(root);
    std::string serializedTreeUtf8 = u32ToUtf8(serialized_tree);
    size_t treeSize = serializedTreeUtf8.size();

    std::string packedEncoded = packBits(encoded);
    size_t encodedBitCount = encoded.size();
    std::ostringstream oss;
    oss << std::setw(8) << std::setfill('0') << treeSize;
    std::string treeHeader = oss.str();
    oss.str("");
    oss.clear();
    oss << std::setw(8) << std::setfill('0') << encodedBitCount;
    std::string bitCountHeader = oss.str();

    std::string final_bitstream = treeHeader + serializedTreeUtf8 + bitCountHeader + packedEncoded;
    return final_bitstream;
}


std::u32string huffman_decode(const std::string& bitstream) {
    if (bitstream.empty()) {
        std::cerr << "Error: Empty string" << std::endl;
        return U"";
    }

    // Extract metadata and data from the bitstream.
    auto [tree_str, encodedBitCount, packedEncoded] = stripData(bitstream);

    // Unpack the encoded bits.
    std::u32string const encodedBits = unpackBits(packedEncoded, encodedBitCount);

    // Rebuild the Huffman tree and decode.
    HuffmanTree tree;
    tree.rebuildTree(tree_str);
    std::u32string decoded = decode(encodedBits, tree.getRoot());
    return decoded;
}
