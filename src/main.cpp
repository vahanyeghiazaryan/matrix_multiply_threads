#include <iostream>
#include <fstream>
#include <cassert>
#include <thread>
#include <mutex>

void read_file(const std::string input, double** a, const int n)
{
    assert (a);
    try
    {
        std::ifstream file(input);
        if (!file.is_open()) {
            throw("");
        }
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (! (file >> a[i][j])) {
                    throw ("");
                }
            }
        }
        file.close();
    }
    catch(...){
        std::cout << "Can't read from file " << input << std::endl;
        exit (1);
    }
}

void write_file (std::string output, double** m, int n)
{
    try
    {
        std::ofstream file(output);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                file << m[i][j] << "\n";
            }
        }
    }
    catch(...){
        std::cout << "Can't write to file " << output << std::endl;
        exit (1);
    }
}
void mult (double** m1, double** m2, double** m3, int n, int& t, std::mutex& mtx)
{
    int i = 0;
    while (1) {
        mtx.lock();
        i = t / n;
        int j = t % n;
        ++t;
        mtx.unlock();
        if (i >= n) {
            return;
        }
        for (int l = 0; l < n; ++l) {
            m3[i][j] += m1[i][l] * m2[l][j];
        }
    }
}
int main(int argc, char * argv[])
{
    if (argc != 4) {
        std::cout << "The program needs 3 arguments.\n";
        return 1;
    }
    int n = 4;
    int threads = atoi(argv[1]);
    if (threads <= 0 || threads > n*n) {
        std::cout << "The first argument should be in range [1," << n*n
            << "]\n";
        return 1;
    }
    std::string input1 = argv[2];
    std::string input2 = argv[3];
    std::string output = "./output.txt";
    double** matrix1 = new double* [n];
    double** matrix2 = new double* [n];
    double** matrix3 = new double* [n];
    for (int i = 0; i < n; ++i) {
        matrix1[i] = new double[n];
        matrix2[i] = new double[n];
        matrix3[i] = new double[n]{};
    }
    std::thread t1 (read_file, input1, matrix1, n);
    std::thread t2 (read_file, input2, matrix2, n);
    std::thread t3[threads];
    t1.join();
    t2.join();
    int t = 0;
    std::mutex mtx;
    for (int i = 0; i < threads; ++i) {
        t3[i] = std::thread(mult, matrix1, matrix2, matrix3, n, std::ref(t), std::ref(mtx));
    }
    for (int i = 0; i < threads; ++i) {
        t3[i].join();
    }
    write_file (output, matrix3, n);
    return 0;
}
