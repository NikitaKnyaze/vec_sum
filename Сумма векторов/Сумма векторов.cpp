#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <iomanip>

void sumVectors(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result, int start, int end) {
   for (int i = start; i < end; ++i) {
      result[i] = a[i] + b[i];
   }
   int i = 0;
  
}

double parallelSumVectors(int numThreads, const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result) {
   std::vector<std::thread> threads(numThreads);
   int dataSize = a.size();
   int blockSize = dataSize / numThreads;

   auto startTime = std::chrono::steady_clock::now();

   for (int i = 0; i < numThreads; ++i) {
      int start = i * blockSize;
      int end{};
      if (i == (numThreads - 1))
      {
         end = dataSize;
      }
      else
      {
         end = start + blockSize;
      }

      threads[i] = std::thread(sumVectors, std::ref(a), std::ref(b), std::ref(result), start, end);
   }

   for (int i = 0; i < numThreads; ++i) {
      threads[i].join();
   }

   auto endTime = std::chrono::steady_clock::now();
   std::chrono::duration<double> elapsed = endTime - startTime;

   return elapsed.count();
}

int main() {
   SetConsoleCP(1251);
   SetConsoleOutputCP(1251);
   
   std::vector<int> sizes = { 1000, 10000, 100000, 1000000 };

   std::vector<int> threadCounts = { 2, 4, 8, 16 };

   unsigned int numCores = std::thread::hardware_concurrency();
   std::cout << "Количество аппаратных ядер: " << numCores << std::endl;

   for (int size : sizes) {
      std::cout <<  std::setw(16) << size;
   }
   std::cout << std::endl;

   for (int threads : threadCounts) {
      std::cout << threads << "\t";

      for (int size : sizes) {
         std::vector<int> a(size, 1); 
         std::vector<int> b(size, 2); 
         std::vector<int> result(size, 0); 

         double time = parallelSumVectors(threads, a, b, result);
         std::cout << std::setw(8) << time << "s\t";
      }

      std::cout << std::endl;
   }

   return 0;
}
