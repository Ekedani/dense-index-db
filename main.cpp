#include "file-classes/DenseIndexFile.h"
#include "helpers/RandomDataGenerator.h"

int main() {
    auto testFile = new DenseIndexFile;
    int keyToSearch;
    cout << "Input key to search: "; cin >> keyToSearch;
    testFile->get(keyToSearch);
    cout << testFile->getStatistics() << '\n';

    return 0;
}
