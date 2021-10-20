#include "DenseIndexFile.h"

bool DenseIndexFile::remove(unsigned int keyValue) {
    auto indexSearchResult = indexSegment->get(keyValue);
    if (indexSearchResult == nullptr) {
        return false;
    }
    dataSegment->remove(indexSearchResult->dataPointer);
    return indexSegment->remove(keyValue);
}

string DenseIndexFile::get(unsigned int keyValue) {
    auto indexSearchResult = indexSegment->get(keyValue);
    if (indexSearchResult == nullptr) {
        return "";
    }
    auto dataSearchResult = dataSegment->get(indexSearchResult->dataPointer);
    return dataSearchResult->dataValue;
}

bool DenseIndexFile::add(unsigned int keyValue, string dataValue) {
    if (indexSegment->get(keyValue) != nullptr) {
        return false;
    }
    auto dataPointer = dataSegment->add(keyValue, std::move(dataValue));
    indexSegment->add(keyValue, dataPointer);
    return true;
}

bool DenseIndexFile::edit(unsigned int keyValue, string dataValue) {
    auto indexSearchResult = indexSegment->get(keyValue);
    if (indexSearchResult == nullptr) {
        return false;
    }
    dataSegment->edit(indexSearchResult->dataPointer, std::move(dataValue));
}

DenseIndexFile::DenseIndexFile() {
    indexSegment = new IndexSegment();
    dataSegment = new DataSegment();
}

void DenseIndexFile::output() {
    cout << "Index segment: " << '\n';
    indexSegment->output();
    cout << "Data segment: " << '\n';
    dataSegment->output();
}
