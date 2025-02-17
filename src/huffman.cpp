#include "huffman.h"
#include "utils.h"
#include <__config>


// Huffmantree-luokan toteutus ja metodit

void HuffmanTree::deleteTree(Node* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void HuffmanTree::generateCodes(Node* node, std::string& code,
    std::unordered_map<char, std::string>& codes) {
    if (!node) return;

    // lehti
    if (!node->left && !node->right) {
        codes[node->symbol] = code.empty() ? "0" : code;
        return;
    }
    code.push_back('0');
    generateCodes(node->left, code, codes);
    code.pop_back();

    code.push_back('1');
    generateCodes(node->right, code, codes);
    code.pop_back();
}

Node* HuffmanTree::buildFromSerializedTree(const std::string& serialized_tree, size_t& index) {
    if (index >= serialized_tree.size()) return nullptr;

    char marker = serialized_tree[index++];

    if (marker == '1') {
        // lehti
        return new Node(serialized_tree[index++], 0);
    } else if (marker == '0') {
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

void HuffmanTree::build(const std::unordered_map<char, int>& frequencies) {
    // vain yhdenlaisen merkin tapaus
    if (frequencies.size() == 1) {
        char c = frequencies.begin()->first;
        root = new Node(c, frequencies.at(c));
        return;
    }

    auto compare = [](Node* a, Node* b) { return a->frequency > b->frequency; };
    // min-heap
    std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> pq(compare);

    // uusi solmu jokaiselle merkille
    for (const std::pair<const char, int>& pair : frequencies) {
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

std::unordered_map<char, std::string> HuffmanTree::getCodes(void) {
    std::unordered_map<char, std::string> codes;
    generateCodes(root, "", codes);
    return codes;
}

void HuffmanTree::rebuildTree(const std::string& serialized) {
    size_t index = 0;
    root = buildFromSerializedTree(serialized, index);
}

// Muut funktiot
std::unordered_map<char, int> get_frequencies(const std::string& text) {
    std::unordered_map<char, int> frequencies;
    for (char c : text) {
        frequencies[c]++;
    }
    return frequencies;
}

std::string encode(std::string const& original_text, HuffmanTree& huffman_tree) {
    std::string encoded_text = "";
    encoded_text.reserve(original_text.size() * 10); // Reserve space for worst case scenario
    std::unordered_map<char, std::string> codes = huffman_tree.getCodes();
    for (char c : original_text) {
        encoded_text += codes[c];
    }
    return encoded_text;
}

std::string serializeTree(Node* root) {
    if (!root) return "";
    // Lehti:
    if (!root->left && !root->right) {
        return "1" + std::string(1, root->symbol);
    }
    // sisäsolmu:
    return "0" + serializeTree(root->left) + serializeTree(root->right);
}

std::tuple<std::string, size_t, std::string> stripData(const std::string& raw_data) {
    // First 8 bytes: tree size.
    std::string treeSizeStr = raw_data.substr(0, 8);
    size_t treeSize = std::stoi(treeSizeStr);

    // Next: the serialized tree.
    std::string tree = raw_data.substr(8, treeSize);

    // Next 8 bytes: number of valid bits in the encoded data.
    std::string bitCountStr = raw_data.substr(8 + treeSize, 8);
    size_t encodedBitCount = std::stoi(bitCountStr);

    // The remainder: the packed encoded data.
    std::string packedEncoded = raw_data.substr(8 + treeSize + 8);

    return {tree, encodedBitCount, packedEncoded};
}

std::string decode(const std::string& text, Node* root) {
    if (!root->left && !root->right){
        if (text.empty()) {
            return std::string(1, root->symbol);
        }

        return std::string(text.size(), root->symbol);
    }


    std::string decoded_text = "";
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

std::string huffman_encode(const std::string& string_to_encode) {
    if (string_to_encode.empty()) {
        std::cerr << "Error: Empty string" << std::endl;
        return "";
    }

    std::unordered_map<char, int> frequencies = get_frequencies(string_to_encode);
    HuffmanTree tree;
    tree.build(frequencies);
    std::string encoded = encode(string_to_encode, tree);

    // Serialize the tree.
    Node* root = tree.getRoot();
    std::string serialized_tree = serializeTree(root);
    size_t treeSize = serialized_tree.size();

    std::string packedEncoded = packBits(encoded);
    size_t encodedBitCount = encoded.size();
    std::ostringstream oss;
    oss << std::setw(8) << std::setfill('0') << treeSize;
    std::string treeHeader = oss.str();
    oss.str("");
    oss.clear();
    oss << std::setw(8) << std::setfill('0') << encodedBitCount;
    std::string bitCountHeader = oss.str();

    std::string final_bitstream = treeHeader + serialized_tree + bitCountHeader + packedEncoded;
    return final_bitstream;
}


std::string huffman_decode(const std::string& bitstream) {
    if (bitstream.empty()) {
        std::cerr << "Error: Empty string" << std::endl;
        return "";
    }

    // Extract metadata and data from the bitstream.
    auto [tree_str, encodedBitCount, packedEncoded] = stripData(bitstream);

    // Unpack the encoded bits.
    std::string const encodedBits = unpackBits(packedEncoded, encodedBitCount);

    // Rebuild the Huffman tree and decode.
    HuffmanTree tree;
    tree.rebuildTree(tree_str);
    std::string decoded = decode(encodedBits, tree.getRoot());
    return decoded;
}
