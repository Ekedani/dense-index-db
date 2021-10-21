#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <iostream>

using namespace std;

//Program representation of a data-file string
struct DataRecord {
    unsigned int keyValue;
    string dataValue;
    bool notDeleted;

    void parseString(string dataLine);
};

//Program representation of a data-file
class DataSegment {
private:
    vector<DataRecord *> fileData;
public:
    DataSegment();

    //File handling methods
    void readFile();
    void saveFile();

    //Database methods
    unsigned int add(unsigned int keyValue, string dataValue);
    DataRecord *get(unsigned int dataPointer);
    void remove(unsigned int dataPointer);
    void edit(unsigned int dataPointer, string dataValue);

};

