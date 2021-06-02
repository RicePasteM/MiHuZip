#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stdio.h"
#include "stdlib.h"
#include <windows.h>
#include <sys/stat.h>
#include <QTextCodec>
#include <QString>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <string>
#include <fstream>
#include <cassert>
#include <cstring>
#include <time.h>
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


QString str2qstr(const string str)
{
    return QString::fromLocal8Bit(str.data());
}

string qstr2str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->aboutBtn->setStyleSheet(//正常状态样式
                             "QPushButton{"
                             "background-color:rgb(82,185,243);"//背景色（也可以设置图片）
                             "border-radius:3px;"                   //边框圆角半径像素
                             "color:rgb(0,0,0);"                //字体颜色
                             "padding:6px;"                          //填衬
                             "}"
                             //鼠标按下样式
                             "QPushButton:pressed{"
                             "background-color:rgb(100,255,100);"
                             "color:rgb(0,0,0);"
                             "}"
                             //鼠标悬停样式
                             "QPushButton:hover{"
                             "background-color:rgb(86,171,255);"
                             "color:rgb(0,0,0);"
                             "}");
    ui->progressBar->setStyleSheet(
                             "QProgressBar{"
                             "        border: none;"
                             "        color: black;"
                             "        text-align: center;"
                             "        background: rgb(103,105,111);"
                             "        border-radius:3px;"
                             "}"
                             "QProgressBar::chunk {"
                             "        border: none;"
                             "        background: rgb(82,185,243);"
                             "        border-radius:3px;"
                             "}");

    ui->fileBtn->setAcceptDrops(true);
    ui->aboutBtn->setAcceptDrops(false);
    ui->runBtn->setAcceptDrops(false);
    ui->progressBar->setAcceptDrops(false);
    this ->setAcceptDrops(true);

    ui->progressBar->setTextVisible(true);
    ui->progressBar->setValue(0);
    ui->progressBar->setFormat("等待文件");

    ui->fileBtn->setText("点击选择或拖拽文件");

    connect(ui->aboutBtn,SIGNAL(clicked()),this,SLOT(aboutBtnClicked()));
    connect(ui->fileBtn,SIGNAL(clicked()),this,SLOT(chooseFile()));
    connect(ui->runBtn,SIGNAL(clicked()),this,SLOT(runZip()));

    readyToRun = false;
    disableRunBtn();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QString name = event->mimeData()->urls().first().toLocalFile();
    string fileName = qstr2str(name);
    qDebug() << name;
    char* fileName_c = (char*)fileName.data();
    struct stat s;

    if (stat(fileName_c,&s)==0)
    {
        if(s.st_mode & S_IFDIR)
        {
            QMessageBox::critical(NULL, "错误", "仅支持压缩单个文件。", QMessageBox::Ok , QMessageBox::Ok);
            std::cout<<"it's a directory"<<std::endl;
        }
        else if (s.st_mode & S_IFREG)
        {
            readyToRun = true;
            curFileName = fileName;
            enableRunBtn();
            std::cout<<"it's a file"<<std::endl;
        }
        else
        {
            QMessageBox::critical(NULL, "错误", "未知错误。", QMessageBox::Ok , QMessageBox::Ok);
            std::cout<<"not file not directory"<<std::endl;
        }
    }
    else
    {
        QMessageBox::critical(NULL, "错误", "文件不存在。", QMessageBox::Ok , QMessageBox::Ok);
        std::cout<<"error, doesn't exist"<<std::endl;
    }
}

void MainWindow::aboutBtnClicked()
{
    QMessageBox message(QMessageBox::NoIcon, "关于米哈压", "这是一个娱乐向的压缩工具，压缩效率低下，功能残缺不全，压缩速度超慢，唯一的优点是花里胡哨。\n"
    "目前只能压缩单个文件，算法是哈夫曼编码。\n软件制作：米糊\n灵感提供：米哈游（误）\n友商推荐：Bandzip");
    message.setIconPixmap(QPixmap(":/new/prefix1/icon.png"));
    message.exec();
}

void MainWindow::chooseFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("选择文件")
                                                    );
    if(fileName != "")
    {
        readyToRun = true;
        curFileName = qstr2str(fileName);
        enableRunBtn();
    }
    else
        return;
}

