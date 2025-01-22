#include "huffman.h"
#include "utils.h"

class HuffmanTree {
    Node* root;

    void deleteTree(Node* node) {
        if (node == nullptr || !node) {
            return;
        }
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    void generateCodes(Node* node, const std::u32string& code,
        std::unordered_map<char32_t, std::u32string>& codes){
        if (!node) return;

        if (!node->left && !node->right){
            codes[node->symbol] = code;
        }

        generateCodes(node->left, code + charToU32String("0"), codes);
        generateCodes(node->right, code + charToU32String("1"), codes);
    }
public:
    HuffmanTree() : root(nullptr) {}

    ~HuffmanTree() {
        deleteTree(root);
    }

    Node* getRoot(){
        return root;
    }

    void build(const std::unordered_map<char32_t, int>& frequencies){

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

    std::unordered_map<char32_t, std::u32string> getCodes(){
        std::unordered_map<char32_t, std::u32string> codes;
        generateCodes(root, charToU32String(""), codes);
        return codes;
    }

};

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


int main() {
    const std::u32string test = U"ÄÄÄBBC";
    std::unordered_map<char32_t, int> frequencies = get_frequencies(test);
    std::cout << "Frequencies:\n";
    for (auto pair : frequencies) {
        std::cout << char32ToUtf8(pair.first) << ": " << pair.second << "\n";
    }

    HuffmanTree tree;
    tree.build(frequencies);

    std::unordered_map<char32_t, std::u32string> codes = tree.getCodes();
    for (auto pair : codes) {
        std::cout << char32ToUtf8(pair.first) << ": " << u32ToUtf8(pair.second) << "\n";
    }

    std::u32string encoded = encode(test, tree);


    Node* root = tree.getRoot();
    std::u32string serialized_tree = serializeTree(root);
    size_t metaDataSize = serialized_tree.size();

    std::cout << "metaDataSize: " << metaDataSize << "\n";
    std::cout << "serialized_tree: " << u32ToUtf8(serialized_tree) << "\n";
    std::cout << "Encoded bitstream " << u32ToUtf8(encoded) << "\n";
    return 0;
}
