#include "riemann.h"
using namespace std;

thread_data::thread_data() { }

thread_data::~thread_data() { }

void thread_data::thread_data_init(int _num_threads, bool _sharing_flag) {
    num_threads = _num_threads;
    sharing_flag = _sharing_flag;
}

double thread_data::func(double value) {
    return value * value;
}

bool thread_data::get_sharing_condition(vector<thread_data> thread_data_vector) {
    if(thread_data_vector[stolen_index].sharing_flag) {
        for (stolen_index = 0; stolen_index < num_threads; stolen_index++)
        {
            if(stolen_index == thread_id) { continue; }
            thread_data_vector[stolen_index].do_work_mutex.lock();
            if((thread_data_vector[stolen_index].get_curr_location() < 
               (thread_data_vector[stolen_index].get_working_partitions() / 2)) 
               && thread_data_vector[stolen_index].cond != 1)
            {
                thread_data_vector[stolen_index].cond = 1;
                stolen_parts = thread_data_vector[stolen_index].get_working_partitions();
                thread_data_vector[stolen_index].set_working_partitions(parts / 2);
                stolen_location = thread_data_vector[stolen_index].get_working_partitions();
                thread_data_vector[stolen_index].do_work_mutex.unlock();
                return true;
            }
            thread_data_vector[stolen_index].do_work_mutex.unlock();
        }
        return false;
    }
    return false;
}

void thread_data::callback(vector<thread_data> thread_data_vector) {
    double local_lbound = thread_data_vector[stolen_index].lbound + stolen_location * width;

    while(stolen_location != stolen_parts) {
        update_sum(local_lbound);
        stolen_location += 1;
    }
}

void thread_data::do_work() {
    double local_lbound = lbound;
    for (int i = 0; i < parts; i++) {
        do_work_mutex.lock();
        update_sum(local_lbound);
        set_curr_location(i);
        do_work_mutex.unlock();
    }
}

void thread_data::set_tid(int _thread_id) {
    thread_id = _thread_id;
}

short thread_data::get_tid() {
    return thread_id;
}

int thread_data::get_num_threads() {
    return num_threads;
}

double thread_data::get_lbound() {
    return lbound;
}

void thread_data::set_lbound(double _lbound) {
    lbound = _lbound;
}

double thread_data::get_rbound() {
    return rbound;
}

void thread_data::set_rbound(double _rbound) {
    rbound = _rbound;
}

void thread_data::set_width(double _width) {
    width = _width;
}

double thread_data::get_width() {
    return width;
}

void thread_data::update_sum(double local_lbound) {
    local_sum += func(local_lbound) * width;
    local_lbound += width;
}

double thread_data::get_local_sum() {
    return local_sum;
}

void thread_data::set_curr_location(int location) {
    curr_location = location;
}

int thread_data::get_curr_location() {
    return curr_location;
}

void thread_data::set_working_partitions(int _parts) {
    parts = _parts;
}

int thread_data::get_working_partitions() {
    return parts;
}

