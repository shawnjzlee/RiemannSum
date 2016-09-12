#ifndef RIEMANN_H
#define RIEMANN_H

#include <mutex>
#include <vector>
#include <unistd.h>
#include <chrono>

using namespace std;

class thread_data {
    public:
        thread_data();
        ~thread_data();
        
        thread_data(thread_data &&);
        thread_data(const thread_data &);
        
        void thread_data_init(int _num_threads, bool _sharing_flag);
        
        double func(double value);
        bool get_sharing_condition(vector<thread_data> thread_data_vector);
        void callback(vector<thread_data> thread_data_vector);
        void do_work();
        
        void set_tid(int _index);
        short get_tid();
        int get_num_threads();
        double get_lbound();
        void set_lbound(double _lbound);
        double get_rbound();
        void set_rbound(double _rbound);
        void set_width(double _width);
        double get_width();
        void update_sum(double local_lbound);
        double get_local_sum();
        void set_curr_location(int location);
        int get_curr_location();
        void set_working_partitions(int _parts);
        int get_working_partitions();
        
    private:
        mutable mutex do_work_mutex;
    
        short thread_id;                    /* Stores thread_id */
        int num_threads;                    /* Stores the number of threads */
        double lbound;                      /* Stores global left bound */
        double rbound;                      /* Stores global right bound */
        
        int stolen_index;
        int stolen_location;
        int stolen_parts;
        
        double width;
        
        double local_sum;                   /* Stores local sum */
        int curr_location;                  /* Tracks the thread's current
                                               working location */
        int parts;                          /* Total number of partitions the thread
                                               is working in */
        
        bool cond;                          /* Flags the condition of the thread */
        bool sharing_flag;                /* Passed in as cmd line argument; enables/disables sharing */
        
        char buffer[200];                   /* Buffer that prevents false sharing on the cache line */
    
};

#endif /* riemann.h */