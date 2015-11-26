// BinHeap.h
// minimum integer binary heap for type config
// this is created so that the configuration with the lowest score will bubble up to the
// top of the heap.

#ifndef BINHEAP1_H
#define BINHEAP1_H

#include "Config.h"

#include <iostream>

using namespace std;

class BinHeap
{
public:
    BinHeap(int);
    BinHeap(Config *x, int n);
    
    // int getMin();
    void insert(Config);
    Config getMin();
    Config *a;
    void dumpHeap();
    
private:
    int lastindx, asize;
    
    int parent(int);
    int leftChild(int);
    int rightChild(int);
    int smallestChild(int);
    void swap(Config&,Config&);
    void percUp(int);
    void percDown(int);
    void expandArray();
    
};

//CONSTRUCTORS

//Constructor: creates a binary heap of size n.
BinHeap::BinHeap(int n)
{
    a = new Config[n+1];
    asize = n+1;
    lastindx = 0;                   // empty means lastindx==0!!
}

//Creates a binary heap given an array and the size of that array.
BinHeap::BinHeap(Config *x, int n)
{
    asize = n+1;
    a = new Config[asize];
    lastindx = n;
    for (int i=0;i<n;i++)
        a[i+1] = x[i];
   // heapify();
}

//PRIVATE

//gets the parent of a node given a node.
int BinHeap::parent(int k)
{
    if(k<=2) {
        return 0;
    } else {
        if(k%2) {
            return (k/2);
        } else {
            return (k/2-1);
        }
    }
}

//gets the index of the left child of the given parent index.
int BinHeap::leftChild(int k) {
    return ((2*k)+1);
}

//gets the index of the right child of the given parent index.
int BinHeap::rightChild(int k) {
    return ((2*k)+2);
}

//gets the index of the minimum child of the given parent index.
int BinHeap::smallestChild(int k) {
    if(a[leftChild(k)].getScore() <= a[rightChild(k)].getScore()) {
        return leftChild(k);
    } else {
        return rightChild(k);
    }
}

//swaps node with another node.
void BinHeap::swap(Config &a, Config &b)
{
    Config tmp = a;
    a = b;
    b = tmp;
}

//Percalates a node with a minimum value up into the root of the heap.
void BinHeap::percUp(int k)
{
    if(a[k].getScore() < a[parent(k)].getScore()) {
        swap(a[k], a[parent(k)]);
        percUp(parent(k));
    }
}

//Percalates a node with larger value than parent down to the bottom of the heap.
void BinHeap::percDown(int k) {
    int sml = smallestChild(k);
    if(a[k].getScore() > a[sml].getScore() && sml<=lastindx) {
        swap(a[k], a[sml]);
        percDown(sml);
    }
}

//Doubles the size of the array when the array fills.
void BinHeap::expandArray() {
    Config *b = new Config[asize*2];
    for (int i=0; i<asize; i++) {
            b[i]=a[i];
    }
    asize *= 2;
    a=b;
}

//PUBLIC

//Inserts a node into the binary heap.
void BinHeap::insert(Config x) {
    if(lastindx >= asize) {
        expandArray();
    }
    a[lastindx] = x;
    percUp(lastindx);
    lastindx++;
    
}

//Removes the top node from the heap
Config BinHeap::getMin() {
    Config min = a[0];
    a[0] = a[lastindx-1];
    lastindx--;
    percDown(0);
    return min;
}

//Dumps the heap to the users screen in terms of score.
void BinHeap::dumpHeap() {
    for(int i=0; i<lastindx; i++) {
        cout << a[i].getScore() << " ";
    }
}

#endif