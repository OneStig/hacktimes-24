#include "scoring.cpp"

#include <iostream>
#include <string>
#include <chrono>
#include <execution>
#include <random>

std::string mutateString(const std::string& input) {
    if (input.length() != 19) {
        throw std::invalid_argument("Input string must be exactly 19 characters long.");
    }

    static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static const size_t maxIndex = sizeof(charset) - 2;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> indexDis(0, 18);
    static std::uniform_int_distribution<> charDis(0, maxIndex);

    size_t randomIndex = indexDis(gen);
    char newChar = charset[charDis(gen)];

    std::string mutatedString = input;
    mutatedString[randomIndex] = newChar;

    return mutatedString;
}

void simulatedAnnealing(std::pair<int, std::string> &best, Scoring& sc, double initialTemp, double finalTemp, double alpha, int maxIterations) {
    std::string current = best.second;
    int currentScore = best.first;

    double currentTemp = initialTemp;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int iter = 0; iter < maxIterations && currentTemp > finalTemp; ++iter) {
        std::string neighbor = mutateString(current);
        int neighborScore = sc.score(neighbor);
        double delta = neighborScore - currentScore;

        if (delta > 0 || std::exp(delta / currentTemp) > dis(gen)) {
            current = neighbor;
            currentScore = neighborScore;
            if (currentScore > best.first) {
                best.first = currentScore;
                best.second = current;
            }
        }

        currentTemp *= alpha;
    }
}

int main() {
    const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const size_t alphabetSize = sizeof(alphabet) - 2;

    Scoring scoring;

    std::vector<std::pair<int, std::string>> data(10000, {0, "ABCDEFGHIJKLMNOPQRS"});

    for (auto& [_, str] : data) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, alphabetSize);
        for (int i = 0; i < 19; i++) {
            str[i] = alphabet[dis(gen)];
        }
    }

    double initialTemp = 1000.0; // Initial temperature
    double finalTemp = 0.1; // Final temperature
    double alpha = 0.999; // Temperature reduction factor
    int maxIterations = 100000; // Maximum number of iterations

    auto startTime = std::chrono::high_resolution_clock::now();

    // Start operation here

    std::for_each(std::execution::par, data.begin(), data.end(),
    [&scoring, &initialTemp, &finalTemp, &alpha, &maxIterations](auto &cur) {
        cur.first = scoring.score(cur.second);
        simulatedAnnealing(cur, scoring, initialTemp, finalTemp, alpha, maxIterations);
    });

    // string node_labels = "SPLERIALATNSPSEGERD";
    // int score = scoring.score(node_labels);

    // End operation here

    auto endTime = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
    long long timeDuration = duration.count();

    long long minutes = timeDuration / 60000000;
    long long seconds = (timeDuration % 60000000) / 1000000;
    double milliseconds = timeDuration / 1000.0;

    // Print the duration in milliseconds with 3 decimal places
    cout << fixed << setprecision(3) << "Time taken: " <<
        minutes << ":" << seconds << ":" <<
        milliseconds << " ms" << endl;


    auto bestAns = std::max_element(data.begin(), data.end());
    std::cout << bestAns->first << ' ' << bestAns->second << std::endl;
    return 0;
}
