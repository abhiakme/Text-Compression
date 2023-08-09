// Header Guards
#ifndef huffman_h
#define huffman_h
#include <bits/stdc++.h>
// #include <string>
// #include <vector>
// #include <queue>
// #include <fstream>
using namespace std;

// huffman Tree Node
struct Node
{
    char data;
    unsigned freq;
    string code;
    Node *left, *right;
    Node()
    {
        left = right = NULL;
    }
};

class huffman
{
private:
    vector<Node *> arr;
    fstream inFile, outFile;
    string inFileName, outFileName;
    Node *root;
    class Compare
    {
    public:
        bool operator()(Node *l, Node *r)
        {
            return l->freq > r->freq;
        }
    };

    priority_queue<Node *, vector<Node *>, Compare> minHeap;

    // initializing vector of tree nodes ( which represents characters ascii value with 0 initialial frequency )
    void createArr();

    // traversing the huffman tree to generate corrosponding huffman codes of each character
    void traverse(Node *, string);

    // convert binary string to its decimal value
    int binToDec(string);

    // convert a decimal number to its binary string
    string decToBin(int);

    // reconstructing the huffman tree while decoding the file
    void buildTree(char, string &);

    // creating min heap with the help of frequency of characters in the input file
    void createMinHeap();

    // creating the huffman tree
    void createTree();

    // creating huffman codes for corrosponding character
    void createCodes();

    // saving huffman encoded file
    void createEncodedFile();

    // saving decoded file to obtain the original File
    void createDecodedFile();

    // reading the file (tree information) to reconstruct the huffman tree
    void getTree();

public:
    // constructor
    huffman(string inFileName, string outFileName)
    {
        this->inFileName = inFileName;
        this->outFileName = outFileName;
        createArr();
    }
    void compress();   // compress fun
    void decompress(); // decompress fun
};

/////////////////////////////////////////////////////////////////////////////////////////////////////
void huffman::createArr()
{
    for (int i = 0; i < 128; i++)
    {
        arr.push_back(new Node());
        arr[i]->data = i;
        arr[i]->freq = 0;
        arr[i]->code = "";
    }
}

int huffman::binToDec(string st)
{
    int ans = 0;
    for (auto it : st)
    {
        ans = ans * 2 + it - '0';
    }
    return ans;
}

string huffman::decToBin(int n)
{
    string ans = "", temp = "";
    while (n > 0)
    {
        temp += (n % 2 + '0');
        n /= 2;
    }
    ans.append(8 - temp.length(), '0');
    for (int i = temp.length() - 1; i >= 0; i--)
    {
        ans += temp[i];
    }
    return ans;
}

void huffman::createMinHeap()
{
    char ch;

    inFile.open(inFileName, ios::in);
    inFile.get(ch);
    // count frequency of each characters in input file
    while (!inFile.eof())
    {
        arr[ch]->freq++;
        inFile.get(ch);
    }
    inFile.close();
    // addind Nodes whose ( freq > 0) in input file  to the priority queue (minHeap)
    for (int i = 0; i < 128; i++)
    {
        if (arr[i]->freq > 0)
        {
            minHeap.push(arr[i]);
        }
    }
}

void huffman::createTree()
{
    // creating huffman tree using minHeap
    Node *left, *right;
    priority_queue<Node *, vector<Node *>, Compare> pq(minHeap);
    while (pq.size() != 1)
    {
        left = pq.top();
        pq.pop();

        right = pq.top();
        pq.pop();

        root = new Node();
        root->freq = left->freq + right->freq;

        root->left = left;
        root->right = right;
        pq.push(root);
    }
}

void huffman::createCodes()
{
    traverse(root, "");  // traverse fun for traversing in huffman Tree 
}

void huffman::traverse(Node *root, string str)
{
    if (root->left == NULL && root->right == NULL)
    {
        root->code = str;
        return;
    }
    traverse(root->left, str + '0');
    traverse(root->right, str + '1');
}

