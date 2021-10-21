#pragma once

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

    long long statistics;

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

    void outputRecords();
};

class IndexSegment {
public:
    //Configuration of index segment
    static const unsigned int MAX_KEY_VALUE = INT_MAX;
    static const unsigned int MAX_BLOCK_SIZE = 2160;
    static const unsigned int NUMBER_OF_BLOCKS = 6;

    long long statistics;

    //Data
    vector<IndexBlock *> blocks;
    IndexBlock *overflowArea;
public:
    IndexSegment();

    //File handling methods
    void readFile();
    void saveFile();


    //Database methods
    bool add(unsigned int keyValue, unsigned int dataPointer);
    IndexRecord *get(unsigned int keyValue);
    bool remove(unsigned int keyValue);


    void outputLimits(){
        for (int i = 0; i < blocks.size(); ++i) {
            cout << "Block number " << i << ": " << "MIN: " << blocks[i]->MIN_KEY_VALUE << ", MAX: " << blocks[i]->MAX_KEY_VALUE << '\n';
            cout << "Block size: " << blocks[i]->size() << '\n';
        }
        cout << "Overflow area: " << "MIN: " << overflowArea->MIN_KEY_VALUE << ", MAX: " << overflowArea->MAX_KEY_VALUE << '\n';
        cout << "Block size: " << overflowArea->size() << '\n';
    }

};

