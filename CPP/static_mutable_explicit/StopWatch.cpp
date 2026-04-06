#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

class Stopwatch {
    int seconds;                		// Поточний час у секундах
    bool paused = false;                		// Чи на паузі
    mutable int queryCount;     			// Кількість викликів GetSeconds/GetMinutes
    time_t startingTime;
    static int stopwatchCount;      			// Загальна кількість створених секундомірів
    static int activeStopwatches;   			// Кількість секундомірів, що зараз "тікають"

public:
    explicit Stopwatch(int sec) : seconds(sec), queryCount(0) {
        activeStopwatches++;
        stopwatchCount++;
        startingTime = time(nullptr);
    };       			// Ініціалізація секундами
    explicit Stopwatch(double min) : queryCount(0) {
        seconds = ((int)(min * 60));
        activeStopwatches++;
        stopwatchCount++;
        startingTime = time(nullptr);
    };    			// Ініціалізація хвилинами
    ~Stopwatch() {
        if (this->isPaused() == false) {
            activeStopwatches--;
        }
        stopwatchCount--;
    };
    bool isPaused() const {
        return paused;
    }
    int getSeconds() const{
        queryCount++;
        return seconds;
    }
    int getMinutes() const{
        queryCount++;
        return ((int)(seconds / 60));
    }
    void Tick() {
        if (!paused) {
            seconds = (int)(time(nullptr) - startingTime);
        }
    };                   		// Оновлюе значення seconds залежно вiд значення що повертае std::time(nullptr), якщо не на паузі

    void pause(bool pauseState = true) {
        if (paused != pauseState) {
            paused = pauseState;
            if (paused)
                activeStopwatches--;
            else
                activeStopwatches++;
        }
    };         // Ставит секундомір на паузу або знімає паузу
};
int Stopwatch::activeStopwatches = 0;
int Stopwatch::stopwatchCount = 0;
int main() {
    Stopwatch sw1(10);
    Stopwatch sw2(1.5);
    cout << "Initial values:" << endl;
    cout << "sw1 seconds: " << sw1.getSeconds() << endl;
    cout << "sw2 minutes: " << sw2.getMinutes() << endl;
    cout << "\nWaiting 3 seconds...\n";
    Sleep(3000);
    sw1.Tick();
    sw2.Tick();
    cout << "\nAfter Tick:" << endl;
    cout << "sw1 seconds: " << sw1.getSeconds() << endl;
    cout << "sw2 seconds: " << sw2.getSeconds() << endl;
    sw1.pause();
    cout << "\nsw1 paused.\n";
    cout << "Waiting 2 seconds...\n";
    Sleep(2000);
    sw1.Tick();
    sw2.Tick();
    cout << "\nAfter pause and Tick:" << endl;
    cout << "sw1 seconds (should not change): " << sw1.getSeconds() << endl;
    cout << "sw2 seconds (should change): " << sw2.getSeconds() << endl;
    sw1.pause(false);
    cout << "\nsw1 resumed.\n";
    cout << "Waiting 2 seconds...\n";
    Sleep(2000);
    sw1.Tick();
    cout << "sw1 seconds after resume: " << sw1.getSeconds() << endl;

    return 0;
}