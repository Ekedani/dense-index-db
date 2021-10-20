#include <random>
#include <ctime>
#include <string>

class RandomDataGenerator {
public:
    static unsigned int generateRandomKey(int num);

    static std::string generateRandomData(int num);
};

