//
// Created by User on 16.10.2021.
//

#include "IndexSegment.h"

IndexBlock::IndexBlock(unsigned int minKeyValue, unsigned int maxKeyValue) : MIN_KEY_VALUE(minKeyValue),
                                                                                         MAX_KEY_VALUE(maxKeyValue) {}

SearchResult IndexBlock::findKey(unsigned int keyValue) {
    unsigned int currentState = records.size() / 2 + 1 ;
    unsigned int delta = records.size() / 2;
    SearchResult searchResult{};

    while (delta >= 0) {
        searchResult.position = currentState ? currentState - 1 : currentState;
        unsigned int curKeyValue;

        //Check if in fictive area
        curKeyValue = currentState > records.size() ? INT_MAX + 1 : records[searchResult.position]->keyValue;

        if (curKeyValue == keyValue) {
            //Successful search
            searchResult.success = true;
            searchResult.value = records[searchResult.position];
            return searchResult;
        }
        if(delta == 0){
            //One more iteration to check delta == 0 value
            searchResult.position--;
            break;
        }

        //New search parameters
        keyValue < curKeyValue ? (currentState -= (delta / 2 + 1)) : (currentState += (delta / 2 + 1));
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
