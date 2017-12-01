#pragma once

#include "Queue.h"

#ifndef EXTENDED_QUEUE_H
#define EXTENDED_QUEUE_H


class Extended_queue :public Queue {
public:
	int size() const;
	void clear();
	Error_code serve_and_retrieve(Queue_entry &item);
    Error_code appendToTop(const Queue_entry &item);
};

#endif
