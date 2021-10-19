#include <iostream>
#include "file-classes/IndexSegment.h"

int main() {
    auto testBlock = IndexBlock(0, INT_MAX);
    for (int i = 0; i < 100; ++i) {
        if(i % 2 == 0){
            continue;
        }
        auto testRecord = new IndexRecord;
        testRecord->keyValue = i;
        testRecord->dataPointer = i;
        testBlock.records.push_back(testRecord);
    }
    //testBlock.add(0, 0);
    //testBlock.add(56, 54);
    cout << testBlock.records.size() << '\n';
    testBlock.add(0, 0);
    testBlock.add(16, 16);
    testBlock.add(18, 18);
    testBlock.add(INT_MAX, 54);
    testBlock.outputRecords();
    return 0;
}
