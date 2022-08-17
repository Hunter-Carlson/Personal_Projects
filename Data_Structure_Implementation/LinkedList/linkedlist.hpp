//Name: Hunter Carlson
//Class: Comp Sci 1575 Section: 101

#ifndef ARRAYLIST_HPP
#define ARRAYLIST_HPP

#include <iostream>
using namespace std;

template <typename T>
ostream& operator<< (ostream& out, const LinkedList<T>& list)
{
  LLNode<T>* tmp = list.m_head;
  out << "[ ";
  for(int i = 0; i < list.m_size; i++)
  {
    out << tmp->m_data << ", ";
    tmp = tmp->m_next;
  }
  out <<"]";
  return out;
}

template <typename T>
bool LinkedList<T>::isEmpty() const
{
  return (m_size == 0 && m_head->m_next == NULL);
}

template <typename T>
int LinkedList<T>::size() const
{
  return m_size;
}

template <typename T>
LLNode<T>* LinkedList<T>::getFirstPtr()
{
  if(isEmpty() == false)
    return m_head;
  else
    return NULL;
}

template <typename T>
LLNode<T>* LinkedList<T>::getAtPtr(int i)
{

  if(i <= m_size)
  {
    LLNode<T>* tmp = m_head;
    for(int j = 0; j < i; j++)
    {
      tmp = tmp->m_next;
    }
    return tmp;
  }

  else
    return NULL;

}

template <typename T>
void LinkedList<T>::insert_front(const T& x)
{
  LLNode<T>* tmp = new LLNode<T>(x,m_head);
  m_head = tmp;
  m_size++;
  return;
}

template <typename T>
void LinkedList<T>::insert(const T& x, LLNode<T>* pos)
{
  LLNode<T>* tmp = new LLNode<T>;
  *tmp = *pos;
  pos->m_data = x;
  pos->m_next = tmp;
  m_size++;
  return;
}

template <typename T>
void LinkedList<T>::insert_back(const T& x)
{
  LLNode<T>* tmp = new LLNode<T>;
  *tmp = *getAtPtr(m_size);
  getAtPtr(m_size)->m_data = x;
  getAtPtr(m_size)->m_next = tmp;
  m_size++;
  return;
}

template <typename T>
void LinkedList<T>::remove_front()
{
  LLNode<T>* tmp = m_head->m_next;
  *m_head = *tmp;
  delete tmp;
  m_size--;
  return;
}

template <typename T>
void LinkedList<T>::remove_back()
{
  LLNode<T>* tmp = getAtPtr(m_size - 1)->m_next;
  *(getAtPtr(m_size - 1)) = *tmp;
  delete tmp;
  m_size--;
  return;
}

template <typename T>
void LinkedList<T>::remove(LLNode<T>* pos)
{
  LLNode<T>* tmp = pos->m_next;
  *pos = *tmp;
  delete tmp;
  m_size--;
  return;
}



template <typename T>
void LinkedList<T>::clear()
{
  LLNode<T>* tmp = m_head;
  while(m_head->m_next != NULL)
  {
    m_head = tmp->m_next;
    delete tmp;
    tmp = m_head;
  }
  m_size = 0;
  return;
}

template <typename T>
LinkedList<T>::~LinkedList()
{
  clear();
  delete m_head;
}

template <typename T>
const LinkedList<T>& LinkedList<T>::operator= (const LinkedList<T>& rhs)
{
  clear();
  LLNode<T>* Right = rhs.m_head;
  while(Right->m_next != NULL)
  {
    insert_back(Right->m_data);
    Right = Right->m_next;
  }
  m_size = rhs.m_size;

  return *this;
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& rhs)
{
  m_head = new LLNode<T>;
  m_head->m_next = NULL;
  *this = rhs;
}

template <typename T>
bool LinkedList<T>::operator== (const LinkedList<T>& rhs) const
{
  LLNode<T>* Left = m_head;
  LLNode<T>* Right = rhs.m_head;

  if(m_size != rhs.m_size)
    return false;
  while(Left->m_next != NULL)
  {
    if(Left->m_data != Right->m_data)
      return false;
    Left = Left->m_next;
    Right = Right->m_next;
  }
  return true;
}

template <typename T>
void LinkedList<T>::reverse()
{
  T tmp;
  if(m_size > 1)
  {
    for(int i = (m_size - 2); i >= 0; i--)
    {
      tmp = getAtPtr(i)->m_data;
      remove(getAtPtr(i));
      insert_back(tmp);
    }
  }
  return;
}

template <typename T>
void LinkedList<T>::append(const LinkedList<T>& l2)
{
  LLNode<T>* tmp = l2.m_head;
  for(int i = 0; i < l2.m_size; i++)
  {
    insert_back(tmp->m_data);
    tmp = tmp->m_next;
  }

  return;
}

template <typename T>
void LinkedList<T>::slice(const LinkedList<T>& l2, LLNode<T>* start, LLNode<T>* stop)
{
  clear();
  while(start->m_next != stop->m_next)
  {
    insert_back(start->m_data);
    start = start->m_next;
  }
  return;
}

template <typename T>
LLNode<T>* LinkedList<T>::find(const T& x)
{
  LLNode<T>* tmp = m_head;
  while (tmp->m_next != NULL)
  {
    if ( tmp->m_data == x)
      return tmp;

    tmp = tmp->m_next;
  }

  return NULL;
}

#endif
