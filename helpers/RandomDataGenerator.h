#include <random>
#include <ctime>
#include <string>

class RandomDataGenerator {
public:
    unsigned int generateRandomKey(int num){
        std::default_random_engine generator(time(nullptr) + num);
        std::uniform_int_distribution<int> distribution(0,INT_MAX);
        return distribution(generator);
    }

    std::string generateRandomData(int num){
        srand(time(nullptr) + num);
        char symbol;
        std::string result;
        for (int i = 0; i < 100; ++i) {
            symbol = (rand() % 86) + 41;
            //Just to make sure that .csv file will be okay
            if(symbol == ','){
                symbol = '0';
            }
            result += symbol;
        }
        return result;
    }
};

