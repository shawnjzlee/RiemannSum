#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>

#include <thread>
#include <chrono>
#include <cstdio>
#include <functional>
#include <algorithm>

#include <pthread.h>
#include "rbarrier.cpp"
#include "riemann.cpp"

using namespace std;
using namespace std::chrono;

rbarrier rbarrier;

void 
barrier_rc (int rc) {
    if (rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
    {
        cout << "Could not wait on barrier\n";
        exit(-1);
    }
}

void get_global_total (vector<thread_data> thread_data_vector) {
    double sum = 0.0;
    int num_threads = thread_data_vector.at(0).get_num_threads();
    for(int i = 0; i < num_threads; i++) {
        sum += thread_data_vector.at(i).get_local_sum();
    }
    cout << "The integral is: " << sum;
}

double thread_get_width (vector<thread_data> thread_data_vector, int index) {
    return thread_data_vector.at(index).get_rbound() - thread_data_vector.at(index).get_lbound();
}

void get_total (vector<thread_data> thread_data_vector, int index) {
    thread_data current_thread = thread_data_vector.at(index);
    short tid = 0;
    tid = current_thread.get_tid();
    
    high_resolution_clock::time_point start;
    if (tid == 0) 
        start = high_resolution_clock::now();
        
    current_thread.do_work();
    
    rbarrier.rbarrier_wait(
        [current_thread, thread_data_vector] (void)->bool {
            return current_thread.get_sharing_condition(thread_data_vector);
        } , 
        [current_thread, thread_data_vector] (void) {
            current_thread.callback(thread_data_vector);
        } );

    if (tid == 0) {
        high_resolution_clock::time_point end = high_resolution_clock::now();
        duration<double> runtime = duration_cast<duration<double>>(end - start);
        cout << "Work Time (in secs): " << runtime.count() << endl;
    }
}

int main(int argc, char * argv[]) {
    
    ifstream instream;
    int num_threads, l_bound, r_bound, partition_sz, rc, remaining_parts, index,
        i = 0, j = 0;
    double normal_dist = 0.0, init_dist = 0.0;
    
    if(argc != 5) {
        cout << "Not enough arguments.\n Requires [input file] "
             << "[number of threads] [dist_multiplier] [sharing flag].\n";
        return -1;
    }
    
    string input_file = argv[1];
    instream.open(input_file.c_str());
    if(!instream.is_open()) {
        cout << "Could not open file" << input_file << endl;
        return -1;
    }
    
    instream >> l_bound >> r_bound >> partition_sz;
    
    if(atoi(argv[2]) == 0) num_threads = 1;
    // else if(atoi(argv[2]) > thread::hardware_concurrency()) {
    //     cout << "num_threads set to maximum supported concurrent threads"
    //          << " (" << thread::hardware_concurrency() << ") \n";
    //     num_threads = thread::hardware_concurrency() - 1;
    // }
    else num_threads = atoi(argv[2]);
    
    if(num_threads > partition_sz) num_threads = partition_sz;
    
    const int dist_multiplier = atoi(argv[3]);
    const bool sharing_flag = atoi(argv[4]);
    
    const double width = (r_bound - l_bound) / (double)partition_sz;
    
    rc = rbarrier.rbarrier_init(num_threads);
    barrier_rc(rc);
    
    vector<thread_data> thread_data_vector(num_threads);
    for(int i = 0; i < num_threads; ++i) {
        thread_data_vector.at(i).thread_data_init(num_threads, sharing_flag);
    }
    
    vector<thread> threads(num_threads);
    
    if (partition_sz  % (num_threads + dist_multiplier))
        remaining_parts = partition_sz  % (num_threads - 1);
 
    if (dist_multiplier == 0) normal_dist = partition_sz / num_threads;
    else {
        init_dist = dist_multiplier * (partition_sz / (num_threads + dist_multiplier));
        normal_dist = (partition_sz - init_dist) / (num_threads - 1);
    }
    double ext_dist = normal_dist + 1;
    int num_norm_parts = partition_sz - remaining_parts;
    int num_ext_parts = partition_sz - num_norm_parts;
    
    if(num_threads == 1) {
        thread_data_vector[index].set_tid(index);
        thread_data_vector[index].set_lbound(l_bound + (width * normal_dist * index));
        thread_data_vector[index].set_rbound(l_bound + (width * normal_dist * (index + 1)));
        thread_data_vector[index].set_curr_location(l_bound + (width * normal_dist * index));
        thread_data_vector[index].set_working_partitions(normal_dist);
        thread_data_vector[index].set_width(width);
        
        high_resolution_clock::time_point start;
        start = high_resolution_clock::now();
        
        thread_data_vector[index].do_work();
        
        high_resolution_clock::time_point end = high_resolution_clock::now();
        duration<double> runtime = duration_cast<duration<double>>(end - start);
        cout << "Work Time (in secs): " << runtime.count() << endl;
    }
    else {
        for (i = 0; i < num_norm_parts, index < num_threads - remaining_parts; 
             index++)
        {
            thread_data_vector[index].set_tid(index);
            if (dist_multiplier && (i == 0)) {
                thread_data_vector[index].set_lbound(l_bound + (width * init_dist * index));
                thread_data_vector[index].set_rbound(l_bound + (width * init_dist * (index + 1)));
                thread_data_vector[index].set_curr_location(l_bound + (width * init_dist * index));
                thread_data_vector[index].set_working_partitions(init_dist);
                i += init_dist;
            }
            else if (dist_multiplier) {
                thread_data_vector[index].set_lbound(l_bound + (width * normal_dist * index) + (width * (init_dist - normal_dist)));
                thread_data_vector[index].set_rbound(l_bound + (width * normal_dist * (index + 1)) + (width * (init_dist - normal_dist)));
                thread_data_vector[index].set_curr_location(l_bound + (width * normal_dist * index) + (width * (init_dist - normal_dist)));
                thread_data_vector[index].set_working_partitions(normal_dist);
                i += normal_dist;
            }
            else {
                thread_data_vector[index].set_lbound(l_bound + (width * normal_dist * index));
                thread_data_vector[index].set_rbound(l_bound + (width * normal_dist * (index + 1)));
                thread_data_vector[index].set_curr_location(l_bound + (width * normal_dist * index));
                thread_data_vector[index].set_working_partitions(normal_dist);
                i += normal_dist;
            }
            thread_data_vector[index].set_width(width);
            threads[i] = thread(get_total, thread_data_vector, i);
        }
        for (j = 0; j < num_ext_parts; i += ext_dist, j++, index++)
        {
            thread_data_vector[index].set_tid(index);
            thread_data_vector[index].set_lbound(l_bound + (width * ext_dist * index) + (width * (init_dist - normal_dist)));
            thread_data_vector[index].set_rbound(l_bound + (width * ext_dist * (index + 1)) + (width * (init_dist - normal_dist)));
            thread_data_vector[index].set_curr_location(l_bound + (width * normal_dist * index) + (width * (init_dist - normal_dist)));
            thread_data_vector[index].set_working_partitions(ext_dist);
            thread_data_vector[index].set_width(width);
            threads[i] = thread(get_total, thread_data_vector, i);
        }
    }

    for_each (threads.begin(), threads.end(), mem_fn(&thread::join));

    get_global_total(thread_data_vector);
    
    return EXIT_SUCCESS;
}