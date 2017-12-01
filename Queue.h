#pragma once

#include "Node.h"

#ifndef QUEUE_H
#define QUEUE_H

enum Error_code {
	success, fail, range_error, underflow, overflow, fatal,
	not_present, duplicate_error, entry_inserted, entry_found,
	internal_error
};

class Queue {
public:
	typedef Node* index;
	typedef Plane Queue_entry;
	Queue();
    bool empty() const;
	Error_code append(const Queue_entry &item);
	Error_code serve();
	Error_code retrieve(Queue_entry &item) const;
	~Queue();
	Queue(const Queue &original);
    void operator = (const Queue &original);
	Node *front, *rear;
};

#endif
