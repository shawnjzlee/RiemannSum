#ifndef RBARRIER_H
#define RBARRIER_H

#include <pthread.h>

using namespace std;

class RBarrier
{
    public:
        RBarrier();
        ~RBarrier();

        template <typename b_fn, typename v_fn>
        bool rbarrier_wait(const b_fn& condition,
                           const v_fn& callback);
                            
        int rbarrier_init(int num_threads);
        
        void barrier_rc (int rc);
                            
    private:
        pthread_barrier_t barrier;
};

#endif /* rbarrier.h */