// SigletonTemp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
template <typename T> class Singleton
{
private:
	Singleton(const Singleton<T>&){};
	Singleton& operator=(const Singleton<T>&){};
protected:
	static T* _mInstance;
	static int i; // for debugging purposes
	Singleton()
	{
		i++; // for debugging purposes
		_mInstance = this;
		// this function never called?
		std::cout << "created" << std::endl;
	};
public:
	static T& getSingleton()
	{
		return *_mInstance;
	}
	static T* getSingletonPtr()
	{
		return _mInstance;
	}
};
template <typename T> T* Singleton<T>::_mInstance;
template <typename T> int Singleton<T>::i = 0; // for debugging purposes
class Orange : public Singleton < Orange >
{
public:
	void squeeze()
	{
		std::cout << "squeeze" << i << std::endl;
	}
};
class Apple : public Singleton < Apple >
{
public:
	void cut()
	{
		std::cout << "cut" << i << std::endl;
	}
};
int main()
{
	Orange::getSingletonPtr()->squeeze();
	Apple::getSingleton().cut();

	return 0;
}