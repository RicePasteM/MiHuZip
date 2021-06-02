#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstring>
using namespace std;

typedef struct _HuffmanNode
{
    int freq = 0;
    unsigned char byteData = '\0';
    int lChild = -1;
    int rChild = -1;
    char *huffmanCode = nullptr;
    bool hasParent = false;
}HuffmanNode;

void calculateHuffmanCode(HuffmanNode* huffmanTree, int root, int index, char* code)
{
    if (huffmanTree[root].lChild != -1 && huffmanTree[root].rChild != -1)
    {
        code[index] = '1';
        calculateHuffmanCode(huffmanTree, huffmanTree[root].lChild, index + 1, code);
        code[index] = '0';
        calculateHuffmanCode(huffmanTree, huffmanTree[root].rChild, index + 1, code);
    }
    else
    {
        code[index] = '\0';
        huffmanTree[root].huffmanCode = new char[strlen(code)];
        strcpy(huffmanTree[root].huffmanCode, code);
    }
}

int main()
{
    char fileName[] = "test.txt";

    int amountOfBytes = 0;
    unsigned char c = '\0';


    //To count the number of bytes in the file.
    ifstream Ifile;
    Ifile.open(fileName, ios::in | ios::binary);
    while (Ifile.read((char*)&c, sizeof(char)))
        amountOfBytes++;
    Ifile.close();


    //To write the fileData into memory.
    unsigned char* fileData = new unsigned char[amountOfBytes + 1];
    Ifile.open(fileName, ios::in | ios::binary);
    int index = 0;
    while (Ifile.read((char*)&c, sizeof(char)))
    {
        fileData[index] = c;
        index++;
    }

    //To count the frequency of the bytes in the file.
    int freq[256] = { 0 };
    int allCodes = 0;
    for (int i = 0; i <= amountOfBytes - 1; i++)
    {
        freq[(int)fileData[i]] ++;
    }

    for (int i = 0; i <= 255; i++)
    {
        if(freq[i] != 0) allCodes ++;
    }
    cout << allCodes << endl;

    //Create a Huffman Tree with the freqs.
    HuffmanNode* huffmanTree = new HuffmanNode[allCodes * 2 - 1];
    int nodeAmount = 0;
    for (int i = 0; i <= 255; i++)
    {
        if (freq[i] != 0)
        {
            cout << i << endl;
            huffmanTree[nodeAmount].byteData = i;
            huffmanTree[nodeAmount].lChild = -1;
            huffmanTree[nodeAmount].rChild = -1;
            huffmanTree[nodeAmount].freq = freq[i];
            huffmanTree[nodeAmount].hasParent = false;
            nodeAmount ++;
        }
    }
    while (nodeAmount < allCodes * 2 - 1)
    {
        int min1 = 2147483647, min2 = 2147483647;
        int minIndex1 = -1, minIndex2 = -1;
        for (int i = 0; i <= nodeAmount - 1; i++)
        {
            if (huffmanTree[i].hasParent == true) continue;
            if (huffmanTree[i].freq < min1)
            {
                min2 = min1;
                minIndex2 = minIndex1;
                min1 = huffmanTree[i].freq;
                minIndex1 = i;
            }
            else if (huffmanTree[i].freq < min2)
            {
                min2 = huffmanTree[i].freq;
                minIndex2 = i;
            }
        }
        assert(minIndex1 != -1 && minIndex2 != -1);

        huffmanTree[nodeAmount].byteData = 0;
        huffmanTree[nodeAmount].freq = min1 + min2;

        huffmanTree[nodeAmount].lChild = minIndex1;
        huffmanTree[minIndex1].hasParent = true;

        huffmanTree[nodeAmount].rChild = minIndex2;
        huffmanTree[minIndex2].hasParent = true;

        huffmanTree[nodeAmount].hasParent = false;
        nodeAmount++;
    }


    //Calculate the huffmanCode.
    char* tempStr = new char[nodeAmount];
    calculateHuffmanCode(huffmanTree, nodeAmount - 1, 0, tempStr);


    for (int i = 0; i < allCodes * 2 - 1; i++)
    {
        cout << (int)(unsigned char)huffmanTree[i].byteData << " " << huffmanTree[i].freq << " " << huffmanTree[i].hasParent
            << " " << huffmanTree[i].lChild << " " << huffmanTree[i].rChild
            << " " << ((huffmanTree[i].huffmanCode != nullptr)? huffmanTree[i].huffmanCode :"")<< endl;
    }


    //Write the head infomation (include byte frequencies) into file.
    ofstream Ofile("output.mihupack",ios::out|ios::binary);

    const char *headInfo = "[MIHUPACK]This is a MiHu Pack,please use MiHuPacker to unpack.\n";
    Ofile.write((char*)headInfo, strlen(headInfo));

    Ofile.write((char*)&allCodes,sizeof(allCodes));
    Ofile.write((char*)&amountOfBytes, sizeof(amountOfBytes));

    int lengthOfFilename = strlen(fileName);
    Ofile.write((char*)&lengthOfFilename, sizeof(lengthOfFilename));
    Ofile.write((char*)&fileName, strlen(fileName));

    for (int i = 0; i <= allCodes - 1; i++)
    {
        Ofile.write((char*)&huffmanTree[i].byteData, sizeof(huffmanTree[i].byteData));
        Ofile.write((char*)&huffmanTree[i].freq, sizeof(huffmanTree[i].freq));
    }

    //Create the key-value pairs of byteData-huffmantreeIndex.
    int HuffKey[256];
    memset(HuffKey, -1, sizeof(HuffKey));
    for (int i = 0; i <= allCodes - 1; i++)
    {
        HuffKey[(int)(unsigned char)huffmanTree[i].byteData] = i;
    }

    //Write the huffmanCodes into file.
    char byte = '\0';
    bool isByteEmpty = true;
    int bitIndex = 0;
    for (int i = 0; i <= amountOfBytes - 1; i++)
    {
        int codeIndex = 0;
        char tempChar = '\0';
        //cout << (int)fileData[i] << endl;
        assert(huffmanTree[HuffKey[fileData[i]]].huffmanCode != nullptr);
        char* code = huffmanTree[HuffKey[fileData[i]]].huffmanCode;
        while (codeIndex < strlen(code))
        {
            char bit = code[codeIndex] - '0';
            //cout << (int)bit;
            byte = byte | (bit <<  (7 - bitIndex) );
            bitIndex ++;
            codeIndex ++;
            isByteEmpty = false;
            if (bitIndex == 8)
            {
                //cout << "!" << (int)(unsigned char)byte << endl;
                Ofile.write((char*)&byte, sizeof(byte));
                byte = '\0';
                isByteEmpty = true;
                bitIndex = 0;
            }
        }
    }


    if(!isByteEmpty) Ofile.write((char*)&byte, sizeof(byte));


    /*
    for(int i = 0 ; i <= amountOfBytes - 1; i ++)
    {
       Ofile.write((char*)(fileData + i), sizeof(char));
    }
    */
    Ofile.close();


}
