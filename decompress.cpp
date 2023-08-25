#include <bits/stdc++.h>
using namespace std;

class HuffmanNode {
    public:
        char data;
        HuffmanNode* left;
        HuffmanNode* right;

        HuffmanNode(char data) {
            this->data = data;
            this->left = NULL;
            this->right = NULL;
        }
};

void addHuffmanNode(HuffmanNode* head, string code, char ch) {
    HuffmanNode* tracker = head;
    for (char bit: code) {
        if (bit == '0') {
            if (!tracker->left) tracker->left = new HuffmanNode('\0');
            tracker = tracker->left;
        }
        else if (bit == '1') {
            if (!tracker->right) tracker->right = new HuffmanNode('\0');
            tracker = tracker->right;
        }
    }
    tracker->data = ch;
}

void buildHuffmanTree(map<string, char>& codes, HuffmanNode* &head) {
    if (!head) head = new HuffmanNode('\0');

    for (auto x: codes) {
        addHuffmanNode(head, x.first, x.second);
    }
}

char parseNextChar(string& binary, int& pos, HuffmanNode* head) {
    if (binary[pos] == '1') return '\0';
    else pos++;

    HuffmanNode* tracker = head;
    while (tracker && tracker->data == '\0') {
        if (binary[pos] == '0') tracker = tracker->left;
        else if (binary[pos] == '1') tracker = tracker->right;
        pos++;
    }

    if (tracker) return tracker->data;
    else return '\0';
}

int main() {
    
    string inName, inCName, outName;
    cout << "enter name of file to be decompressed (.bin): ";
    getline(cin, inName);

    fstream file;
    file.open(inName, ios::in | ios::binary);

    if (file) {
        string binary = "";
        char val;
        while (file >> noskipws >> val) {
            binary += bitset<8>((int) val).to_string();
        }

        cout << "enter name of codes file to be used (.txt): ";
        getline(cin, inCName);

        fstream codesIn;
        codesIn.open(inCName, ios::in);

        if (codesIn) {
            int ch;
            string code;
            map<string, char> codes;

            while (!codesIn.eof()) {
                codesIn >> ch;
                codesIn >> code;
                codes.insert({code, (char) ch});
            }

            HuffmanNode* head = NULL;
            buildHuffmanTree(codes, head);

            cout << "enter name for decompressed file (.txt): ";
            getline(cin, outName);

            fstream out;
            out.open(outName, ios::out);

            if (out) {
                int pos = 0;
                char nextChar;

                while (pos < binary.length() && (nextChar = parseNextChar(binary, pos, head)) != '\0') {
                    out << nextChar;
                }
                out.close();
            }
            else cout << "can't write decompressed file" << endl;

            codesIn.close();
        }
        else cout << "can't read codes file" << endl;

        file.close();
    }
    else cout << "can't read file to be decompressed" << endl;

    return 0;
}
