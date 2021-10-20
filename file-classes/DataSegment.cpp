#include "DataSegment.h"

void DataSegment::saveFile() {
    ofstream filePtr;
    filePtr.open(R"(D:\Programming\dense-index-db\data\data_seg.csv)");
    for (auto obj : fileData) {
        filePtr << obj->keyValue << ',' << '"' << obj->dataValue << '"' << ','
                << obj->notDeleted << '\n';
    }
    filePtr.close();
}

unsigned int DataSegment::add(unsigned int keyValue, string dataValue) {
    auto newRecord = new DataRecord{keyValue, std::move(dataValue), true};
    fileData.push_back(newRecord);
    saveFile();
    return fileData.size() - 1;
}

void DataSegment::remove(unsigned int dataPointer) {
    fileData[dataPointer]->notDeleted = false;
    saveFile();
}

void DataSegment::edit(unsigned int dataPointer, string dataValue) {
    fileData[dataPointer]->dataValue = std::move(dataValue);
    saveFile();
}

void DataSegment::output() {
    for (auto obj : fileData) {
        cout << obj->keyValue << ';' << obj->dataValue << ';' << obj->notDeleted << '\n';
    }
}

DataRecord *DataSegment::get(unsigned int dataPointer) {
    return fileData[dataPointer];
}

void DataSegment::readFile() {
    ifstream filePtr;
    filePtr.open(R"(D:\Programming\dense-index-db\data\data_seg.csv)");
    while (!filePtr.eof()) {
        string curDataLine;
        getline(filePtr, curDataLine);
        if (curDataLine.empty()) {
            continue;
        }
        auto newDataRecord = new DataRecord;
        newDataRecord->parseString(curDataLine);
        fileData.push_back(newDataRecord);
    }
    filePtr.close();
}

DataSegment::DataSegment() {
    this->readFile();
}

void DataRecord::parseString(string dataLine) {
    this->keyValue = stoi(dataLine.substr(0, dataLine.find(',')));
    dataLine.erase(0, dataLine.find(',') + 1);
    this->dataValue = dataLine.substr(1, dataLine.find("\",") - 1);
    dataLine.erase(0, dataLine.find("\",") + 2);
    this->notDeleted = stoi(dataLine);
}