void huffman::createEncodedFile()
{
    // creating encoded (.huf) file
    inFile.open(inFileName, ios::in);
    outFile.open(outFileName, ios::out | ios::binary);
    string ans = "", temp = "";
    char it;
    // Saving the meta data (huffman tree)
    ans += (char)minHeap.size();
    priority_queue<Node *, vector<Node *>, Compare> pq(minHeap);
    while (!pq.empty())
    {
        Node *curr = pq.top();
        ans += curr->data;
        // save 16 decimal values representing code of curr->data in temp
        temp.assign(127 - curr->code.length(), '0');
        temp += '1';
        temp += curr->code;
        // save decimal value of every 8-bit binary code in ans
        ans += (char)binToDec(temp.substr(0, 8));
        for (int i = 0; i < 15; i++)
        {
            temp = temp.substr(8);
            ans += (char)binToDec(temp.substr(0, 8));
        }
        pq.pop();
    }
    temp.clear();

    // add codes of every charachter appearing in input file in ans
    inFile.get(it);
    while (!inFile.eof())
    {
        temp += arr[it]->code;
        // add decimal value of every 8-bit binary code in ans
        while (temp.length() > 8)
        {
            ans += (char)binToDec(temp.substr(0, 8));
            temp = temp.substr(8);
        }
        inFile.get(it);
    }

    // if remaining bits  are less than 8, the append 0'temp
    int count = 8 - temp.length();
    if (temp.length() < 8)
    {
        temp.append(count, '0');
    }
    ans += (char)binToDec(temp);
    // append count of appended 0's
    ans += (char)count;

    // cout<<ans<<endl;

    // writing ans string to output file
    outFile.write(ans.c_str(), ans.size());
    inFile.close();
    outFile.close();
}


void huffman::createDecodedFile()
{
    inFile.open(inFileName, ios::in | ios::binary);
    outFile.open(outFileName, ios::out);
    unsigned char size;
    inFile.read(reinterpret_cast<char *>(&size), 1);
    // reading count of appended 0's  in the end of the file (number of bits appended to make final value 8-bit)
    inFile.seekg(-1, ios::end);
    char count0;
    inFile.read(&count0, 1);
    // ignoring the meta data of huffman tree (1 + 17 * size) and reading remaining file
    inFile.seekg(1 + 17 * size, ios::beg);

    vector<unsigned char> text;
    unsigned char text_segment;
    inFile.read(reinterpret_cast<char *>(&text_segment), 1);
    while (!inFile.eof())
    {
        text.push_back(text_segment);
        inFile.read(reinterpret_cast<char *>(&text_segment), 1);
    }

    Node *curr = root;
    string path;
    for (int i = 0; i < text.size() - 1; i++)
    {
        // converting decimal number to its equivalent 8-bit binary code
        path = decToBin(text[i]);
        if (i == text.size() - 2)
        {
            path = path.substr(0, 8 - count0);
        }
        // traversing huffman tree and appending resultant data to the file
        for (int j = 0; j < path.size(); j++)
        {
            if (path[j] == '0')
            {
                curr = curr->left;
            }
            else
            {
                curr = curr->right;
            }

            if (curr->left == NULL && curr->right == NULL)
            {
                outFile.put(curr->data);
                curr = root;
            }
        }
    }
    inFile.close();
    outFile.close();
}

void huffman::getTree()
{
    inFile.open(inFileName, ios::in | ios::binary);
    // reading size of minHeap
    unsigned char size;
    inFile.read(reinterpret_cast<char *>(&size), 1);
    root = new Node();
    // next size * (1 + 16) characters contain (char)data and (string)code[in decimal]
    for (int i = 0; i < size; i++)
    {
        char ch;
        unsigned char chCode[16];
        inFile.read(&ch, 1);
        inFile.read(reinterpret_cast<char *>(chCode), 16);
        // converting decimal characters into their binary equivalent to obtain code
        string chCodeStr = "";
        for (int i = 0; i < 16; i++)
        {
            chCodeStr += decToBin(chCode[i]);
        }
        // removing padding by ignoring first (127 - curr->code.length()) '0's and next '1' character
        int j = 0;
        while (chCodeStr[j] == '0')
        {
            j++;
        }
        chCodeStr = chCodeStr.substr(j + 1);
        // Adding node with ch data and hCodeStr string to the huffman tree
        buildTree(ch, chCodeStr);
    }
    inFile.close();
}

void huffman::buildTree(char ch, string &chCode)
{
    Node *curr = root;
    for (int i = 0; i < chCode.length(); i++)
    {
        if (chCode[i] == '0')
        {
            if (curr->left == NULL)
            {
                curr->left = new Node();
            }
            curr = curr->left;
        }
        else if (chCode[i] == '1')
        {
            if (curr->right == NULL)
            {
                curr->right = new Node();
            }
            curr = curr->right;
        }
    }
    curr->data = ch;
}

// compress fun
void huffman::compress()
{
    createMinHeap();
    createTree();
    createCodes();
    createEncodedFile();
}

// decompress fun
void huffman::decompress()
{
    getTree();
    createDecodedFile();
}

#endif