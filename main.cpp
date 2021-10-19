#include <iostream>
#include "file-classes/IndexSegment.h"

int main() {
    auto testBlock = IndexBlock(0, INT_MAX);
    for (int i = 0; i < 100; ++i) {
        auto testRecord = new IndexRecord;
        testRecord->keyValue = i;
        testRecord->dataPointer = i;
        testBlock.records.push_back(testRecord);
    }
    for (int j = 0; j < 100; ++j) {
        auto result = testBlock.findKey(j);
        cout << j << " success "  << result.success << '\n';
    }
    auto result = testBlock.findKey(INT_MAX);
    auto search = testBlock.get(99);
    cout << "Search values: " << search->keyValue << " and " << search->dataPointer;
    return 0;
}
