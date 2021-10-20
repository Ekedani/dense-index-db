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
};

class IndexBlock {
public:
    vector<IndexRecord *> records;

    //Uniform binary search from TAOCP
    SearchResult findKey(unsigned int keyValue);

public:
    //Block parameters
    const unsigned int MIN_KEY_VALUE;
    const unsigned int MAX_KEY_VALUE;

    IndexBlock(unsigned int minKeyValue, unsigned int maxKeyValue);

    //Returns pointer to record with given key or nullptr
    IndexRecord* get(unsigned int keyValue);

    //Returns true if removal is successful
    bool remove(unsigned int keyValue);

    //Returns true if insertion is successful
    bool add(unsigned int keyValue, unsigned int dataPointer);

    [[nodiscard]] unsigned int size() const{
        return records.size();
    }

    void outputRecords(){
        for (auto obj : records) {
            cout << "Key: " << obj->keyValue << ", Ptr: " << obj->dataPointer << '\n';
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
        //Creating new overflow area
        overflowArea = new IndexBlock(0, MAX_KEY_VALUE);

        //Creating new blocks
        unsigned int leftBorder = 0;
        const unsigned int BLOCK_STEP = MAX_KEY_VALUE / NUMBER_OF_BLOCKS;
        for (int blockCounter = 0; blockCounter < NUMBER_OF_BLOCKS; ++blockCounter) {
            auto block = new IndexBlock(leftBorder, leftBorder + BLOCK_STEP);
            leftBorder += BLOCK_STEP;
            this->blocks.push_back(block);
        }

        //Reading file
        //this->readFile();
    }

    bool add(unsigned int keyValue, unsigned int dataPointer);

    IndexRecord* get(unsigned int keyValue);

    bool remove(unsigned int keyValue);

};

