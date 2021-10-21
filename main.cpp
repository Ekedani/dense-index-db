#include "file-classes/DenseIndexFile.h"
#include "helpers/RandomDataGenerator.h"
#include "helpers/DatabaseInterface.h"

int main() {
    auto denseIndexFile= new DenseIndexFile;
    while(true){
        char choice;
        cout << "Input A for add, G for get, E for, D for delete and Q for quit: "; cin >> choice;
        switch(choice){
            case 'A':
                DatabaseInterface::addQuery(denseIndexFile);
                break;
            case 'G':
                DatabaseInterface::getQuery(denseIndexFile);
                break;
            case 'E':
                DatabaseInterface::editQuery(denseIndexFile);
                break;
            case 'D':
                DatabaseInterface::deleteQuery(denseIndexFile);
                break;
            default:
                return 0;
        }
        cin.clear();
    }
}
