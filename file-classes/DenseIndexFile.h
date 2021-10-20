#include <utility>

#include "DataSegment.h"
#include "IndexSegment.h"

class DenseIndexFile {
private:
    IndexSegment* indexSegment;
    DataSegment* dataSegment;
public:
    DenseIndexFile(){
        indexSegment = new IndexSegment();
        dataSegment = new DataSegment();
    }

    bool add(unsigned int keyValue, string dataValue){
        if(indexSegment->get(keyValue) != nullptr){
            return false;
        }
        auto dataPointer = dataSegment->add(keyValue, std::move(dataValue));
        indexSegment->add(keyValue, dataPointer);
        return true;
    }

    string get(unsigned int keyValue){
        auto indexSearchResult = indexSegment->get(keyValue);
        if(indexSearchResult == nullptr){
            return "";
        }
        auto dataSearchResult = dataSegment->get(indexSearchResult->dataPointer);
        return dataSearchResult->dataValue;
    }

    bool remove(unsigned int keyValue){
        auto indexSearchResult = indexSegment->get(keyValue);
        if(indexSearchResult == nullptr){
            return false;
        }
        dataSegment->remove(indexSearchResult->dataPointer);
        return indexSegment->remove(keyValue);
    }

    bool edit(unsigned int keyValue, string dataValue){
        auto indexSearchResult = indexSegment->get(keyValue);
        if(indexSearchResult == nullptr){
            return false;
        }
        dataSegment->edit(indexSearchResult->dataPointer, std::move(dataValue));
    }

    void output(){
        cout << "Index segment: " << '\n';
        indexSegment->output();
        cout << "Data segment: " << '\n';
        dataSegment->output();
    }

};
