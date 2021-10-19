#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

struct IndexRecord {
    unsigned int keyValue;
    unsigned int dataPointer;

    void parseLine(string indexLine) {
        this->keyValue = stoi(indexLine.substr(0, indexLine.find(';') + 1));
        indexLine.erase(0, indexLine.find(';') + 1);
        this->dataPointer = stoi(indexLine.substr(0, indexLine.find(';') + 1));
    }
};

struct SearchResult {
    bool success;
    unsigned int position;
    IndexRecord *value;

    void output() {
        cout << "Success: " << success << '\n';
        cout << "Position: " << position << '\n';
    }
};

class IndexBlock {
public:
    vector<IndexRecord *> records;
    //Uniform binary search from TAOCP
    SearchResult findKey(unsigned int keyValue);

public:
    const unsigned int MIN_KEY_VALUE;
    const unsigned int MAX_KEY_VALUE;

    IndexBlock(unsigned int minKeyValue, unsigned int maxKeyValue);

    //Returns pointer to record with given key or nullptr
    IndexRecord* get(unsigned int keyValue);

    //Returns true if removal is successful
    bool remove(unsigned int keyValue){
        auto searchResult = findKey(keyValue);
        if(searchResult.success){

            return true;
        }
        else{
            return false;
        }
    }

    //Returns true if insertion is successful
    bool add(unsigned int keyValue, unsigned int dataPointer){
        auto searchResult = findKey(keyValue);
        if(!searchResult.success){

            return true;
        }
        else{
            return false;
        }
    }
};

class IndexSegment {
private:
    //Configuration of index segment
    static const unsigned int MAX_KEY_VALUE = INT_MAX;
    static const unsigned int MAX_BLOCK_SIZE = 1500;
    static const unsigned int NUMBER_OF_BLOCKS = 4;

    //Data
    vector<IndexBlock *> blocks;
    IndexBlock *overflowArea;
public:
    void readFile() {
        ifstream fileStream;
        fileStream.open("/* Way to file*/");
        const unsigned int BLOCK_STEP = INT_MAX / NUMBER_OF_BLOCKS;
        auto currentBlock = new IndexBlock(0, BLOCK_STEP);
        unsigned int currentBlockNum = 1;
        while (!fileStream.eof()) {
            //Get string of data
            string dataLine;
            getline(fileStream, dataLine);
            auto currentIndexLine = new IndexRecord;
            currentIndexLine->parseLine(dataLine);

            if (currentIndexLine->keyValue < currentBlock->MIN_KEY_VALUE ||
                currentIndexLine->keyValue > currentBlock->MAX_KEY_VALUE) {
                //Not in block range
                if (currentBlockNum < NUMBER_OF_BLOCKS) {
                    //Create another block
                    currentBlock = new IndexBlock(currentBlock->MAX_KEY_VALUE + 1,
                                                  currentBlock->MAX_KEY_VALUE + BLOCK_STEP);
                    this->blocks.push_back(currentBlock);
                    currentBlockNum++;
                } else {
                    currentBlock = this->overflowArea;
                }
            }

        }
        if (currentBlockNum < NUMBER_OF_BLOCKS) {

        }
        fileStream.close();
    }

    void saveFile() {

    }

    IndexSegment() {
        overflowArea = new IndexBlock(0, MAX_KEY_VALUE);
        this->readFile();
    }

};