void MainWindow::enableRunBtn()
{
    ui->runBtn->setEnabled(true);
    ui->runBtn->setStyleSheet(//正常状态样式
                             "QPushButton{"
                             "background-color:rgb(82,185,243);"//背景色（也可以设置图片）
                             "border-radius:3px;"                   //边框圆角半径像素
                             "color:rgb(0,0,0);"                //字体颜色
                             "padding:6px;"                          //填衬
                             "cursor:auto;"
                             "}"
                             //鼠标按下样式
                             "QPushButton:pressed{"
                             "background-color:rgb(100,255,100);"
                             "color:rgb(0,0,0);"
                             "}"
                             //鼠标悬停样式
                             "QPushButton:hover{"
                             "background-color:rgb(86,171,255);"
                             "color:rgb(0,0,0);"
                             "}");
    ui->fileBtn->setStyleSheet(
                            //正常状态样式
                            "QPushButton{"
                            "color:white;"
                            "text-align:bottom;"
                            "background-image:url(':/new/prefix1/file_selected.png');"
                            "background-position:center;"
                            "background-repeat:no-repeat;"
                            "border:5px dotted rgb(103,105,111);"
                            "border-radius:5px;"                   //边框圆角半径像素
                            "padding:6px;"                          //填衬
                            "}"
                            //鼠标按下样式
                            "QPushButton:pressed{"
                            "border:5px solid"
                            "background-color:rgb(100,255,100);"
                            "color:rgb(0,0,0);"
                            "}"
                            //鼠标悬停样式
                            "QPushButton:hover{"
                            "border:5px dotted rgb(175, 177, 180);"
                            "border-radius:5px;"
                            "color:rgb(0,0,0);"
                            "}");
    int i = 0;
    for(i = curFileName.length() - 1 ; i >= 0 ; i --)
    {
        if(curFileName[i] == '/')
            break;
    }
    string tempStr = curFileName.substr(i + 1);
    ui->fileBtn->setText(str2qstr(tempStr));
    if((int)tempStr.find(".mihupack") != -1)
    {
        ui->runBtn->setText("解压");
    }
    else
    {
       ui->runBtn->setText("压缩");
       FILE * pFile;
       long size;
       char *tempChr = (char*)curFileName.data();
       pFile = fopen (tempChr,"rb");
       if (pFile==NULL)
       {
             QMessageBox::critical(NULL, "错误", "文件打开错误。", QMessageBox::Ok , QMessageBox::Ok);
             disableRunBtn();
             return;
       }
       else
       {
             fseek (pFile, 0, SEEK_END);   ///将文件指针移动文件结尾
             size=ftell (pFile); ///求出当前文件指针距离文件开始的字节数
             fclose (pFile);
             printf ("Size of file.cpp: %ld bytes.\n",size);
             ui->fileBtn->setText(ui->fileBtn->text() + '\n' + QString::number(size) + "Bytes");
             fileCompact = size;
             if(size <= 1024)
             {
                 QMessageBox::warning(NULL, "提示", "暂不支持对小于1KB的文件进行压缩。", QMessageBox::Yes , QMessageBox::Yes);
                 disableRunBtn();
                 return;
             }
       }
    }



    ui->progressBar->setFormat("准备就绪");
}

void MainWindow::disableRunBtn()
{
    curFileName = "";
    readyToRun = false;
    ui->runBtn->setEnabled(false);
    ui->runBtn->setStyleSheet(//正常状态样式
                             "QPushButton{"
                             "background-color:rgb(103,105,111);"//背景色（也可以设置图片）
                             "border-radius:3px;"                   //边框圆角半径像素
                             "color:rgb(0,0,0);"                //字体颜色
                             "padding:6px;"
                             "cursor:pointer;"
                             "}"
                             //鼠标按下样式
                             "QPushButton:pressed{"
                             "background-color:rgb(103,105,111);"
                             "color:rgb(0,0,0);"
                             "}"
                             //鼠标悬停样式
                             "QPushButton:hover{"
                             "background-color:rgb(103,105,111);"
                             "color:rgb(0,0,0);"
                             "}");
    ui->fileBtn->setStyleSheet(
                            //正常状态样式
                            "QPushButton{"
                            "color:white;"
                            "text-align:bottom;"
                            "background-image:url(':/new/prefix1/file.png');"
                            "background-position:center;"
                            "background-repeat:no-repeat;"
                            "border:5px dotted rgb(103,105,111);"
                            "border-radius:5px;"                   //边框圆角半径像素
                            "padding:6px;"                          //填衬
                            "}"
                            //鼠标按下样式
                            "QPushButton:pressed{"
                            "border:5px solid"
                            "background-color:rgb(100,255,100);"
                            "color:rgb(0,0,0);"
                            "}"
                            //鼠标悬停样式
                            "QPushButton:hover{"
                            "border:5px dotted rgb(175, 177, 180);"
                            "border-radius:5px;"
                            "color:rgb(0,0,0);"
                            "}");
    ui->fileBtn->setText("点击选择或拖拽文件");
    ui->runBtn->setText("压缩/解压");
    ui->progressBar->setFormat("等待文件");
    ui->progressBar->setValue(0);
}

