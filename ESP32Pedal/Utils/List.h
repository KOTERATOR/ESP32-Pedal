#pragma once

#include <stddef.h>

template <class T, size_t N>
constexpr size_t GET_ARRAY_LENGTH(const T (&array)[N]) noexcept
{

	return N;
}

template <class T>
class ListNode
{
  public:
	T value;
	ListNode<T> *next;
	ListNode(T value, ListNode<T> *next)
	{
		this->value = value;
		this->next = next;
	}
};

template <class T>
class List
{
  private:
	ListNode<T> *root;
	size_t _size = 0;
	ListNode<T> *getNode(size_t index);

  public:
	List();
	List(T *array, size_t length);
	virtual ~List();
	size_t size();
	size_t length();
	size_t indexOf(T item);
	size_t lastIndexOf(T item);
	bool contains(T item);
	void add(T item);
	void add(T *array, size_t length);
	void add(List<T> list);
	void insert(size_t index, T item);
	void insert(size_t index, List<T> list);
	void insert(size_t index, T *array, size_t length);
	bool remove(T item);
	bool removeAt(size_t index);
	void removeRange(size_t index, size_t count);
	void clear();
	T &get(size_t index);
	T &operator[](size_t index);

	void foreach (void (*func)(T item));
	void foreach (void (*func)(size_t index, T item));
};

template <class T>
List<T>::List()
{
}

template <class T>
List<T>::List(T *array, size_t length)
{
	this->add(array, length);
}

template <class T>
List<T>::~List()
{
}

template <class T>
size_t List<T>::size()
{
	return _size;
}

template <class T>
size_t List<T>::length()
{
	return size();
}

template <class T>
ListNode<T> *List<T>::getNode(size_t index)
{
	if (index >= 0 && index < this->size())
	{
		ListNode<T> *temp = root;

		for (size_t i = 1; i <= index; i++)
		{
			temp = temp->next;
		}
		return temp;
	}
	else
	{
		return NULL;
	}
}

template <class T>
void List<T>::add(T item)
{
	this->insert(_size, item);
}

template <class T>
void List<T>::add(T *array, size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		this->add(array[i]);
	}
}

template <class T>
void List<T>::add(List<T> list)
{
	for (size_t i = 0; i < list.size(); i++)
	{
		this->add(list[i]);
	}
}

template <class T>
void List<T>::insert(size_t index, T item)
{
	if (index >= 0 && index <= _size)
	{
		ListNode<T> *indexNode = getNode(index);
		ListNode<T> *newNode = new ListNode<T>(item, NULL);
		if (index == 0)
		{
			root = newNode;
			root->next = indexNode;
		}
		else if (index == _size)
		{
			getNode(index - 1)->next = newNode;
		}
		else
		{
			ListNode<T> *prevIndexNode = getNode(index - 1);
			prevIndexNode->next = newNode;
			newNode->next = indexNode;
		}
		_size++;
	}
}

template <class T>
void List<T>::insert(size_t index, List<T> list)
{
	for (size_t i = 0; i < list.size(); i++)
	{
		this->insert(i + index, list[i]);
	}
}

template <class T>
void List<T>::insert(size_t index, T *array, size_t length)
{
	for (size_t i = 0; i < length; i++)
	{
		this->insert(i + index, array[i]);
	}
}

template <class T>
bool List<T>::remove(T item)
{
	return this->removeAt(this->indexOf(item));
}

template <class T>
bool List<T>::removeAt(size_t index)
{
	if (index >= 0 && index < _size)
	{
		ListNode<T> *indexNode = getNode(index);
		ListNode<T> *nextNode = indexNode->next;
		if (index == 0)
		{
			root = nextNode;
		}
		else
		{
			ListNode<T> *prevNode = getNode(index - 1);
			prevNode->next = nextNode;
		}
		_size--;
		return true;
	}
	else
	{
		return false;
	}
}

template <class T>
void List<T>::removeRange(size_t index, size_t count)
{
	for (size_t i = index; i < index + count; i++)
	{
		this->removeAt(index);
	}
}

template <class T>
void List<T>::clear()
{
	size_t sizeOfList = _size;
	for (size_t i = 0; i < sizeOfList; i++)
	{
		removeAt(0);
	}
}

template <class T>
size_t List<T>::indexOf(T item)
{
	for (size_t i = 0; i < this->size(); i++)
	{
		T itemAt = this->get(i);
		if (itemAt == item)
		{
			return i;
		}
	}
	return -1;
}

template <class T>
size_t List<T>::lastIndexOf(T item)
{
	for (size_t i = this->size() - 1; i >= 0; i--)
	{
		T itemAt = this->get(i);
		if (itemAt == item)
		{
			return i;
		}
	}
	return -1;
}

template <class T>
bool List<T>::contains(T item)
{
	return this->indexOf(item) >= 0;
}

template <class T>
T &List<T>::get(size_t index)
{
	ListNode<T> *node = getNode(index);
	return node->value;
}

template <class T>
T &List<T>::operator[](size_t index)
{
	return get(index);
}

template <class T>
void List<T>::foreach (void (*func)(T item))
{
	for (size_t i = 0; i < this->size(); i++)
	{
		func(this->get(i));
	}
}

template <class T>
void List<T>::foreach (void (*func)(size_t index, T item))
{
	for (size_t i = 0; i < this->size(); i++)
	{
		func(i, this->get(i));
	}
}
