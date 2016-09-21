#include "riemann.h"
using namespace std;

ThreadData::ThreadData() { }

ThreadData::~ThreadData() { }

void ThreadData::thread_data_init(int _num_threads, bool _can_share) {
    num_threads = _num_threads;
    can_share = _can_share;
}

double ThreadData::func(double value) {
    return value * value;
}

bool ThreadData::get_sharing_condition(vector<ThreadData> &thread_data_vector) {
    if(thread_data_vector[stolen_thread_id].can_share) {
        for (stolen_thread_id = 0; stolen_thread_id < num_threads; stolen_thread_id++)
        {
            if(stolen_thread_id == thread_id) { continue; }
            thread_data_vector[stolen_thread_id].do_work_mutex.lock();
            if((thread_data_vector[stolen_thread_id].curr_location < (thread_data_vector[stolen_thread_id].parts / 2)) 
               && thread_data_vector[stolen_thread_id].is_shared != 1)
            {
                thread_data_vector[stolen_thread_id].is_shared = 1;
                stolen_parts = thread_data_vector[stolen_thread_id].parts;
                stolen_curr_location = (thread_data_vector[stolen_thread_id].parts / 2);
                thread_data_vector[stolen_thread_id].do_work_mutex.unlock();
                return true;
            }
            thread_data_vector[stolen_thread_id].do_work_mutex.unlock();
        }
        return false;
    }
    return false;
}

void ThreadData::callback(vector<ThreadData> &thread_data_vector) {
    double local_lbound = thread_data_vector[stolen_thread_id].lbound + stolen_curr_location * width;

    while(stolen_curr_location != stolen_parts) {
        local_sum += func(local_lbound) * width;
        local_lbound += width;
        stolen_curr_location += 1;
    }
}

void ThreadData::do_work() {
    double local_lbound = lbound;
    for (int i = 0; i < parts; i++) {
        do_work_mutex.lock();
        local_sum += func(local_lbound) * width;
        local_lbound += width;
        curr_location = i;
        do_work_mutex.unlock();
    }
}

short ThreadData::get_thread_id() const {
    return thread_id;
}

void ThreadData::set_thread_id(int thread_id) {
    this->thread_id = thread_id;
}

int ThreadData::get_lbound() const {
    return lbound;
}

int ThreadData::get_rbound() const {
    return rbound;
}

void ThreadData::set_bounds(double lbound, double rbound) {
    this->lbound = lbound;
    this->rbound = rbound;
}

int ThreadData::get_working_partitions() const {
    return parts;
}

void ThreadData::set_working_partitions(int parts) {
    this->parts = parts;
}

int ThreadData::get_curr_location() const {
    return curr_location;
}

void ThreadData::set_curr_location(int curr_location) {
    this->curr_location = curr_location;
}

double ThreadData::get_width() const {
    return width;
}

void ThreadData::set_width(double width) {
    this->width = width;
}

double ThreadData::get_local_sum() const {
    return local_sum;
}