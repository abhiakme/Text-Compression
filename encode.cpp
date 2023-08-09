#include <iostream>
#include "huffman.h"
using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "input file not found";
        exit(1);
    }
    huffman obj(argv[1], argv[2]);
    obj.compress();
    cout << "compression done" << endl;
    return 0;
}