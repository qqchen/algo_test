#include <vector>
#include <algorithm>
#include <list>
#include <iostream>
#include <chrono>
#include <memory>
#include "thread_maker.h"
//#include "pipe.h"
#include "cJSON.h"
#include <glog/logging.h>

using namespace std;

void test_remove_if();
void test_thread();
void test_cjson();
void test_glog(const char* argv);

// OBJECT from TEMPLATE function!
// Template as first-class citizen
#define define_functor_type(func_name) class tfn_##func_name {\
public: template <typename... Args> auto operator()(Args&&... args) const ->decltype(func_name(std::forward<Args>(args)...))\
{ return func_name(std::forward<Args>(args)...); } }

//#define make_globle_functor(NAME,F) auto NAME=define_functor_type(F)

int add(int a, int b)
{
	return a + b;
}

int add_one(int a)
{
	return 1 + a;
}

//make_globle_functor(tfn_add,add);

define_functor_type(add);
define_functor_type(add_one);

int test(tfn_add add_functor, int a, int b)
{
	return add_functor(a, b) + 1;
}

int main(int argc, char* argv[])
{
	//test_remove_if();
	//test_thread();
	//tfn_add add_functor;
	//tfn_add_one add_one_functor;
	//cout << "add_functor " << add_functor(1, 2) << endl; //result is 3
	//cout << "add_one_functor " << add_one_functor(5) << endl;
	//cout << "test " << test(add_functor ,5, 5) << endl;

	//test_cjson();

	test_glog(argv[0]);

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

void test_cjson()
{

}

void test_glog(const char* argv)
{
	google::InitGoogleLogging(argv);
	//FLAGS_stderrthreshold = google::INFO;
	FLAGS_colorlogtostderr = true;
	google::SetLogDestination(google::GLOG_INFO, "./myInfo");
	for (int i = 1; i <= 100; i++)
	{
		LOG_IF(WARNING, i == 100) << "LOG_IF(INFO,i==100)  google::COUNTER=" << google::COUNTER << "  i=" << i;
		//LOG_EVERY_N(INFO, 10) << "LOG_EVERY_N(INFO,10)  google::COUNTER=" << google::COUNTER << "  i=" << i;
		//LOG_IF_EVERY_N(WARNING, (i > 50), 10) << "LOG_IF_EVERY_N(INFO,(i>50),10)  google::COUNTER=" << google::COUNTER << "  i=" << i;
		//LOG_FIRST_N(ERROR, 5) << "LOG_FIRST_N(INFO,5)  google::COUNTER=" << google::COUNTER << "  i=" << i;
		LOG(INFO) << "COUNTER i = " << i << endl;
	}
	google::ShutdownGoogleLogging();
}