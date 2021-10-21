#include "file-classes/DenseIndexFile.h"
#include "helpers/RandomDataGenerator.h"

int main() {
    auto testFile = new DenseIndexFile;
    testFile->outputLimits();
    return 0;
}
