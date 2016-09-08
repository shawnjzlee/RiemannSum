#ifndef RBARRIER_H
#define RBARRIER_H

using namespace std;

class rbarrier
{
    public:
        rbarrier();
        ~rbarrier();

        template <typename b_fn, typename v_fn>
        bool rbarrier_wait(const b_fn& condition,
                           const v_fn& callback);
                            
        int rbarrier_init(int num_threads);
                            
    private:
        pthread_barrier_t barrier;
};

#endif /* rbarrier.h */