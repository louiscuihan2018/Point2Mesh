#include "clock.h"
#include <iostream>
#include <chrono>

using namespace std;

namespace CGL {

    void Timer::reset() {
        t_times.clear();
        t_last = c.now();
        t_times.push_back(t_last);
    }

    double Timer::stop(bool output) {
        Tp tmp = c.now();
        Dr period = tmp - t_last;
        double duration = chrono::duration_cast<chrono::nanoseconds>(period).count() / 1000000;
        if (output) cout << "Time: " << duration << " ms" << endl;
        t_last = tmp;
        t_times.push_back(t_last);
        return duration;
    }

    list<double> Timer::get_all_stops() {
        list<double> res;
        Tp last;
        auto it = t_times.begin();
        
        if (it != t_times.end()) {
            last = *it;
            it++;
        }
        
        int c = 0;
        for (; it != t_times.end(); it++, c++) {
            double duration = chrono::duration_cast<chrono::nanoseconds>(*it - last).count() / 1000000;
            res.push_back(duration);
            last = *it;
        }
        
        return res;
        
    }

    void Timer::output_all_stops() {
        Tp last;
        auto it = t_times.begin();

        if (it != t_times.end()) {
            last = *it;
            it++;
        }

        int c = 0;
        for (; it != t_times.end(); it++, c++) {
            cout << "Period " << c << " : ";
            double duration = chrono::duration_cast<chrono::nanoseconds>(*it - last).count() / 1000000;
            cout << duration << " ms" << endl;
            last = *it;
        }
    }
}
