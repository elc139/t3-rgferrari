#include <algorithm>
#include <iostream>
#include <string>
#include <omp.h>

class SharedArray
{
private:
    char *array;
    int index;
    int size;
    bool usemutex;

public:
    SharedArray(int n, bool use) : size(n), index(0), usemutex(use)
    {
        array = new char[size];
        std::fill(array, array + size, '-');
    }
    ~SharedArray()
    {
        delete[] array;
    }
    void addChar(char c)
    {
        if (usemutex){
            #pragma omp ordered
            {
                array[index] = c;
                spendSomeTime();
                index++;
            }
        }
        else{
            array[index] = c;
            spendSomeTime();
            index++;
        }
            
    }
    int countOccurrences(char c)
    {
        return std::count(array, array + size, c);
    }
    std::string toString()
    {
        return std::string(array, size);
    }

private:
    void spendSomeTime()
    {
        for (int i = 0; i < 10000; i++)
        {
            for (int j = 0; j < 100; j++)
            {
                // These loops shouldn't be removed by the compiler
            }
        }
    }
};

class ArrayFiller
{
private:
    static const int nThreads = 3;
    static const int nTimes = 20;
    SharedArray *array;

public:
    ArrayFiller(bool usemutex)
    {
        array = new SharedArray(nThreads * nTimes, usemutex);
    }
    void fillArrayConcurrently()
    {
        omp_set_num_threads(nThreads);

        #pragma omp parallel for schedule(runtime) ordered
        for (int i = 0; i < nTimes * nThreads; i++)
        {
            array->addChar('A' + omp_get_thread_num());
        }
    }
    void printStats()
    {
        std::cout << array->toString() << std::endl;
        for (int i = 0; i < nThreads; ++i)
            std::cout << (char)('A' + i) << "="
                      << array->countOccurrences('A' + i) << " ";
        std::cout << std::endl;
    }
    ~ArrayFiller()
    {
        delete array;
    }
};

int main()
{
    std::cout << "Each thread should add its char to the array n times (n=20)" << std::endl;
    std::cout << "Correct results should total exactly nThreads*nTimes chars" << std::endl;

    std::cout << "Case 1: runtime with mutex" << std::endl;
    ArrayFiller m1(true);
    m1.fillArrayConcurrently();
    m1.printStats();
    
    std::cout << "Case 2: static (with no chunk specification) with mutex" << std::endl;
    ArrayFiller m2(true);
    omp_set_schedule(omp_sched_static, 0);
    m2.fillArrayConcurrently();
    m2.printStats();

    std::cout << "Case 3: static (with chunk specification) with mutex" << std::endl;
    ArrayFiller m3(true);
    omp_set_schedule(omp_sched_static, 8);
    m3.fillArrayConcurrently();
    m3.printStats();
    
    std::cout << "Case 4: dynamic (with no chunk specification) with mutex" << std::endl;
    ArrayFiller m4(true);
    omp_set_schedule(omp_sched_dynamic, 0);
    m4.fillArrayConcurrently();
    m4.printStats();

    std::cout << "Case 5: dynamic (with chunk specification) with mutex" << std::endl;
    ArrayFiller m5(true);
    omp_set_schedule(omp_sched_dynamic, 8);
    m5.fillArrayConcurrently();
    m5.printStats();

    std::cout << "Case 6: guided (with no chunk specification) with mutex" << std::endl;
    ArrayFiller m6(true);
    omp_set_schedule(omp_sched_guided, 0);
    m6.fillArrayConcurrently();
    m6.printStats();

    std::cout << "Case 7: guided (with chunk specification) with mutex" << std::endl;
    ArrayFiller m7(true);
    omp_set_schedule(omp_sched_guided, 8);
    m7.fillArrayConcurrently();
    m7.printStats();

    std::cout << "Case 8: auto with mutex" << std::endl;
    ArrayFiller m8(true);
    omp_set_schedule(omp_sched_auto, 0);
    m8.fillArrayConcurrently();
    m8.printStats();

    std::cout << "Case 9: static (with no chunk specification) (with no mutex)" << std::endl;
    ArrayFiller m9(false);
    omp_set_schedule(omp_sched_static, 0);
    m9.fillArrayConcurrently();
    m9.printStats();

    std::cout << "Case 10: static (with chunk specification) (with no mutex)" << std::endl;
    ArrayFiller m10(false);
    omp_set_schedule(omp_sched_static, 8);
    m10.fillArrayConcurrently();
    m10.printStats();
}
