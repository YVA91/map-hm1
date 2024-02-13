
#include <iostream>
#include <thread> 
#include <execution>
#include <random>
#include <chrono>
#include <Windows.h>
#include <locale>
#include <vector>
#include <condition_variable>


std::once_flag flag;


void summVector(std::vector<int>& v1, std::vector<int>& v2, std::vector<int>& vSum, int size) {

	auto start = std::chrono::steady_clock::now();

	for (int i = 0; i < size; i++) {
		vSum[i] = v1[i] + v2[i];
	}

	auto end = std::chrono::steady_clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	double time = static_cast<double>(diff) / 1000000;
	std::cout << "\t" << std::fixed << time << "s\t";
}


void randomVector(std::vector<int> &v, int size) {

	std::mt19937 gen;
	std::uniform_int_distribution<int> dist(0, size);
	std::generate(std::execution::par, v.begin(), v.end(), [&]() {
		return dist(gen);
		});
}

void print_cores_and_size() {
	std::cout << "Колличество аппаратных ядер - " << std::thread::hardware_concurrency() << std::endl;
	std::cout << "\n";
	std::cout << "\t";

	for (size_t i = 1000; i <= 1000000; i *= 10)
	{
		std::cout << "\t" << i << "\t";
	}
	std::cout << "\n";
}


void start_thread(std::vector<int>& v1, std::vector<int>& v2, std::vector<int>& vSum, int size) {

		std::thread th1(summVector, std::ref(v1), std::ref(v2), std::ref(vSum), size);
		if (th1.joinable()) {
			th1.join();
		
		
	}

}

int main()
{
  
	setlocale(LC_ALL, "Russian");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	
	for (int i = 1; i <= 16; i *= 2) {

		std::call_once(flag, print_cores_and_size);
		std::cout << i << " потоков ";

		for (int i = 1000; i <= 1000000; i *= 10) {


			size_t size = i;
			std::vector<int> v1(size), v2(size), vSum(size);
			randomVector(v1, size);
			randomVector(v2, size);
			start_thread(v1, v2, vSum, size);

		}
		std::cout << std::endl;
	}

}
