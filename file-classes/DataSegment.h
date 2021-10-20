#include <string>
#include <utility>
#include <vector>
#include <fstream>

using namespace std;

struct DataRecord{
    unsigned int keyValue;
    string dataValue;
    bool notDeleted;
};

class DataSegment {
private:
    vector<DataRecord*> fileData;
public:
    DataSegment() {
    }

    void saveFile(){

    }

    unsigned int add(unsigned int keyValue, string dataValue){
        auto newRecord = new DataRecord{keyValue, std::move(dataValue), true};
        fileData.push_back(newRecord);
        saveFile();
        return fileData.size() - 1;
    }

    DataRecord* get(unsigned int dataPointer){
        return fileData[dataPointer];
    }

    void remove(unsigned int dataPointer){
        fileData[dataPointer]->notDeleted = false;
        saveFile();
    }

    void edit(unsigned int dataPointer, string dataValue){
        fileData[dataPointer]->dataValue = std::move(dataValue);
        saveFile();
    }

};

