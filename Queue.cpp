
#include "stdafx.h"

Queue::Queue() {
	front = rear = nullptr;
}


bool Queue::empty() const {
	return (front == nullptr) && (rear == nullptr);
}


Error_code Queue::append(const Queue_entry &item) {
	Node *new_rear = new Node(item);
	if (new_rear == nullptr) return overflow;
	if (rear == nullptr) front = rear = new_rear;
	else {
		rear->next = new_rear;
		rear = new_rear;
	}
	return success;
}


Queue::Queue(const Queue &original) {
	front = original.front;
	rear = original.rear;
}


Queue::~Queue() {
	while (!empty()) {
		serve();
	}
}

Error_code Queue::serve() {
	if (front == NULL) return underflow;
	Node *old_front = front;
	front = old_front->next;
	if (front == NULL) rear = NULL;
	delete old_front;
	return success;
}


Error_code Queue::retrieve(Queue_entry &item) const {
	if (front == NULL) return underflow;
	item = front->entry;
	return success;
}


void Queue::operator = (const Queue &original) {
	Queue new_copy(original);
	front = new_copy.front;
	rear = new_copy.rear;
}



