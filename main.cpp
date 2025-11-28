#include <iostream>
#include <syncstream>
#include <chrono>
#include <future>
#include <thread>
#include <iomanip>

using namespace std;

void quick(const string& name) {
    auto t1 = chrono::steady_clock::now();
    this_thread::sleep_for(chrono::seconds(1));
    auto t2 = chrono::steady_clock::now();

    chrono::duration<double> dt = t2 - t1;

    osyncstream(cout)
        << "[" << fixed << setprecision(2) << dt.count() << "s] "
        << name << endl;
}

void slow(const string& name) {
    auto t1 = chrono::steady_clock::now();
    this_thread::sleep_for(chrono::seconds(7));
    auto t2 = chrono::steady_clock::now();

    chrono::duration<double> dt = t2 - t1;

    osyncstream(cout)
        << "[" << fixed << setprecision(2) << dt.count() << "s] "
        << name << endl;
}

void run_final_chain() {
    slow("B");
    quick("C1");
    quick("D1");
    quick("F");
}

void work() {
    auto start = chrono::high_resolution_clock::now();

    future<void> futA = async(launch::async, slow, "A");

    quick("C2");
    quick("D2");

    futA.wait();

    run_final_chain();

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;

    cout << "Time: " << fixed << setprecision(2)
         << elapsed.count() << " s" << endl;
}

int main() {
    work();
    cout << "Work is done!" << endl;
    return 0;
}
