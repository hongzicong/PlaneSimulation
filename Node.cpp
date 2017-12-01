#include "stdafx.h"

Node::Node() {

}

Node::Node(Node_entry item, Node *add_on) :entry(item), next(add_on) {

}