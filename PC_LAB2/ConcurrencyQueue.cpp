/*#include "ConcurrencyQueue.h"
#include "Task.h"

template <class T>
ConcurrencyQueue<T>::ConcurrencyQueue()
{
	this->m_head = nullptr;
	this->m_tail = nullptr;
	this->m_size = 0;
}

template <class T>
ConcurrencyQueue<T>::~ConcurrencyQueue()
{

}

template <class T>
void ConcurrencyQueue<T>::push(T& data)
{
	unique_lock<shared_mutex> u_lock(this->m_rw_mutex);
	Node* temp = new Node(nullptr, this->m_tail, data);
}

template <class T>
T ConcurrencyQueue<T>::pop()
{

}

template <class T>
bool ConcurrencyQueue<T>::empty()
{
	shared_lock<shared_mutex> s_lock(this->m_rw_mutex);
	return this->m_size == 0 ? true : false;
}

template <class T>
void ConcurrencyQueue<T>::clear()
{

}

template <class T>
unsigned int ConcurrencyQueue<T>::get_size()
{
	shared_lock<shared_mutex> s_lock(this->m_rw_mutex);
	return this->m_size;
}

template class ConcurrencyQueue<Task>;*/