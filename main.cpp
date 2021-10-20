#include <iostream>
#include "file-classes/DenseIndexFile.h"
#include "helpers/RandomDataGenerator.h"

int main() {
    auto testFile = new DenseIndexFile;
    auto random = new RandomDataGenerator;
    auto result = testFile->get(18341254);
    cout << result;
    /*for (int i = 0; i < 5000; ++i) {
        auto keyValue = random->generateRandomKey(i);
        auto dataValue = random->generateRandomData(i);
        testFile->add(keyValue, dataValue);
    }*/
    return 0;
}
