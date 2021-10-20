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

    //Failure
    if(searchResult.position > INT_MAX){
        searchResult.position = 0;
    }
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
    filePtr.open("D:\\Programming\\dense-index-db\\data\\index_seg.csv");

    //Saving blocks
    for (auto block : blocks) {
        for(auto obj : block->getRecords()){
            filePtr << obj->keyValue << ',' << obj->dataPointer << '\n';
        }
    }

    //
    filePtr << "===OVERFLOW_AREA===\n";
    for (auto obj : overflowArea->getRecords()){
        filePtr << obj->keyValue << ',' << obj->dataPointer << '\n';
    }

    filePtr.close();
}
