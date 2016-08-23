#include <vector>
#include <algorithm>
#include <list>
#include <iostream>
#include <chrono>
#include <memory>
#include "thread_maker.h"

using namespace std;

void test_remove_if();
void test_thread();

int main()
{
	//test_remove_if();
	test_thread();
	return 1;
}

void test_remove_if()
{
	//int myints[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };	
	//int* pbegin = myints;
	//int* pend = myints + sizeof(myints) / sizeof(int);

	//pend = remove_if(pbegin, pend, [](int x) -> bool { return x % 2 == 1; });
	//cout << "result : " << endl;
	//for (int* p = pbegin; p != pend; ++p)
	//{
	//	cout << " " << *p;
	//}
	//cout << endl;

	//int myints[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	//vector<int> data(myints, myints + 10);
	//data.erase(remove_if(data.begin(), data.end(), [](int& x) -> bool {return x % 2 == 1; }), data.end());

	//for (auto iter = data.begin(); iter != data.end(); iter++)
	//{
	//	cout << *iter << " ";
	//}
	//cout << endl;


}


template<typename T>
class Accumulator : public thread_executer<T>
{
public:
	Accumulator(){};
	~Accumulator(){ cout << "~Accumulator" << endl; };

	void execute(std::vector<T>& data)
	{
		T sum = 0;
		for (int i = 0; i < data.size(); i ++)
		{
			sum += data[i];
		}
		std::lock_guard<std::mutex> lk(mut);
		cout << "sum : " << sum << endl;
	}

private:

};

template<typename T>
class Multipler : public thread_executer < T >
{
public:
	Multipler(){};
	~Multipler(){ cout << "~Multipler" << endl; };

	void execute(std::vector<T>& data)
	{
		T sum = 1;
		for (int i = 0; i < data.size(); i++)
		{
			sum *= data[i];
		}
		std::lock_guard<std::mutex> lk(mut);
		cout << "sum : " << sum << endl;
	}

private:

};


void test_thread()
{
	vector<int> data;
	int num = 19;
	for (int i = 0; i < num; i++)
	{
		data.push_back(i);
	}

	//shared_ptr<Accumulator<int>> accu = make_shared<Accumulator<int>>();
	shared_ptr<Multipler<int>> accu = make_shared<Multipler<int>>();
	thread_maker<int> maker(5);
	maker.set_thread_data(data);
	vector<vector<int> > splited_data = maker.create_threads(accu);
	maker.run_threads();
	cout << endl;
	for (int i = 0; i < splited_data.size(); i++)
	{
		for (int j = 0; j < splited_data[i].size(); j++)
		{
			cout << splited_data[i][j] << " ";
		}
		cout << endl;
	}
}