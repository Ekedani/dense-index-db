#include <iostream>
#include "file-classes/DenseIndexFile.h"
#include "helpers/RandomDataGenerator.h"

int main() {
    auto testFile = new DenseIndexFile;
    int successCounter = 0;
    while (successCounter < 10000){
        auto keyValue = RandomDataGenerator::generateRandomKey(successCounter);
        auto dataValue = RandomDataGenerator::generateRandomData(successCounter);
        auto result = testFile->add(keyValue, dataValue);
        if(result){
            successCounter++;
        }
    }
    return 0;
}
