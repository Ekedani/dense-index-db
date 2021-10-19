#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct DataRecord{
    int keyValue;
    string dataValue;
    bool notDeleted;
};

class DataSegment {
private:
    fstream filePtr;
    vector<DataRecord> fileData;
public:
    DataSegment() {
        filePtr.open("data\\data_seg.csv");
    }

    //TODO: add, get, edit, delete

    ~DataSegment(){
        filePtr.close();
    }
};

