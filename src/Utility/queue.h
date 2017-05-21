#ifndef QUEUE_h
#define QUEUE_h

#define MAX_SIZE 12
#include "Arduino.h"

template <class T>
class queue
{
  T A[MAX_SIZE];
  int fr = 0;
  int rear = 0;
  public:
  	// Constructor - set front and rear as -1.
  	// We are assuming that for an empty Queue, both front and rear will be -1.
  	queue()
  	{
  		fr = -1;
  		rear = -1;
  	}

  	// To check wheter Queue is empty or not
  	bool empty()
  	{
  		return (fr == -1 && rear == -1);
  	}

  	// To check whether Queue is full or not
  	bool full()
  	{
  		return (rear+1)%MAX_SIZE == fr ? true : false;
  	}

  	// Inserts an element in queue at rear end
  	void push(T x)
  	{
  		if(full())
  		{
  			return;
  		}
  		if (empty())
  		{
  			fr = rear = 0;
  		}
  		else
  		{
  		    rear = (rear+1)%MAX_SIZE;
  		}
  		A[rear] = x;
      Serial.println(String("push ") + fr + " " + rear);
  	}

  	// Removes an element in Queue from front end.
  	void pop()
  	{
      Serial.println("pop");
  		if(empty())
  		{
  			return;
  		}
  		else if(fr == rear )
  		{
  			rear = fr = -1;
  		}
  		else
  		{
  			fr = (fr+1)%MAX_SIZE;
  		}
  	}
  	// Returns element at front of queue.
  	T front()
  	{
  		if(fr == -1)
  		{
  			return nullptr;
  		}
      Serial.println(String("front ") + fr + " " + rear);
  		return A[fr];
  	}

};

#endif
