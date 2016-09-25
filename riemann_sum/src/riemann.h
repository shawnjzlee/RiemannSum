#ifndef RIEMANN_H
#define RIEMANN_H

#include <vector>

#include "thread_data.h"
using namespace std;

class Riemann : public ThreadData {
    public:
        Riemann();
        ~Riemann();
        
        void thread_data_init(int, bool);
        short get_thread_id() const;
        void set_thread_id(int);
        
        double func(double value);
        
        bool get_sharing_condition(vector<Riemann> &);
        void callback(vector<Riemann> &);
        void do_work();
        
        int get_lbound() const;
        int get_rbound() const;
        void set_bounds(double, double);
        
        int get_working_partitions() const;
        void set_working_partitions(int);
        
        int get_curr_location() const;
        void set_curr_location(int);
        
        double get_width() const;
        void set_width(double);
        
        double get_local_sum() const;
        
    private:
        double lbound;                      /* Stores global left bound */
        double rbound;                      /* Stores global right bound */
        
        int stolen_thread_id;
        int stolen_curr_location;
        int stolen_parts;
        
        double width;
        
        double local_sum;                   /* Stores local sum */
        int curr_location;                  /* Tracks the thread's current
                                               working location */
        int parts;                          /* Total number of partitions the thread
                                               is working in */
};

#endif /* riemann.h */