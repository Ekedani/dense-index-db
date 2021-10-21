#include "DatabaseInterface.h"

void DatabaseInterface::addQuery(DenseIndexFile *file) {
    long long keyValue;
    cout << "Input your key: ";
    cin >> keyValue;
    if (keyValue >= 0 && keyValue <= INT_MAX) {
        string dataValue;
        cout << "Input your data: ";
        cin.ignore();
        getline(cin, dataValue);
        if (dataValue.empty()) {
            cout << "Error: Data can't be empty!\n";
            return;
        }
        auto addResult = file->add(keyValue, dataValue);
        if (addResult) {
            cout << "Success!\n";
        } else {
            cout << "Error: Key must be unique!\n";
        }
    } else {
        cout << "Error: Your key value is out of range!\n";
        return;
    }
}

void DatabaseInterface::getQuery(DenseIndexFile *file) {
    long long keyValue;
    cout << "Input your key: ";
    cin >> keyValue;
    if (keyValue >= 0 && keyValue <= INT_MAX) {
        auto getResult = file->get(keyValue);
        if (getResult.empty()) {
            cout << "Error: File doesn't contain this key!\n";
        } else {
            cout << "Search result: " << getResult << '\n';
        }
    } else {
        cout << "Error: Your key value is out of range!\n";
        return;
    }
}

void DatabaseInterface::editQuery(DenseIndexFile *file) {
    long long keyValue;
    cout << "Input your key: ";
    cin >> keyValue;
    if (keyValue >= 0 && keyValue <= INT_MAX) {
        string dataValue;
        cout << "Input your data: ";
        cin.ignore();
        getline(cin, dataValue);
        if (dataValue.empty()) {
            cout << "Error: Data can't be empty!\n";
            return;
        }
        auto editResult = file->edit(keyValue, dataValue);
        if (!editResult) {
            cout << "Error: File doesn't contain this key!\n";
        } else {
            cout << "Success!\n";
        }
    } else {
        cout << "Error: Your key value is out of range!\n";
        return;
    }
}

void DatabaseInterface::deleteQuery(DenseIndexFile *file) {
    long long keyValue;
    cout << "Input your key: ";
    cin >> keyValue;
    if (keyValue >= 0 && keyValue <= INT_MAX) {
        auto deleteResult = file->remove(keyValue);
        if (deleteResult) {
            cout << "Success!\n";
        } else {
            cout << "Error: File doesn't contain this key!\n";
        }

    } else {
        cout << "Error: Your key value is out of range!\n";
        return;
    }
}
