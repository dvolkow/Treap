#include <random>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <climits>
#include <string>
#include <iostream>


const char* out_name_file = {"input.txt"}; //--название выходного файла
//--варианты операций, дублирование для повышения частоты выпадения вставок:
std::vector<std::string> v = {"delete", "exists", "next", "prev", "insert", "insert"}; 

using namespace std;

int main(int argc, char const *argv[])
{
	const size_t n = 5; //--диапазон выпадаемых индексов, в нашем случае не больше 6
	const int m = atoi(argv[2]); //--количество операций
	const int s = atoi(argv[3]); //--размах принимаемых значений для элементов (дерева, в нашем случае)
	const int seed = atoi(argv[1]); // +  abs((2 * atoi(argv[1])) | (atoi(argv[1]) << 3));

	std::cout << "Seed = " << seed << endl;

	mt19937 gen; 
	gen.seed(seed); 

	ofstream fout;
	fout.open(out_name_file);
	
	for (int i = 0; i != m; ++i)
	{
		fout << v[gen() % n] << " " << static_cast<int>(gen() % s) - static_cast<int>(gen() % ((3 * s) / 2)) << "\n";
	}
}