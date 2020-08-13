#include <chrono>
#include <list>

using namespace std;

typedef chrono::high_resolution_clock Clock;
typedef chrono::steady_clock::time_point Tp;
typedef chrono::steady_clock::duration Dr;

namespace CGL {

    class Timer {

    private:
        Tp t_last;
        list<Tp> t_times;
        Clock c;


    public:
        Timer() {}
        
        void reset();

        double stop(bool output = true);

        list<double> get_all_stops();

        void output_all_stops();
    };
}
