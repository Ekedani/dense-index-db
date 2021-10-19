#include <fstream>
#include <vector>
#include <string>

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
private:
    vector<IndexRecord *> records;

    SearchResult findKey(unsigned int keyValue) {
        unsigned int currentState = records.size() / 2 + 1;
        unsigned int delta = records.size() / 2;
        SearchResult searchResult{};

        while (delta > 0) {
            searchResult.position = currentState + delta;
            unsigned int curKeyValue;

            if (currentState + delta == records.size()) {
                //Infinity
                curKeyValue = INT_MAX + 1;
            } else {
                curKeyValue = records[currentState + delta]->keyValue;
            }

            if (curKeyValue == keyValue) {
                //Successful search
                searchResult.success = true;
                searchResult.value = records[searchResult.position];
                return searchResult;
            }

            //New search parameters
            keyValue < curKeyValue ? currentState += (delta / 2 + 1) : currentState -= (delta / 2 + 1);
            delta /= 2;
        }

        //Failure
        searchResult.success = false;
        searchResult.value = records[searchResult.position];
        return searchResult;
    }

public:
    const unsigned int MIN_KEY_VALUE;
    const unsigned int MAX_KEY_VALUE;

    IndexBlock(unsigned int minKeyValue, unsigned int maxKeyValue);
    //TODO: ADD, GET, EDIT, DELETE
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

