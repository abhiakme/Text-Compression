#include <iostream>
#include "huffman.h"
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "compressed file not find";
        exit(1);
    }
    huffman obj(argv[1], argv[2]);
    obj.decompress();
    cout << "decompression done" << endl;
    return 0;
}