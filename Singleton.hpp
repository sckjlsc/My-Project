//#ifndef SINGLETON_H
//#define SINGLETON_H
//
//#include <mutex>
//static std::mutex m;
//#include <iostream>
//
//template <typename T>
//class Singleton  
//{
//public:
//    static T * GetInstance();
//	static void release();
//protected:
//    static T* instance;
//	static bool destroyed;
//	Singleton();
//	~Singleton();
//
//private:
//    Singleton(Singleton const&){}
//    Singleton& operator=(Singleton const&){}
//};
//
//template<typename T>
//T* Singleton<T>::instance = NULL;
//
//template<typename T>
//bool Singleton<T>::destroyed = false;
//
//template<typename T>
//Singleton<T>::Singleton()
//{
//	std::cout << "singleton create!" << endl;
//}
//
//template<typename T>
//Singleton<T>::~Singleton()
//{
//	destroyed = true;
//}
//
//template<typename T>
//T* Singleton<T>::GetInstance()
//{
//    if(instance == NULL)
//    {
//		std::unique_lock<std::mutex> lock(m);
//		if (instance == NULL)
//		{
//			instance = new T();
//		}   
//    }
//    return instance;
//}
//
//template<typename T>
//void Singleton<T>::release()
//{
//	if (instance != NULL)
//	{
//		delete instance;
//		instance = NULL;
//	}
//}
//
//#endif

/******************************************************************
 lazy init
 the singletons will be destroyed at the end of the program in the reverse order in which they are created, 
 so there can be dependency issues.
 C++03 doesn't guarantee against race conditions in multithreaded environments.
 *******************************************************************/

#ifndef SINGLETON_H
#define SINGLETON_H

#include <mutex>
#include <iostream>

template <typename T>
class Singleton
{
	mutable std::once_flag flag;
public:
	static T * GetInstance();
protected:
	Singleton();
	~Singleton();

private:
	Singleton(Singleton const&) = delete;
	Singleton& operator=(Singleton const&) = delete;
};

template<typename T>
Singleton<T>::Singleton()
{
	std::cout << "singleton create!" << endl;
}

template<typename T>
Singleton<T>::~Singleton()
{
	std::cout << "singleton destroy!" << endl;
}

template<typename T>
T* Singleton<T>::GetInstance()
{
	static T instance;
	return &instance;
}

#endif