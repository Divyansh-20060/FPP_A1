#include <functional>
using namespace std;
#ifndef stamp
#define stamp

void execute_tuple(std::function<void()> &&lambda1, std::function<void()> &&lambda2);

void parallel_for(int low, int high, int stride, std::function<void(int)> &&lambda,
int numThreads);

void parallel_for(int high, std::function<void(int)> &&lambda, int numThreads);

void parallel_for(int low1, int high1, int stride1, int low2, int high2, int stride2,
std::function<void(int, int)> &&lambda, int numThreads);

void parallel_for(int high1, int high2, std::function<void(int, int)> &&lambda,
int numThreads);

#endif