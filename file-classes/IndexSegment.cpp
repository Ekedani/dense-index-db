//
// Created by User on 16.10.2021.
//

#include "IndexSegment.h"

IndexBlock::IndexBlock(unsigned int minKeyValue, unsigned int maxKeyValue) : MIN_KEY_VALUE(minKeyValue),
                                                                                         MAX_KEY_VALUE(maxKeyValue) {}

SearchResult IndexBlock::findKey(unsigned int keyValue) {
    //If block is empty
    if(records.empty()){
        auto result = SearchResult();
        result.success = false;
        result.position = 0;
        result.value = new IndexRecord;
        result.value->keyValue = INT_MAX;
        return result;
    }

    long long currentState = ceil(records.size() / 2.0);
    unsigned int delta = records.size() / 2;
    SearchResult searchResult{};

    while (delta >= 0) {
        searchResult.position = currentState ? currentState - 1 : currentState;
        long long curKeyValue;

        //Check if in fictive area
        curKeyValue = currentState > 0 ? records[searchResult.position]->keyValue : -1;

        if (curKeyValue == keyValue) {
            //Successful search
            searchResult.success = true;
            searchResult.value = records[searchResult.position];
            return searchResult;
        }
        if(delta == 0){
            //One more iteration to check delta == 0 value
            break;
        }

        //New search parameters
        keyValue < curKeyValue ? (currentState -= ceil(delta / 2.0)) : (currentState += ceil(delta / 2.0));
        delta /= 2;
    }

    //Check if position overflow
    if(searchResult.position > INT_MAX){
        searchResult.position = 0;
    }
    //Failure
    searchResult.value = records[searchResult.position];
    return searchResult;
}

IndexRecord *IndexBlock::get(unsigned int keyValue) {
    auto searchResult = findKey(keyValue);
    return searchResult.success ? searchResult.value : nullptr;
}

bool IndexBlock::remove(unsigned int keyValue) {
    auto searchResult = findKey(keyValue);
    if(searchResult.success){
        records.erase(records.begin() + searchResult.position);
        return true;
    }
    else{
        return false;
    }
}

bool IndexBlock::add(unsigned int keyValue, unsigned int dataPointer) {
    auto searchResult = findKey(keyValue);
    if(!searchResult.success){
        auto vecIterator = records.begin() + searchResult.position;
        if(searchResult.value->keyValue < keyValue) {
            vecIterator += 1;
        }
        auto newRecord = new IndexRecord{keyValue, dataPointer};
        records.insert(vecIterator, newRecord);
        return true;
    }
    else{
        return false;
    }
}

void IndexBlock::pushRecord(IndexRecord *newIndexRecord) {
    records.push_back(newIndexRecord);
}

vector<IndexRecord *> IndexBlock::getRecords() const {
    return records;
}

unsigned int IndexBlock::size() const {
    return records.size();
}

void IndexBlock::outputRecords() {
    for (auto obj : records) {
        cout << "Key: " << obj->keyValue << ", Ptr: " << obj->dataPointer << '\n';
    }
}

bool IndexSegment::add(unsigned int keyValue, unsigned int dataPointer) {
    for(auto block : blocks){
        if(block->MIN_KEY_VALUE <= keyValue && block->MAX_KEY_VALUE >= keyValue){
            if(block->size() < MAX_BLOCK_SIZE){
                auto result = block->add(keyValue, dataPointer);
                if(result){
                    this->saveFile();
                }
                return result;
            }
            else{
                auto result = overflowArea->add(keyValue, dataPointer);
                if(result){
                    this->saveFile();
                }
                return result;
            }
        }
    }
}

IndexRecord *IndexSegment::get(unsigned int keyValue) {
    for(auto block : blocks){
        if(block->MIN_KEY_VALUE <= keyValue && block->MAX_KEY_VALUE >= keyValue){
            auto result = block->get(keyValue);
            if(result == nullptr){
                result = overflowArea->get(keyValue);
            }
            return result;
        }
    }
}

bool IndexSegment::remove(unsigned int keyValue) {
    for(auto block : blocks){
        if(block->MIN_KEY_VALUE <= keyValue && block->MAX_KEY_VALUE >= keyValue){
            auto result = block->remove(keyValue);
            if(!result){
                result = overflowArea->remove(keyValue);
            }
            this->saveFile();
            return result;
        }
    }
}

void IndexSegment::saveFile() {
    ofstream filePtr;
    filePtr.open(R"(D:\Programming\dense-index-db\data\index_seg.csv)");

    //Saving blocks
    for (auto block : blocks) {
        for(auto obj : block->getRecords()){
            filePtr << obj->keyValue << ',' << obj->dataPointer << '\n';
        }
    }

    //
    filePtr << "#Overflow_Area\n";
    for (auto obj : overflowArea->getRecords()){
        filePtr << obj->keyValue << ',' << obj->dataPointer << '\n';
    }

    filePtr.close();
}

IndexSegment::IndexSegment() {
    //Creating new overflow area
    overflowArea = new IndexBlock(0, MAX_KEY_VALUE);

    //Creating new blocks
    unsigned int leftBorder = 0;
    const unsigned int BLOCK_STEP = MAX_KEY_VALUE / NUMBER_OF_BLOCKS;
    for (int blockCounter = 0; blockCounter < NUMBER_OF_BLOCKS; ++blockCounter) {
        auto block = new IndexBlock(leftBorder, leftBorder + BLOCK_STEP);
        leftBorder += BLOCK_STEP + 1;
        this->blocks.push_back(block);
    }

    //Reading file
    this->readFile();
}

void IndexSegment::output() {
    for (int i = 0; i < NUMBER_OF_BLOCKS; ++i) {
        cout << "Block number: " << i << '\n';
        blocks[i]->outputRecords();
    }
}

void IndexSegment::readFile() {
    ifstream filePtr;
    filePtr.open(R"(D:\Programming\dense-index-db\data\index_seg.csv)");
    string curLine;
    getline(filePtr, curLine);
    unsigned int currentBlock = 0;

    //Fill main blocks
    while (curLine != "#Overflow_Area") {
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

void IndexRecord::parseLine(string indexLine) {
    this->keyValue = stoi(indexLine.substr(0, indexLine.find(',')));
    indexLine.erase(0, indexLine.find(',') + 1);
    this->dataPointer = stoi(indexLine);
}
