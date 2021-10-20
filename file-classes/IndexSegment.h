#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;


struct IndexRecord {
    unsigned int keyValue;
    unsigned int dataPointer;

    void parseLine(string indexLine);
};

struct SearchResult {
    bool success;
    unsigned int position;
    IndexRecord *value;
};

class IndexBlock {
private:
    vector<IndexRecord *> records;

    //Uniform binary search from TAOCP
    SearchResult findKey(unsigned int keyValue);

public:
    //Block parameters
    const unsigned int MIN_KEY_VALUE;
    const unsigned int MAX_KEY_VALUE;

    IndexBlock(unsigned int minKeyValue, unsigned int maxKeyValue);

    //Returns pointer to record with given key or nullptr
    IndexRecord *get(unsigned int keyValue);

    //Returns true if removal is successful
    bool remove(unsigned int keyValue);

    //Returns true if insertion is successful
    bool add(unsigned int keyValue, unsigned int dataPointer);

    [[nodiscard]] unsigned int size() const;

    vector<IndexRecord *> getRecords() const;

    //To read .csv files faster
    void pushRecord(IndexRecord *newIndexRecord);

    void outputRecords() {
        for (auto obj : records) {
            cout << "Key: " << obj->keyValue << ", Ptr: " << obj->dataPointer << '\n';
        }
    }
};

class IndexSegment {
public:
    //Configuration of index segment
    static const unsigned int MAX_KEY_VALUE = INT_MAX;
    static const unsigned int MAX_BLOCK_SIZE = 1500;
    static const unsigned int NUMBER_OF_BLOCKS = 4;

    //Data
    vector<IndexBlock *> blocks;
    IndexBlock *overflowArea;
public:
    void readFile() {
        ifstream filePtr;
        filePtr.open(R"(D:\Programming\dense-index-db\data\index_seg.csv)");
        string curLine;
        getline(filePtr, curLine);
        unsigned int currentBlock = 0;

        //Fill main blocks
        while (curLine != "===OVERFLOW_AREA===") {
            auto curIndexRecord = new IndexRecord;
            curIndexRecord->parseLine(curLine);

            //Selecting new blocks
            while (!(curIndexRecord->keyValue >= blocks[currentBlock]->MIN_KEY_VALUE &&
                     curIndexRecord->keyValue <= blocks[currentBlock]->MAX_KEY_VALUE)) {
                currentBlock++;
            }

            blocks[currentBlock]->pushRecord(curIndexRecord);
            getline(filePtr, curLine);
        }

        //Fill overflow area
        while (!filePtr.eof()) {
            getline(filePtr, curLine);
            if(curLine.empty()){
                continue;
            }
            auto curIndexRecord = new IndexRecord;
            curIndexRecord->parseLine(curLine);
            overflowArea->pushRecord(curIndexRecord);
        }
        filePtr.close();
    }

    void saveFile();

    IndexSegment();

    //Database methods
    bool add(unsigned int keyValue, unsigned int dataPointer);

    IndexRecord *get(unsigned int keyValue);

    bool remove(unsigned int keyValue);

    void output();

};

