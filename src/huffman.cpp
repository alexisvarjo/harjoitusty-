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
    std::unordered_map<char32_t, std::u32string>& codes){
    if (!node) return;

    if (!node->left && !node->right){
        codes[node->symbol] = code;
    }

    generateCodes(node->left, code + charToU32String("0"), codes);
    generateCodes(node->right, code + charToU32String("1"), codes);
}

Node* HuffmanTree::buildFromSerializedTree(const std::u32string& serialized_tree, size_t& index){
    if (index >= serialized_tree.size()) return nullptr;

    char32_t marker = serialized_tree[index++];

    if (marker == U'1'){
        //lehti
        return new Node(serialized_tree[index++], 0);
    } else if (marker == U'0'){
        //sisäsolmu
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

Node* HuffmanTree::getRoot(){
    return root;
}

void HuffmanTree::build(const std::unordered_map<char32_t, int>& frequencies){

    auto compare = [](Node* a, Node* b) { return a->frequency > b->frequency; };
    std::priority_queue<Node*, std::vector<Node*>, decltype(compare)> pq(compare);

    for (const std::pair<const char32_t, int>& pair : frequencies){
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() > 1){
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();

        Node* parent = new Node(left->frequency + right->frequency, left, right);
        pq.push(parent);
    }

    root = pq.top();
}

std::unordered_map<char32_t, std::u32string> HuffmanTree::getCodes(){
    std::unordered_map<char32_t, std::u32string> codes;
    generateCodes(root, charToU32String(""), codes);
    return codes;
}

void HuffmanTree::rebuildTree(const std::u32string& serialized) {
    size_t index = 0;
    root = buildFromSerializedTree(serialized, index);
}

// Muut funktiot

std::unordered_map<char32_t, int> get_frequencies(const std::u32string& text){
    std::unordered_map<char32_t, int> frequencies;
    for (char32_t c : text){
        frequencies[c]++;
    }
    return frequencies;
};

std::u32string encode(const std::u32string& original_text, HuffmanTree& huffman_tree){
    std::u32string encoded_text = charToU32String("");
    std::unordered_map<char32_t, std::u32string> codes = huffman_tree.getCodes();
    for (char32_t c : original_text){
        encoded_text += codes[c];
    }
    return encoded_text;
}

std::u32string serializeTree(Node* root){
    if (!root) return charToU32String("");

    //Lehti:
    if (!root->left && !root->right){
        return charToU32String("1") + std::u32string(1, root->symbol);
    }

    //sisäsolmu:
    return charToU32String("0") + serializeTree(root->left) + serializeTree(root->right);
}

std::pair<std::u32string, std::string> stripData(const std::string& raw_data){

    std::string metaData = raw_data.substr(0, 8);
    int metaDataSize = std::stoi(metaData);

    std::string treeUtf8 = raw_data.substr(8, metaDataSize);

    std::u32string tree = utf8ToU32(treeUtf8);

    std::string text = raw_data.substr(8 + metaDataSize);
    return std::make_pair(tree, text);
}

std::u32string decode(const std::string& text, Node* root){
    std::u32string decoded_text = charToU32String("");
    Node* current = root;

    for (char c : text){
        if (c == '0'){
            current = current->left;
        } else{
            current = current->right;
        }
        if (!current->left && !current->right){
            decoded_text += current->symbol;
            current = root;
        }
    }
    return decoded_text;
}

std::string huffman_encode(const std::u32string& string_to_encode){
    std::unordered_map<char32_t, int> frequencies = get_frequencies(string_to_encode);
    HuffmanTree tree;
    tree.build(frequencies);

    std::unordered_map<char32_t, std::u32string> code = tree.getCodes();

    std::u32string encoded = encode(string_to_encode, tree);
    Node* root = tree.getRoot();
    std::u32string serialized_tree = serializeTree(root);
    std::string serializedTreeUtf8 = u32ToUtf8(serialized_tree);
    size_t metaDataSize = serializedTreeUtf8.size();
    std::ostringstream oss;
    oss << std::setw(8) << std::setfill('0') << metaDataSize;
    std::string sizeHeader = oss.str();
    std::string final_bitstream = sizeHeader + serializedTreeUtf8 + u32ToUtf8(encoded);

    return final_bitstream;
}

std::u32string huffman_decode(const std::string& bitstream){
    std::pair<std::u32string, std::string> stripped = stripData(bitstream);

    std::u32string tree_str = stripped.first;
    std::string text = stripped.second;

    HuffmanTree tree = HuffmanTree();
    tree.rebuildTree(tree_str);

    std::u32string decoded = decode(text, tree.getRoot());
    return decoded;
}
