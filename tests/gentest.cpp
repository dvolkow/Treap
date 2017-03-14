#include <random>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <climits>
#include <string>
#include <iostream>

using namespace std;

//#define PARALLEL_ 1
//#define SERIAL_ 1

#ifdef PARALLEL_ 
	#include <pthread.h> 
	pthread_mutex_t mutex;
#endif		

ofstream fout;
const char* out_name_file = {"input.txt"}; //--название выходного файла
//--варианты операций, дублирование для повышения частоты выпадения вставок:
std::vector<std::string> OPERATIONS = {"delete", "exists", "next", "prev", "insert", "insert"}; 
const size_t n = 5; //--диапазон выпадаемых индексов, в нашем случае не больше 6

#ifdef PARALLEL_ 

struct util
{
	int OPERATION_COUNT;
	int RANGE;
	mt19937 gn;
};


void * run_1(void* arg)
{
	std::vector<int> args, idx;
	util consts = *(util*)arg;
	args.reserve(consts.OPERATION_COUNT / 2 + 1);
	idx.reserve(consts.OPERATION_COUNT / 2 + 1);

	for (int i = 0; i < consts.OPERATION_COUNT / 2; ++i)
	{
		idx.push_back(consts.gn() % n);
		args.push_back(static_cast<int>(consts.gn() % consts.RANGE) - 
					static_cast<int>(consts.gn() % ((3 * consts.RANGE) / 2)));
	}
#ifdef DEBUG 
	cout << "First calculate!\n";
#endif		

    pthread_mutex_lock(&mutex);
#ifdef DEBUG 
	cout << "First output...\n";
#endif		
#ifdef WRITE_RES 
	for (int i = 0; i < consts.OPERATION_COUNT / 2; ++i)
	{
		fout << OPERATIONS[idx[i]] << " " << args[i] << "\n";
	}
#endif		
#ifdef DEBUG 
	cout << "First done!\n";
#endif		
    pthread_mutex_unlock(&mutex);
    // return pointer to result
    pthread_exit(NULL);
}

void * run_2(void* arg)
{
	std::vector<int> args, idx;
	util consts = *(util*)arg;
	args.reserve(consts.OPERATION_COUNT / 2 + 1);
	idx.reserve(consts.OPERATION_COUNT / 2 + 1);

	for (int i = 0; i < consts.OPERATION_COUNT / 2; ++i)
	{
		idx.push_back(consts.gn() % n);
		args.push_back(static_cast<int>(consts.gn() % consts.RANGE) - 
					static_cast<int>(consts.gn() % ((3 * consts.RANGE) / 2)));
	}
#ifdef DEBUG 
	cout << "Second calculate!\n";
#endif		

    pthread_mutex_lock(&mutex);
#ifdef DEBUG 
	cout << "Second output...\n";
#endif		
#ifdef WRITE_RES 
	for (int i = 0; i < consts.OPERATION_COUNT / 2; ++i)
	{
		fout << OPERATIONS[idx[i]] << " " << args[i] << "\n";
	}
#endif		
#ifdef DEBUG 
	cout << "Second done!\n";
#endif		
    pthread_mutex_unlock(&mutex);
    // return pointer to result
    pthread_exit(NULL);
}
#endif		


int main(int argc, char const *argv[])
{
	const int OPERATION_COUNT = atoi(argv[2]); //--количество операций
	const int RANGE = atoi(argv[3]); //--размах принимаемых значений для элементов (дерева, в нашем случае)
	const int seed = atoi(argv[1]); // +  abs((2 * atoi(argv[1])) | (atoi(argv[1]) << 3));


#ifdef PARALLEL_ 
	static pthread_t first_runner, second_runner;
    //---атрибуты потока
    static pthread_attr_t attrs;
    //---инициализация атрибутов потока
    if (pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_JOINABLE))
    {
        perror("Error is setting attributes");
	}
    //---инициализация мьюекса
    pthread_mutex_init(&mutex, NULL);
#endif		

	std::cout << "Seed = " << seed << endl;

	mt19937 gen; 
	gen.seed(seed); 

	fout.open(out_name_file);

#ifdef PARALLEL_ 
	util consts = {OPERATION_COUNT, RANGE, gen};
#endif		
	
#ifdef SERIAL_ 
	for (int i = 0; i != OPERATION_COUNT; ++i)
	{
		fout << OPERATIONS[gen() % n] << " " << static_cast<int>(gen() % RANGE) - static_cast<int>(gen() % ((3 * RANGE) / 2)) << "\n";
	}
#endif		
#ifdef PARALLEL_ 
	//---порождение потоков
    if (pthread_create(&first_runner, &attrs, run_1, &consts))
    {
        perror("Cannot create thread!");
    }

    if (pthread_create(&second_runner, &attrs, run_2, &consts))
    {
        perror("Cannot create thread!");
    }

    //---ожидание завершения порожденных потоков
    pthread_join(first_runner, NULL);
    pthread_join(second_runner, NULL);

    //---освобождение ресурсов, занимаемых описателем атрибутов
    pthread_attr_destroy(&attrs);
    //---разрушение мьютекса и cond
    pthread_mutex_destroy(&mutex);
#endif		
}