#ifndef ATOMIC_H
#define ATOMIC_H

#include <iostream>       // std::cout
#include <atomic>         // std::atomic, std::atomic_flag,   ATOMIC_FLAG_INIT
#include <thread>         // std::thread, std::this_thread::yield
#include <vector>         // std::vector
#include <utility>
#include <sstream>        // std::stringstream
#include <assert.h>
#ifdef __cplusplus
extern "C" {
#endif

void AtomicCase1(void);
void AtomicCase2(void);
void AtomicCase3(void);
void AtomicCase4(void);
void AtomicCase5(void);
void AtomicCase6(void);
void AtomicCase7(void);
#ifdef __cplusplus
}
#endif

#endif
