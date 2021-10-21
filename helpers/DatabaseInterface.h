#include "../file-classes/DenseIndexFile.h"

class DatabaseInterface {
public:
    static void addQuery(DenseIndexFile *file);

    static void deleteQuery(DenseIndexFile *file);

    static void editQuery(DenseIndexFile *file);

    static void getQuery(DenseIndexFile *file);

};
