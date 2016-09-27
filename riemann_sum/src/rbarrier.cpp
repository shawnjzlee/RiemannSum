#include <iostream>
#include <pthread.h>

#include "rbarrier.h"

using namespace std;

RBarrier::RBarrier() { }

RBarrier::~RBarrier() {
    pthread_barrier_destroy(&barrier);
}

int RBarrier::rbarrier_init(int num_threads) {
    int rc = pthread_barrier_init(&barrier, NULL, num_threads);
    return rc;
}

template <typename b_fn, typename v_fn>
bool RBarrier::rbarrier_wait (const b_fn& condition,
                              const v_fn& callback) {
                         
    const bool result = condition ();
    
    if (result) { callback (); }
    pthread_barrier_wait (&barrier);
    
    return true;
}

void RBarrier::barrier_rc (int rc) {
    if (rc != 0 && rc != PTHREAD_BARRIER_SERIAL_THREAD)
    {
        cout << "Could not wait on barrier\n";
        exit(-1);
    }
}