void MainWindow::runZip()
{
    if(readyToRun == true)
    {
        if(ui->runBtn->text() == "压缩")
        {
            HuffmanZip();
        }
        else if(ui->runBtn->text() == "解压")
        {
            HuffmanUnZip();
        }
    }
}


void MainWindow::HuffmanZip()
{
    ui->progressBar->setOrientation(Qt::Horizontal);  // 水平方向
    ui->progressBar->setMinimum(0);  // 最小值
    ui->progressBar->setMaximum(0);  // 最大值

    clock_t time_start=clock();
    ui->runBtn->setEnabled(false);

    char *fileName = (char*)curFileName.data();

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
        QApplication::processEvents(QEventLoop::AllEvents, 100);
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
        QApplication::processEvents(QEventLoop::AllEvents, 100);
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
        QApplication::processEvents(QEventLoop::AllEvents, 100);
        cout << (int)(unsigned char)huffmanTree[i].byteData << " " << huffmanTree[i].freq << " " << huffmanTree[i].hasParent
            << " " << huffmanTree[i].lChild << " " << huffmanTree[i].rChild
            << " " << ((huffmanTree[i].huffmanCode != nullptr)? huffmanTree[i].huffmanCode :"")<< endl;
    }


    //Write the head infomation (include byte frequencies) into file.
    int t = 0,t1=0,t2 = 0;
    for(t = curFileName.length() - 1 ; t >= 0 ; t --)
    {
        if(curFileName[t] == '.')
        {
            t1=t;
        }
        if(curFileName[t] == '/')
        {
            t2=t;
            break;
        }
    }
    string tempStr2 = curFileName.substr(0,t1) + ".mihupack";
    char *outputName = (char*)tempStr2.data();
    cout << outputName << endl;
    ofstream Ofile(outputName,ios::out|ios::binary);

    const char *headInfo = "[MIHUPACK]This is a MiHu Pack,please use MiHuPacker to unpack.\n";
    Ofile.write((char*)headInfo, strlen(headInfo));

    Ofile.write((char*)&allCodes,sizeof(allCodes));
    Ofile.write((char*)&amountOfBytes, sizeof(amountOfBytes));

    string tempStr3 = curFileName.substr(t2 + 1);
    char *fileDiscreteName = (char*)tempStr3.data();
    int lengthOfFilename = strlen(fileDiscreteName);
    cout << fileDiscreteName << " " << lengthOfFilename << endl;
    Ofile.write((char*)&lengthOfFilename, sizeof(lengthOfFilename));
    Ofile.write((char*)fileDiscreteName, strlen(fileDiscreteName));

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
    ui->progressBar->setMinimum(0);  // 最小值
    ui->progressBar->setMaximum(100);  // 最大值
    for (int i = 0; i <= amountOfBytes - 1; i++)
    {
        QApplication::processEvents(QEventLoop::AllEvents, 100);
        int dProgress = (double)i * 1.0 / ( amountOfBytes - 1 ) * 100;
        ui->progressBar->setFormat(QString::fromLocal8Bit("%1%").arg(QString::number(dProgress)));
        ui->progressBar->setValue(dProgress);
        int codeIndex = 0;
        //cout << (int)fileData[i] << endl;
        assert(huffmanTree[HuffKey[fileData[i]]].huffmanCode != nullptr);
        char* code = huffmanTree[HuffKey[fileData[i]]].huffmanCode;
        while (codeIndex < (int)strlen(code))
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

    clock_t time_end=clock();

    FILE * pFile;
    long size;
    char *tempChr = outputName;
    int zippedFileCompact;
    pFile = fopen (tempChr,"rb");
    if (pFile==NULL)
    {
          QMessageBox::critical(NULL, "错误", "文件压缩错误。", QMessageBox::Ok , QMessageBox::Ok);
          disableRunBtn();
          return;
    }
    else
    {
          fseek (pFile, 0, SEEK_END);   ///将文件指针移动文件结尾
          size=ftell (pFile); ///求出当前文件指针距离文件开始的字节数
          fclose (pFile);
          printf ("Size of file.cpp: %ld bytes.\n",size);
          zippedFileCompact = size;
    }

    QMessageBox::about(NULL, "压缩完成", "原文件大小：" + QString::number(fileCompact) + "  压缩文件大小：" + QString::number(zippedFileCompact)
                       + "\n压缩率：" + QString::number((double)zippedFileCompact / fileCompact * 100) +
                       "%  耗时：" +  QString::number((time_end-time_start)/(double)CLOCKS_PER_SEC)+ "s");

    disableRunBtn();
    delete []fileData;
    delete fileDiscreteName;
    delete []tempStr;
    delete []huffmanTree;
}










