#ifndef THREAD_MAKER
#define THREAD_MAKER

#include <thread>
#include <mutex>
#include <vector>
#include <assert.h>
#include <functional>
#include <chrono>
#include <memory>

template<typename T>
class thread_executer
{
public:
	virtual void execute(std::vector<T>& data) = 0;

protected:
	mutable std::mutex mut;
};


template<typename T>
class thread_maker
{
public:
	thread_maker(int thread_num = 2) : m_thread_num(thread_num) {};
	~thread_maker(){};

	void set_thread_data(std::vector<T>& thread_data) { m_thread_data = thread_data; };

	std::vector<std::vector<T> > create_threads(std::shared_ptr<thread_executer<T> > pexe)
	{
		std::vector<std::vector<T> > splited_datas;
		splite_thread_data(m_thread_num, splited_datas);
		auto f = std::bind(&thread_maker<T>::execute, this, std::placeholders::_1, std::placeholders::_2);
		for (int i = 0; i < m_thread_num; i++)
		{
			thread t(f, pexe, splited_datas[i]);
			m_threads.push_back(std::move(t));
		}
		return splited_datas;
	}

	void run_threads()
	{
		// 启动线程，等到线程执行完
		for (size_t i = 0; i < m_threads.size(); i++)
		{
			m_threads[i].join();
		}
	};

private:
	void splite_thread_data(int thread_num, std::vector<std::vector<T> >& splited_datas)
	{
		assert(thread_num >= 2);
		int average = m_thread_data.size() / thread_num;
		int start = 0;
		for (size_t i = 0; i < thread_num; i++)
		{
			int end = (i + 1) * average;
			if (thread_num - 1 == i) // 最后一个线程
				end = m_thread_data.size();

			splited_datas.push_back(std::vector<T>(m_thread_data.begin() + start, m_thread_data.begin() + end));
			start = end; 
		}
	}

	void execute(std::shared_ptr<thread_executer<T> > pexe, std::vector<T>& data)
	{
		pexe->execute(data);
	}

private:
	std::vector<std::thread> m_threads;
	std::vector<T> m_thread_data;
	int m_thread_num;
};



#endif