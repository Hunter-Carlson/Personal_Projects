//Name: Hunter Carlson
//Class: Comp Sci 1575 Section: 101

#ifndef ARRAYLIST_HPP
#define ARRAYLIST_HPP

#include <iostream>
#include "arraylist.h"
using namespace std;

template <typename T>
void ArrayList<T>::grow()
{
  T* tmp = new T[(m_max * 2)];

  for(int i=0; i < m_size; i++)
    tmp[i]=m_data[i];

  delete [] m_data;
  m_data = tmp;
  m_max = m_max * 2;
  return;
}

template <typename T>
void ArrayList<T>::shrink()
{
  T* tmp = new T[(m_max / 2)];

  for(int i=0; i < m_size; i++)
    tmp[i]=m_data[i];

  delete [] m_data;
  m_data = tmp;
  m_max = m_max / 2;
  return;
}

template <typename T>
ArrayList<T>::ArrayList(int s, const T& x)
{
  m_size = s;
  m_max = s * 2;
  m_data = new T[m_max];

  for(int i = 0; i < m_size; i++)
    m_data[i] = x;
}

template <typename T>
int ArrayList<T>::size() const
{
  return m_size;
}

template <typename T>
const T& ArrayList<T>::first() const
{
  if (m_size > 0)
    return m_data[0];

  else
  {
    cout<<"!-- ERROR : PANIC in ARRAYLIST.first()!!  (List is empty)"<<endl;
    return m_errobj;
  }
}

template <typename T>
void ArrayList<T>::clear()
{
  delete [] m_data;
  m_data = new T[4];
  m_max = 4;
  m_size = 0;

  return;
}

template <typename T>
ArrayList<T>::~ArrayList()
{
  delete [] m_data;
  m_data = NULL;
  m_max = 0;
  m_size = 0;
}

template <typename T>
ArrayList<T>& ArrayList<T>::operator=(const ArrayList<T>& rhs)
{
  delete [] m_data;
  m_max = rhs.m_max;
  m_size = rhs.m_size;
  m_data = new T[m_max];

  for(int i = 0; i < m_size; i++)
    m_data[i] = rhs.m_data[i];

  return *this;
}

template <typename T>
ArrayList<T>::ArrayList(const ArrayList<T>& cpy)
{
  m_data = NULL;
  *this = cpy;
}

template <typename T>
void ArrayList<T>::insert(const T& x, int i)
{
  if(i > m_size || i < 0)
  {
    cout<<"!-- ERROR : PANIC in ARRAYLIST!!.insert()  (index out of bounds)"<<endl;
    return;
  }

  for(int j = m_size; j > i; j--)
    m_data[j] = m_data[j-1];

  m_data[i] = x;
  m_size++;
  if(m_size == m_max)
    grow();

  return;
}

template <typename T>
void ArrayList<T>::insert_back(const T& x)
{
  m_data[m_size] = x;
  m_size++;

  if(m_size == m_max)
    grow();

  return;
}

template <typename T>
void ArrayList<T>::remove(int i)
{
  if(i >= m_size || i < 0)
  {
    cout<<"!-- ERROR : PANIC in ARRAYLIST!!.remove()  (index out of bounds)"<<endl;
    return;
  }

  for(int j = i; j < m_size; j++)
    m_data[j] = m_data[j+1];

  m_size--;

  if(m_size < (m_max / 4))
    shrink();

  return;
}

template <typename T>
int ArrayList<T>::search(const T& x) const
{
  int position = -1;

  for(int i = 0; i < m_size; i++)
    if(m_data[i] == x)
    {
      position = i;
      i = m_size - 1;
    }

  return position;
}

template <typename T>
T& ArrayList<T>::operator[](int i)
{
  if(i < m_size && i >= 0)
    return m_data[i];

  cout<<"!-- ERROR : PANIC in ARRAYLIST!!.[]  (index out of bounds)"<<endl;
  return m_errobj;
}

template <typename T>
const T& ArrayList<T>::operator[](int i) const
{
  if(i < m_size && i >= 0)
    return m_data[i];

  cout<<"!-- ERROR : PANIC in ARRAYLIST!!.[]  (index out of bounds)"<<endl;
  return m_errobj;
}

template <typename T>
void ArrayList<T>::swap(int i, int k)
{
  if((i >= m_size) || (k >= m_size) || (i < 0) || (k < 0))
  {
    cout<<"!-- ERROR : PANIC in ARRAYLIST!!.swap()  (index out of bounds)"<<endl;
    return;
  }

  T tmp = m_data[i];
  m_data[i] = m_data[k];
  m_data[k] = tmp;

  return;
}

template <typename T>
void ArrayList<T>::purge()
{
  for(int i = 0; i < m_size; i++)
    for(int j = i + 1; j < m_size; j++)
      while(m_data[i] == m_data[j] && j < m_size)
        remove(j);
  return;
}

template <typename T>
void ArrayList<T>::reverse()
{
  T* tmp = new T[m_max];
  int i = 0;

  for(int j = m_size - 1; j >= 0; j--)
  {
    tmp[i] = m_data[j];
    i++;
  }

  delete [] m_data;
  m_data = tmp;

  return;
}

#endif
