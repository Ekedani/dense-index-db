//
// Created by User on 16.10.2021.
//

#include "IndexSegment.h"

IndexBlock::IndexBlock(unsigned int minKeyValue, unsigned int maxKeyValue) : MIN_KEY_VALUE(minKeyValue),
                                                                                         MAX_KEY_VALUE(maxKeyValue) {}

SearchResult IndexBlock::findKey(unsigned int keyValue) {
    unsigned int currentState = records.size() / 2 + 1;
    unsigned int delta = records.size() / 2;
    SearchResult searchResult{};

    while (delta >= 0) {
        searchResult.position = currentState - 1;
        unsigned int curKeyValue;

        //Check if in fictive area
        curKeyValue = currentState > records.size() ? INT_MAX + 1 : records[currentState - 1]->keyValue;

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
        keyValue < curKeyValue ? (currentState -= (delta / 2 + 1)) : (currentState += (delta / 2 + 1));
        delta /= 2;
    }

    //Failure
    searchResult.value = records[searchResult.position];
    return searchResult;
}

IndexRecord *IndexBlock::get(unsigned int keyValue) {
    auto searchResult = findKey(keyValue);
    return searchResult.success ? searchResult.value : nullptr;
}
