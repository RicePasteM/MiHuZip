#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <cstring>
#include <algorithm>
using namespace std;

typedef struct _HuffmanNode
{
    int freq = 0;
    unsigned char byteData = '\0';
    int lChild = -1;
    int rChild = -1;
    char* huffmanCode = nullptr;
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
    char fileName[] = "output.mihupack";

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

    //To verify file format.
    const char* headInfo = "[MIHUPACK]This is a MiHu Pack,please use MiHuPacker to unpack.\n";
    char* fileHead = new char[strlen(headInfo) + 1];

    for(index = 0 ; index <= min(strlen(headInfo) - 1,strlen((const char*)fileData)) ; index ++)
    {
    	fileHead[index] = fileData[index];
	}
	fileHead[index] = '\0';
	if (strcmp(fileHead, headInfo) == 0)
    {
        cout << "Valid file format,decoding..." << endl;
    }
    else
    {
    	cout << "Invalid file format!" << endl;
    	return 0;
	}



    //To get the frequency of the bytes in the source file.
	int allCodes = 0;
	int amountOfSourceBytes = 0;
	memcpy(&allCodes,fileData + index,sizeof(int));
	index += sizeof(int);
	memcpy(&amountOfSourceBytes,fileData + index,sizeof(int));
	index += sizeof(int);

	int i = 0;
    int lengthOfFilename = 0;
    memcpy(&lengthOfFilename,fileData + index,sizeof(int));
    index += sizeof(int);
    char *sourceFileName = new char[lengthOfFilename + 1];
    for(i = 0;i < lengthOfFilename;i ++)
    {
        sourceFileName[i] = fileData[index + i];
    }
    sourceFileName[i] = '\0';
    index += i;
	cout << allCodes << " " << amountOfSourceBytes << " " << sourceFileName << endl;

	int freq[256] = { 0 };
	for (int i = 0; i <= allCodes - 1; i++)
    {
    	unsigned char byteData = '\0';
    	int frequency = 0;
    	memcpy(&byteData,fileData + index,sizeof(char));
    	index += sizeof(char);
    	memcpy(&frequency,fileData + index,sizeof(int));
    	index += sizeof(int);
    	freq[byteData] = frequency;
    }

	HuffmanNode* huffmanTree = new HuffmanNode[allCodes * 2 - 1];
    int nodeAmount = 0;
    for (int i = 0; i <= 255; i++)
    {
        if (freq[i] != 0)
        {
        	cout << (int)i << " " << freq[i] << endl;
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
        cout << huffmanTree[i].byteData << " " << huffmanTree[i].freq << " " << huffmanTree[i].hasParent
            << " " << huffmanTree[i].lChild << " " << huffmanTree[i].rChild
            << " " << ((huffmanTree[i].huffmanCode != nullptr)? huffmanTree[i].huffmanCode :"")<< endl;
    }

    //Decode the file.
	ofstream Ofile(sourceFileName, ios::out | ios::binary);

	int sourceByteIndex = 0;
	int curByteIndex = index;
	int curBitIndex = 0;

	const int rootNode = allCodes * 2 - 1 - 1;
	int curTreeNode = rootNode;

	char curByte = '\0';
	memcpy(&curByte,fileData + curByteIndex,sizeof(char));

	while(sourceByteIndex < amountOfSourceBytes)
	{
		int curBit = ( curByte >> (7 - curBitIndex) ) &  1 ;
		if(curBit == 1)
		{
			curTreeNode = huffmanTree[curTreeNode].lChild;
		}
		else
		{
			curTreeNode = huffmanTree[curTreeNode].rChild;
		}
		if(huffmanTree[curTreeNode].huffmanCode != nullptr)
		{
			//cout << huffmanTree[curTreeNode].byteData << " " << sourceByteIndex << " " << amountOfSourceBytes << endl;
			Ofile.write((char*)&huffmanTree[curTreeNode].byteData, sizeof(huffmanTree[curTreeNode].byteData));
			curTreeNode = rootNode;
			sourceByteIndex ++;
		}
		curBitIndex ++;
		if(curBitIndex == 8)
		{
			curByteIndex ++;
			curBitIndex = 0;
			memcpy(&curByte,fileData + curByteIndex,sizeof(char));
		}
	}
}
