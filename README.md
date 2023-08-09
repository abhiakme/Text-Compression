<h1 align="center">Huffman Coding</h1>

C++ Project based on Huffman's coding of a lossless, bottom-up compression algorithm. It can compress any Txt file and decompress any compressed file which is compressed using Huffman coding.

## How does it work?

- \_Huffman Coding Compression:

  1. Character Frequency Calculation:

  - Read the input text file and calculate the frequency of each character present in the file.

  2. Priority Queue (Min Heap) Creation:

  - Create a priority queue (min-heap) using the calculated character frequencies. Each character is initially treated as a single-node tree with its frequency.

  3. Huffman Tree Construction:

  - While there is more than one node in the priority queue, repeatedly remove the two nodes with the lowest frequencies and create a new internal node with their sum frequency. Insert the new node back into the priority queue.

  4. Huffman Coding Mapping Generation:

  - Traverse the constructed Huffman tree to generate a mapping of characters to their corresponding Huffman codes. Assign '0' for left branches and '1' for right branches during the traversal.

  5. Text Encoding:

  - Read the input text file again and replace each character with its corresponding Huffman code based on the mapping generated in the previous step.

  6. Binary Data Storage:
  \*Pack the variable-length Huffman codes into bytes and write the encoded binary data to an output file.

- Huffman Coding Decompression:

  1. Huffman Tree Reconstruction:
  \*Read the compressed binary data from the compressed file and reconstruct the Huffman tree by interpreting '0' as a move to the left child and '1' as a move to the right child.

  2. Text Decoding:
  \*Traverse the reconstructed Huffman tree using the binary data from the compressed file. Whenever a leaf node is reached, write the corresponding character to the output file and start traversing the tree from the root again.

### Compress:

`Compress` ('encode') is a 2-pass program. Means`Compressor` reads input files twice.

In the first time, the program counts the frequency of each character and creates a Huffman tree on the basis of character frequency.

In the second time, it creates a compressed file { metadata of Huffman tree + corresponding code for each character } for decompression purposes

### Decompress:

`Decompress` ('decode') is a 1-pass program.

`Decompress` first reads the metadata of the Huffman tree and gets the Huffman tree back from it.

After that, it starts decoding the rest of the file using created Huffman tree.

## How to use it?

1. Compression:

1.1. Compile the 'encode.cpp' using your favorite shell:

```
    g++ encode.cpp -o encode
```

1.2. After compiling give the name of the input file and the compressed file

```
    ./encode input.txt compressed.huf
```

2. Decompression:

2.1. Compile the 'decode.cpp' using your favorite shell:

```
    g++ decode.cpp -o encode
```

2.2. After compiling give the name of the input file and the compressed file

```
    ./decode compressed.txt output.txt
```

## Note:-

- Compresses and decompresses 1 file at a time.
- Useful for educative purposes.
