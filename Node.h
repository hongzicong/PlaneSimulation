#pragma once

#include "Plane.h"

#ifndef NODE_H
#define NODE_H


struct Node {

	using Node_entry=Plane;
	Node_entry entry;
	Node *next;

	Node();
	Node(Node_entry item, Node *add_on = nullptr);

};

#endif