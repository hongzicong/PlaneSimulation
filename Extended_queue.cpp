#include "stdafx.h"

int Extended_queue::size() const
{
	Node *window = front;
	int count = 0;
	while (window != NULL) {
		window = window->next;
		count++;
	}
	return count;
}

void Extended_queue::clear(){
    while (!empty()) {
        Plane p;
        retrieve(p);
        p.refuse();
        serve();
    }
}

Error_code Extended_queue::appendToTop(const Queue_entry &item){
    Node *new_front = new Node(item);
    if (new_front == nullptr) return overflow;
    if (rear == nullptr) front = rear = new_front;
    else {
        Node *old_front=front;
        front=new_front;
        front->next=old_front;
    }
    return success;
}
