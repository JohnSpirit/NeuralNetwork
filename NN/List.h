#pragma once
//2019.6 rw:2020.6 Copyright@JSY-2020.
#include <iostream>
using namespace std;

typedef struct
{
	int x;
	int y;
}COOR;

template<typename datatype>
struct Node
{
	datatype data;
	struct Node* next;
};

template<typename T>
class List
{
protected:
	int length = 0;
	struct Node<T>* head = NULL;
	struct Node<T>* tail = NULL;
	struct Node<T>* ptr = NULL;
public:
	List(T data)
	{
		tail = head = new struct Node<T>;
		head->data = data;
		tail->next = NULL;
		length++;
	}
	List(void)
	{
		tail = head = new struct Node<T>;
		tail->next = NULL;
		length++;
	}
	~List()
	{
		ptr = head;
		struct Node<T>* nextptr = head;
		while (ptr != NULL)
		{
			nextptr = nextptr->next;
			delete ptr;
			ptr = nextptr;
		}
	}
	void Append(T data)
	{
		length++;
		ptr = tail;
		tail = new struct Node<T>;
		tail->data = data;
		ptr->next = tail;
		tail->next = NULL;
	}
	void Append(void)
	{
		length++;
		ptr = tail;
		tail = new struct Node<T>;
		ptr->next = tail;
		tail->next = NULL;
	}
	void Insert(int pos = this->length + 1)
	{
		if (pos == length + 1)Append();
		else
		{
			ptr = head;
			if (pos < 1 || pos>length + 1) 
			{ 
				cerr<<"InsertOutOfRange\n";
				exit(EXIT_FAILURE);
			}
			for (int i = 1; i < pos - 1; i++)ptr = ptr->next;
			struct Node<T>* nextptr = ptr->next;
			ptr->next = new struct Node<T>;
			ptr->next->next = nextptr;
			length++;
		}
	}
	void Insert(T data, int pos = this->length + 1)
	{
		if (pos == length + 1)Append(data);
		else
		{
			ptr = head;
			if (pos < 1 || pos>length + 1) 
			{
				cerr << "InsertOutOfRange\n";
				exit(EXIT_FAILURE);
			}
			for (int i = 1; i < pos - 1; i++)ptr = ptr->next;
			struct Node<T>* nextptr = ptr->next;
			ptr->next = new struct Node<T>;
			ptr->next->data = data;
			ptr->next->next = nextptr;
			length++;
		}
	}
	void Delete(int pos = 1)
	{
		if (pos < 1 || pos>length)
		{
			cerr << "DeleteOutOfRange\n";
			exit(EXIT_FAILURE);
		}
		ptr = head;
		for (int i = 1; i < pos - 1; i++)ptr = ptr->next;
		struct Node<T>* nextptr = ptr->next->next;
		free(ptr->next);
		if (nextptr == NULL)ptr->next = NULL;
		else ptr->next = nextptr;
		length--;
	}
	void Print()
	{
		ptr = head;
		for (int i = 1; i <= length; i++)
		{
			printf("%d->", ptr->data);
			ptr = ptr->next;
		}
		printf("end\n");
	}
	int GetLength()
	{
		return this->length;
	}
	virtual void ReadToRef(T& readto, int pos = 0)
	{
		if (pos < 1 || pos>length)
		{
			cerr << "VisitOutOfRange\n";
			exit(EXIT_FAILURE);
		}
		ptr = head;
		for (int i = 1; i < pos; i++)ptr = ptr->next;
		readto = ptr->data;
	}
	virtual T* ReadToPtr(int pos = 0)
	{
		if (pos < 1 || pos>length)
		{
			cerr << "VisitOutOfRange\n";
			exit(EXIT_FAILURE);
		}
		ptr = head;
		for (int i = 1; i < pos; i++)ptr = ptr->next;
		return &ptr->data;
	}
	virtual T Read(int pos = 0)
	{
		if (pos < 1 || pos>length)
		{
			cerr << "VisitOutOfRange\n";
			exit(EXIT_FAILURE);
		}
		ptr = head;
		for (int i = 1; i < pos; i++)ptr = ptr->next;
		return ptr->data;
	}
	virtual void Write(T data, int pos = this->length)
	{
		if (pos < 1 || pos>length) 
		{
			cerr << "WriteOutOfRange\n";
			exit(EXIT_FAILURE);
		}
		ptr = head;
		for (int i = 1; i < pos; i++)ptr = ptr->next;
		ptr->data = data;
	}
	virtual void Write(T& data, int pos = this->length)
	{
		if (pos < 1 || pos>length)
		{
			cerr << "WriteOutOfRange\n";
			exit(EXIT_FAILURE);
		}
		ptr = head;
		for (int i = 1; i < pos; i++)ptr = ptr->next;
		ptr->data = data;
	}
	virtual void Write(T* data, int pos = this->length)
	{
		if (pos < 1 || pos>length)
		{
			cerr << "WriteOutOfRange\n";
			exit(EXIT_FAILURE);
		}
		ptr = head;
		for (int i = 1; i < pos; i++)ptr = ptr->next;
		ptr->data = *data;
	}
};

