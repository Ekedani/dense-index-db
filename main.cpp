#include <iostream>
#include "file-classes/IndexSegment.h"

int main() {
    auto testBlock = IndexBlock(0, INT_MAX);
    for (int i = 0; i < 100; ++i) {
        /*if(i % 2 == 0){
            continue;
        }*/
        auto testRecord = new IndexRecord;
        testRecord->keyValue = i;
        testRecord->dataPointer = i;
        testBlock.records.push_back(testRecord);
    }
    cout << testBlock.records.size() << '\n';
    for (int j = 0; j < 100; ++j) {
        auto result = testBlock.findKey(j);
        cout << j << " success "  << result.success << '\n';
    }
    return 0;
}
