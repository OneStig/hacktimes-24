#include "scoring.cpp"

#include <iostream>
#include <chrono>

int main() {
    Scoring scoring;
    string node_labels = "SPLERIALATNSPSEGERD";

    auto startTime = std::chrono::high_resolution_clock::now();
    int score = scoring.score(node_labels);
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


    cout << "Score: " << score << endl;
    return 0;
}
