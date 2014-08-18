#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_
//////////////////////////////////////////////////////////////////////////////
//++
//
// LinkedList.cpp
//
// Description:
//	  Contains implementation of CLinkedList class.  CLinkedList is a generic
//    linked list class including methods for list manipulation.
//
// Copyright 2008 - Remote Sensing Solutions Inc.
//					Barnstable, MA
//
// Creation Author: Dan Robinson
// Creation Date:   06/17/2008
//
//--
//////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>

template <typename T>
class CLinkedList
{
private:
    struct node
    {
        T data;
        node* prev;
        node* next;
        node(T t, node* p, node* n) : data(t), prev(p), next(n) {}
    };
    node* head;
    node* tail;
    node* current;
    int	  size;
public:
    CLinkedList() 
    { 
    	head = NULL;
    	tail = NULL; 
    	size = 0;
    }

	// copy constructor
    CLinkedList( CLinkedList *pLinkedList ) 
    { 
    	head = NULL;
    	tail = NULL; 
    	size = 0;
    	
    	if ( pLinkedList != NULL )
    	{
	    	if (!(pLinkedList->empty()))
	    	{
	   			Append( pLinkedList->GetHeadData( ) );
	   			while (!( pLinkedList->EndOfList() ) )
	   			{
	   				Append( pLinkedList->GetNextData( ) );
	   			}
	    	}
    	}
    }

    bool empty() const { return ( !head || !tail ); }
    bool EndOfList() const { return current == tail; }
    operator bool() const { return !empty(); } 
    int  GetSize() const { return size; }
    void PointToHead() { current = head; }
    
    bool PointToNext()
    {
    	bool bRtn = true;
    	if ( empty()  || EndOfList() )
    	{
    		bRtn = false;
    	}
    	else
    	{
    		current = current->next;
    	}
    		
    	return bRtn;
    }
    
    void Append(T data)
	{
	    tail = new node(data, tail, NULL);
	    if( tail->prev )
	        tail->prev->next = tail;
	
	    if( empty() )
	    {
	        head = tail;
	        current = tail;
	    }
	    ++size;
	}

    void Prepend(T data)
	{
	    head = new node(data, NULL, head);
	    if( head->next )
	        head->next->prev = head;
	
	    if( empty() )
	    {
	        tail = head;
	        current = head;
		}
	    ++size;
	}
	
    void InsertBeforeCurrent(T data)
	{
		if (current == head) 
		{
			Prepend( data );
		}
		else
		{
			current = new node( data, current->prev, current );
			current->prev->next = current;
			current->next->prev = current;
		}
	    ++size;
	}

    T PopTail()
	{
	    if( empty() )
	        throw("CLinkedList : list empty");
	    node* temp(tail);
	    T data( tail->data );
	    tail = tail->prev ;
	
	    if( tail )
	    {
	        tail->next = NULL;
	        current = tail;
	    }
	    else
	    {
	        head = NULL ;
	        current = NULL;
	    }
	
	    --size;
	    delete temp;
	    return data;
	}
	
    T PopHead()
	{
	    if( empty() )
	        throw("CLinkedList : list empty");
	    node* temp(head);
	    T data( head->data );
	    head = head->next ;
	
	    if( head )
	    {
	        head->prev = NULL;
	        current = head;
	    }
	    else
	    {
	        tail = NULL;
	        current = NULL;
	    }
	
	    --size;
	    delete temp;
	    return data;
	}
	
    void RemoveCurrent()
	{
	    if( empty() )
	        throw("CLinkedList : list empty");
	    node* temp(current);

		if ( size == 1 )
		{
			head = NULL;
			tail = NULL;
			current = NULL;
		}
		else if ( current == head )
		{
			head = current->next;
			current->next->prev = NULL;
			current = head;
		}
		else if ( current == tail )
		{
			tail = current->prev;
			current->prev->next = NULL;
			current = tail;
		}
		else
		{
			current->prev->next = current->next;
			current->next->prev = current->prev;
			current = head;
		}
			
	    --size;
	    delete temp;
	}
	
    T GetNextData()
	{
	    if( current->next == NULL )
	        throw("CLinkedList : list overrun");
	        
	    current = current->next;
	
	    return current->data;
	}
	
	T GetCurrentData()
	{
	    if( empty() )
	        throw("CLinkedList : list empty");
	        
	    return current->data;
	}

    T GetHeadData()
	{
	    if( empty() )
	        throw("CLinkedList : list empty");
	        
	    current = head;
	    
	    return head->data;
	}
	
    T GetTailData()
	{
	    if( empty() )
	        throw("CLinkedList : list empty");
	        
	    current = tail;
	    
	    return tail->data;
	}

    ~CLinkedList()
    {
    	if ( !empty() )
    	{
	        while(head != NULL)
	        {
	            node* temp(head);
	            head=head->next;
	            delete temp;
	        }
    	}
    }
    
};


#endif /*LINKEDLIST_H_*/
