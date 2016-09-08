#include "rbarrier.h"
using namespace std;

rbarrier::rbarrier() { }

rbarrier::~rbarrier() {
    pthread_barrier_destroy(&barrier);
}

int rbarrier::rbarrier_init(int num_threads) {
    int rc = pthread_barrier_init(&barrier, NULL, num_threads);
    return rc;
}

template <typename b_fn, typename v_fn>
bool rbarrier::rbarrier_wait (const b_fn& condition,
                              const v_fn& callback) {
                         
    const bool result = condition ();
    
    if (result) { callback (); }
    pthread_barrier_wait (&barrier);
    
    return true;
}