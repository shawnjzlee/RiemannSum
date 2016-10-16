#include <iostream>

#include "riemann.h"
#include "global.h"

using namespace std;

Riemann::Riemann() { }

Riemann::~Riemann() { }

void Riemann::thread_data_init(int _num_threads, bool _can_share) {
    num_threads = _num_threads;
    can_share = _can_share;
}

double Riemann::func(double value) {
    return value * value;
}

bool Riemann::get_sharing_condition(vector<Riemann> &thread_data_vector) {
    if(thread_data_vector[stolen_thread_id].can_share) {
        for (stolen_thread_id = 0; stolen_thread_id < num_threads; stolen_thread_id++)
        {
            if(stolen_thread_id == thread_id) { continue; }
            lock_guard<mutex> lock(*(mutex_map.at(stolen_thread_id)));
            if((thread_data_vector[stolen_thread_id].curr_location  < (thread_data_vector[stolen_thread_id].parts / 2)) 
               && thread_data_vector[stolen_thread_id].is_shared != 1)
            {
                thread_data_vector[stolen_thread_id].is_shared = 1;
                stolen_parts = thread_data_vector[stolen_thread_id].parts;
                thread_data_vector[stolen_thread_id].parts /= 2;
                stolen_curr_location = thread_data_vector[stolen_thread_id].parts;
                cout << "Thread " << thread_id << " is stealing from " << stolen_thread_id << endl;
                return true;
            }
            cout << "Thread " << stolen_thread_id << " not stolen because " 
                 << thread_data_vector[stolen_thread_id].curr_location << " > " 
                 << thread_data_vector[stolen_thread_id].parts / 2 << endl;
        }
        return false;
    }
    return false;
}

void Riemann::callback(vector<Riemann> &thread_data_vector) {
    double local_lbound = thread_data_vector[stolen_thread_id].lbound + stolen_curr_location * width;

    while(stolen_curr_location != stolen_parts) {
        local_sum += func(local_lbound) * width;
        local_lbound += width;
        stolen_curr_location += 1;
    }
}

void Riemann::do_work() {
    double local_lbound = lbound;
    (*(mutex_map.at(thread_id))).lock();
    int local_parts = parts;
    (*(mutex_map.at(thread_id))).unlock();
    
    for (int i = 0; i < local_parts; i++) {
        {
            lock_guard<mutex> lock(*(mutex_map.at(thread_id)));
            if (is_shared) {
                local_parts = parts;
                if (i == local_parts) continue;
            }
            curr_location = i;
        }
        
        local_sum += func(local_lbound) * width;
        local_lbound += width;
    }
}

short Riemann::get_thread_id() const {
    return thread_id;
}

void Riemann::set_thread_id(int thread_id) {
    this->thread_id = thread_id;
}

int Riemann::get_lbound() const {
    return lbound;
}

int Riemann::get_rbound() const {
    return rbound;
}

void Riemann::set_bounds(double lbound, double rbound) {
    this->lbound = lbound;
    this->rbound = rbound;
}

int Riemann::get_working_partitions() const {
    return parts;
}

void Riemann::set_working_partitions(int parts) {
    this->parts = parts;
}

int Riemann::get_curr_location() const {
    return curr_location;
}

void Riemann::set_curr_location(int curr_location) {
    this->curr_location = curr_location;
}

double Riemann::get_width() const {
    return width;
}

void Riemann::set_width(double width) {
    this->width = width;
}

double Riemann::get_local_sum() const {
    return local_sum;
}