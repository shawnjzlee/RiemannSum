#ifndef RIEMANN_H
#define RIEMANN_H

using namespace std;

class ThreadData {
    public:
        ThreadData();
        ~ThreadData();
        
        void thread_data_init(int, bool);
        
        double func(double value);
        bool get_sharing_condition(vector<ThreadData> &);
        void callback(vector<ThreadData> &);
        void do_work();
        
        short get_thread_id() const;
        void set_thread_id(int);
        
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
        mutable mutex do_work_mutex;
    
        short thread_id;                    /* Stores thread_id */
        int num_threads;                    /* Stores the number of threads */
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
        bool is_shared;                     /* Flags the condition of the thread */
        bool can_share;                     /* Passed in as cmd line argument; enables/disables sharing */
};

#endif /* riemann.h */