void MainWindow::HuffmanUnZip()
{

        ui->progressBar->setOrientation(Qt::Horizontal);  // 水平方向
        ui->progressBar->setMinimum(0);  // 最小值
        ui->progressBar->setMaximum(0);  // 最大值

        clock_t time_start=clock();
        ui->runBtn->setEnabled(false);

        char *fileName = (char*)curFileName.data();

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

        for(index = 0 ; index <= min((int)strlen(headInfo) - 1,(int)strlen((const char*)fileData)) ; index ++)
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
            delete []fileData;
            delete []fileHead;
            QMessageBox::critical(NULL, "错误", "文件格式不合法。", QMessageBox::Ok , QMessageBox::Ok);
            return;
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
        //cout << allCodes << " " << amountOfSourceBytes << " " << sourceFileName << endl;

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
                //cout << (int)i << " " << freq[i] << endl;
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

        /*for (int i = 0; i < allCodes * 2 - 1; i++)
        {
            cout << huffmanTree[i].byteData << " " << huffmanTree[i].freq << " " << huffmanTree[i].hasParent
                << " " << huffmanTree[i].lChild << " " << huffmanTree[i].rChild
                << " " << ((huffmanTree[i].huffmanCode != nullptr)? huffmanTree[i].huffmanCode :"")<< endl;
        }*/

        //Decode the file.

        int t = 0,t1=0;
        for(t = curFileName.length() - 1 ; t >= 0 ; t --)
        {
            if(curFileName[t] == '/')
            {
                t1=t;
                break;
            }
        }

        cout << sourceFileName << endl;

        string tempStr2 = curFileName.substr(0,t1 + 1);
        string tempStr3 = sourceFileName;
        string tempStr4 = tempStr2 + tempStr3;
        char *outputName = (char*)tempStr4.data();
        cout << outputName << endl;
        ofstream Ofile(outputName, ios::out | ios::binary);

        int sourceByteIndex = 0;
        int curByteIndex = index;
        int curBitIndex = 0;

        const int rootNode = allCodes * 2 - 1 - 1;
        int curTreeNode = rootNode;

        char curByte = '\0';
        memcpy(&curByte,fileData + curByteIndex,sizeof(char));

        ui->progressBar->setMinimum(0);  // 最小值
        ui->progressBar->setMaximum(100);  // 最大值

        while(sourceByteIndex < amountOfSourceBytes)
        {
            QApplication::processEvents(QEventLoop::AllEvents, 100);
            int dProgress = (double)sourceByteIndex * 1.0 / ( amountOfSourceBytes ) * 100;
            ui->progressBar->setFormat(QString::fromLocal8Bit("%1%").arg(QString::number(dProgress)));
            ui->progressBar->setValue(dProgress);
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

        Ofile.close();

        clock_t time_end=clock();

        FILE * pFile;
        long size;
        cout << outputName << endl;
        char *tempChr = outputName;
        int zippedFileCompact;
        pFile = fopen (tempChr,"rb");
        if (pFile==NULL)
        {
              QMessageBox::critical(NULL, "错误", "压缩文件有损坏。", QMessageBox::Ok , QMessageBox::Ok);
              disableRunBtn();
              delete []fileData;
              delete []fileHead;
              delete fileName;
              delete outputName;
              delete []huffmanTree;
              return;
        }
        else
        {
              fseek (pFile, 0, SEEK_END);   ///将文件指针移动文件结尾
              size=ftell (pFile); ///求出当前文件指针距离文件开始的字节数
              fclose (pFile);
              printf ("Size of file.cpp: %ld bytes.\n",size);
              zippedFileCompact = size;
        }

        QMessageBox::about(NULL, "解压完成", "压缩文件大小：" + QString::number(fileCompact) + "  原文件大小：" + QString::number(zippedFileCompact)
                            + "\n耗时：" +  QString::number((time_end-time_start)/(double)CLOCKS_PER_SEC)+ "s");

        disableRunBtn();
        delete []fileData;
        delete []fileHead;
        delete fileName;
        delete outputName;
        delete []huffmanTree;
}
