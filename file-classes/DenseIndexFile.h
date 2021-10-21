#pragma once

#include "DataSegment.h"
#include "IndexSegment.h"

//Program representation of dense-index file
class DenseIndexFile {
private:
    IndexSegment* indexSegment;
    DataSegment* dataSegment;
public:
    DenseIndexFile();

    //Database methods
    bool add(unsigned int keyValue, string dataValue);
    string get(unsigned int keyValue);
    bool remove(unsigned int keyValue);
    bool edit(unsigned int keyValue, string dataValue);

    long long getStatistics(){
        return indexSegment->statistics;
    }

    void outputLimits(){
        indexSegment->outputLimits();
    }

};
