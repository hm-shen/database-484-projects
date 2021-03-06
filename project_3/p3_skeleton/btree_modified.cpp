#include "btree.h"
#include "bnode.h"
#include "bnode_inner.h"
#include "bnode_leaf.h"

#include <cassert>

using namespace std;

const int LEAF_ORDER = BTREE_LEAF_SIZE/2;
const int INNER_ORDER = (BTREE_FANOUT-1)/2;

Btree::Btree() : root(new Bnode_leaf), size(0) {
    // Fill in here if needed
}

Btree::~Btree() {
    // Don't forget to deallocate memory
}

bool Btree::insert(VALUETYPE value) {
    // TODO: Implement this
    return true;
}

bool Btree::remove(VALUETYPE value) {
    // TODO: Implement this
    return true;
}

vector<Data*> Btree::search_range(VALUETYPE begin, VALUETYPE end) {
    std::vector<Data*> returnValues;
    // TODO: Implement this
    assert(root);
    Bnode_leaf* leaf;
    int *idx = NULL;
    leaf = this->search_larger(begin, idx);
    Data* temp_data = leaf->getData(*idx);

    while (temp_data->value <= end)
    {
        returnValues.push_back(temp_data);
        temp_data = this->getNext(leaf,idx);
    } 
    return returnValues;
}

//
// Given code
//
Data* Btree::search(VALUETYPE value) {
    assert(root);
    Bnode* current = root;

    // Have not reached a leaf node yet
    Bnode_inner* inner = dynamic_cast<Bnode_inner*>(current);
    // A dynamic cast <T> will return a nullptr if the given input is polymorphically a T
    //                    will return a upcasted pointer to a T* if given input is polymorphically a T
    while (inner) {
        int find_index = inner->find_value_gt(value);
        current = inner->getChild(find_index);
        inner = dynamic_cast<Bnode_inner*>(current);
    }

    // Found a leaf node
    Bnode_leaf* leaf = dynamic_cast<Bnode_leaf*>(current);
    assert(leaf);
    for (int i = 0; i < leaf->getNumValues(); ++i) {
        if (leaf->get(i) > value)    return nullptr; // passed the possible location
        if (leaf->get(i) == value)   return leaf->getData(i);
    }

    // reached past the possible values - not here
    return nullptr;
}

Bnode_leaf* Btree::search_larger(VALUETYPE value, int *out_idx) {
    assert(root);
    Bnode* current = root;

    // Have not reached a leaf node yet
    Bnode_inner* inner = dynamic_cast<Bnode_inner*>(current);
    // A dynamic cast <T> will return a nullptr if the given input is polymorphically a T
    //                    will return a upcasted pointer to a T* if given input is polymorphically a T
    while (inner) {
        int find_index = inner->find_value_gt(value);
        current = inner->getChild(find_index);
        inner = dynamic_cast<Bnode_inner*>(current);
    }

    // Found a leaf node
    Bnode_leaf* leaf = dynamic_cast<Bnode_leaf*>(current);
    assert(leaf);

    for (int i = 0; i < leaf->getNumValues(); ++i) 
    {
        if (leaf->get(i) >= value)   
        {
            *out_idx = i;
            return leaf;
        }
    }

    // reached past the possible values - not here
    std::cout<<"Cannot find it. Bug!"<<std::endl;
    return nullptr;
}

Data* Btree::getNext(Bnode_leaf* leaf, int* idx)
{
    assert((idx >= 0) && (idx < leaf.num_values));
    if ((idx == (leaf.num_values-1)) && (leaf->next != NULL))
    { 
        leaf = leaf->next;
        *idx = 0;
        return leaf->get(*idx);
    }
    else if (leaf->next != NULL)
    {
        *idx = *idx + 1;
        return leaf->get(*idx);
    }
}
