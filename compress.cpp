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

void assignCodes(unordered_map<char, string>& codes, HuffmanNode* node, string code) {
    if (node == NULL) return;

    if (node->data != '\0') codes[node->data] = code;
    else {
        assignCodes(codes, node->left, code + '0');
        assignCodes(codes, node->right, code + '1');
    }
}

void huffmanEncoding(unordered_map<char, int>& freq, unordered_map<char, string>& codes) {
    priority_queue<pair<int, HuffmanNode*>, vector<pair<int, HuffmanNode*>>, greater<pair<int, HuffmanNode*>>> minHeap;
    
    for (auto i: freq) {
        HuffmanNode* thisNode = new HuffmanNode(i.first);
        minHeap.push(make_pair(i.second, thisNode));
    }

    while (minHeap.size() > 1) {
        auto f1 = minHeap.top(); minHeap.pop();
        auto f2 = minHeap.top(); minHeap.pop();

        HuffmanNode* thisNode = new HuffmanNode('\0');
        thisNode->left = f1.second;
        thisNode->right = f2.second;

        minHeap.push(make_pair(f1.first + f2.first, thisNode));
    }
    
    HuffmanNode* head = minHeap.top().second;
    assignCodes(codes, head, "");   
}

char nextCh(string& binary, int start) {
    int val = 0, multiplier = 1;
    for (int i = start + 7; i >= start; i--) {
        if (binary[i] == '1') val += multiplier;
        multiplier *= 2;
    }
    return val;
}

int main() {
    
    string inName, outName, outCName;
    cout << "enter name of file to be compressed (.txt): ";
    getline(cin, inName);
    
    fstream file;
    file.open(inName, ios::in);
    
    if (file) {
        unordered_map<char, int> freq;
        char ch;
        while (file >> noskipws >> ch) {
            if (freq.count(ch) == 0) freq[ch] = 1;
            else freq[ch]++;
        }

        unordered_map<char, string> codes;
        huffmanEncoding(freq, codes);

        file.clear();
        file.seekg(0);

        cout << "enter name for compressed file (.bin): ";
        getline(cin, outName);

        fstream out;
        out.open(outName, ios::out | ios::binary);
        
        if (out) {
            string binary;
            while (file >> noskipws >> ch) {
                binary += ("0" + codes[ch]);
            }
    
            int buffer = 8 - (binary.length() % 8);
            if (buffer == 8) buffer = 0;
            for (int i = 0; i < buffer; i++) binary += "1";
            
            string toWrite = "";
            for (int pos = 0; pos < binary.length(); pos += 8) {
                toWrite += nextCh(binary, pos);
            }
            out.write(toWrite.c_str(), toWrite.length());
            
            out.close();
        }
        else cout << "can't write compressed file" << endl;

        cout << "enter name for codes file (.txt): ";
        getline(cin, outCName);

        fstream outCodes;
        outCodes.open(outCName, ios::out);
        if (outCodes) {
            for (auto x: codes) {
                outCodes << (int) x.first << " " << x.second << " \n"; 
            }
            outCodes.close();
        }
        else cout << "can't write codes file" << endl;
        
        file.close();
    }
    else cout << "can't read file to be compressed" << endl;

    return 0;
}
