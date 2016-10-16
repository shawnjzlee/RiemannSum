#ifndef RBARRIER_H
#define RBARRIER_H

#include <functional>
#include <pthread.h>

using namespace std;

class RBarrier
{
    public:
        RBarrier();
        ~RBarrier();

        // template <typename b_fn, typename v_fn>
        bool rbarrier_wait(function<bool ()> const &,
                           function<void ()> const &);
                            
        int rbarrier_init(int num_threads);
        
        void barrier_rc (int rc);
                            
    private:
        pthread_barrier_t barrier;
};

#endif /* rbarrier.h */