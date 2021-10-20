#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

struct DataRecord {
    unsigned int keyValue;
    string dataValue;
    bool notDeleted;

    void parseString(string dataLine){
        this->keyValue = stoi(dataLine.substr(0, dataLine.find(',')));
        dataLine.erase(0, dataLine.find(',') + 1);
        this->dataValue = dataLine.substr(1, dataLine.find("\","));
        dataLine.erase(0, dataLine.find("\",") + 2);
        this->notDeleted = stoi(dataLine);
    }
};

class DataSegment {
private:
    vector<DataRecord *> fileData;
public:
    DataSegment() {
    }

    void readFile(){
        ifstream filePtr;
        filePtr.open("D:\\Programming\\dense-index-db\\data\\data_seg.csv");
        while(!filePtr.eof()){
            string curDataLine;
            getline(filePtr, curDataLine);
            auto newDataRecord = new DataRecord;
            newDataRecord->parseString(curDataLine);
            fileData.push_back(newDataRecord);
        }
        filePtr.close();
    }

    void saveFile() {
        ofstream filePtr;
        filePtr.open("D:\\Programming\\dense-index-db\\data\\data_seg.csv");
        for (auto obj : fileData) {
            filePtr << obj->keyValue << ',' << '"' << obj->dataValue << '"' << ','
                    << obj->notDeleted << '\n';
        }
        filePtr.close();
    }

    unsigned int add(unsigned int keyValue, string dataValue) {
        auto newRecord = new DataRecord{keyValue, std::move(dataValue), true};
        fileData.push_back(newRecord);
        saveFile();
        return fileData.size() - 1;
    }

    DataRecord *get(unsigned int dataPointer) {
        return fileData[dataPointer];
    }

    void remove(unsigned int dataPointer) {
        fileData[dataPointer]->notDeleted = false;
        saveFile();
    }

    void edit(unsigned int dataPointer, string dataValue) {
        fileData[dataPointer]->dataValue = std::move(dataValue);
        saveFile();
    }

    void output() {
        for (auto obj : fileData) {
            cout << obj->keyValue << ';' << obj->dataValue << ';' << obj->notDeleted << '\n';
        }
    }

};

