#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
#include "stack.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

/*
 * The constructor initializes all of the member variables needed for
 * an instance of the PQHeap class. The allocated capacity
 * is initialized to a starting constant and a dynamic array of that
 * size is allocated. The number of filled slots is initially zero.
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated]; // allocated zero'd memory
    _numFilled = 0;
}

/*
 * The destructor is responsible for cleaning up any resources
 * used by this instance of the PQArray class. The array
 * memory that was allocated for the PQArray is deleted here.
 */
PQHeap::~PQHeap() {
    delete [] _elements;
}

/* The enlarger function is used to enlarge the newCapacity twcie as large
 * as the previous capacity when the previous capacity is run out. And copy
 * the value in the previous array to the new array in the previous order.
 */

void PQHeap::enlarge(){
    DataPoint* newElements = new DataPoint[2 * _numAllocated];
    for(int i = 0; i < _numAllocated; i++){
        newElements[i] = _elements[i];
    }
    delete [] _elements;
    _elements = newElements;
    _numAllocated *= 2;
}


/*
 * The enqueue function is used to add a new element to the array.
 * If needed, it calls the helper function enlarger() to twice the capacity
 * of the previous.
 */
void PQHeap::enqueue(DataPoint elem) {
    if(_numFilled == 0){
        _elements[0] = elem;
        _numFilled++;
        return;
    }
    else if(_numFilled ==  _numAllocated){
        //create bigger array with twice capacity
        enlarge();
    }

    // add to bottom of array
    _elements[_numFilled] = elem;

    swapUp(elem,_numFilled);
    _numFilled++;


}


/*
 * The count of enqueued elements is tracked in the member variable _numFilled.
 */
int PQHeap::size() const {
    return _numFilled;
}


/*
 * This function returns the value of the frontmost element without removing
 * it from the queue.  Because the frontmost element was at the
 * last-filled index, decrementing filled count is sufficient to remove it.
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("PQueue is empty!");
    }
    return _elements[0];
}


/*
 * Since the array elements are stored in increasing sorted order by priority,
 * the frontmost element is located in the first filled slot of the array.
 *
 * This function returns the element at that index.
 * The first value of array then swap down recursively using `swapDown` with
 * its child if any child has smaller priority than itselfs.
 */
DataPoint PQHeap::dequeue() {
    if(_numFilled == 0){
        error("cannot dequeue empty pqheap");
    }

    DataPoint topPrior = _elements[0];

    // swap top and bottom
    _elements[0] = _elements[_numFilled - 1];
    _elements[_numFilled - 1] = topPrior;

    // swap top recursively to bottom
    _numFilled--;
    if (_numFilled > 1) {
        swapDown(_elements[0],0);
    }
    return topPrior;
}

/*
 * Returns true if no elements in the queue, false otherwise
 */
bool PQHeap::isEmpty() const {
    return size() == 0;
}


/*
 * Updates internal state to reflect that the queue is empty, e.g. count
 * of filled slots is reset to zero. The array memory stays allocated
 * at current capacity. The previously stored elements do not need to
 * be cleared; the slots would be overwritten when additional elements
 * are enqueued, but as a defensive programming technique, we mark
 * each with a sentinel value as a debugging aid.
 */
void PQHeap::clear() {
    for (int i = 0; i < size(); i++) {
        _elements[i] = { "CLEARED", -106 };
    }
    _numFilled = 0;
}

/*
 * Prints the contents of internal array for debugging purposes.
 */
void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
    for (int i = 0; i < size(); i++) {
        cout << "[" << i << "] = " << _elements[i] << endl;
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * should traverse the heap array and ensure that the heap property
 * holds for all elements in the array. If elements are found that
 * violate the heap property, an error should be thrown.
 */
void PQHeap::validateInternalState() const {
    // out-of-bound check
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    // if there are two or more element - enqueue
    if(_numFilled > 1){
        for (int i = 1; i < size(); i++) {
            int parentIndex = getParentIndex(i);
            if(_elements[i].priority < _elements[parentIndex].priority){
                printDebugInfo("parentIndex");
                printDebugInfo("i");
                error("Array has elements out of order at index " + integerToString(i));
            }
        }
    }
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the parent of the
 * specified child index. If this child has no parent, return
 * the sentinel value NONE.
 */
int PQHeap::getParentIndex(int child) const {
    return (child - 1)/2;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the left child of the
 * specified parent index. If this parent has no left child, return
 * the sentinel value NONE.
 */
int PQHeap::getLeftChildIndex(int parent) const {
    return 2*parent+1;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the right child of the
 * specified parent index. If this parent has no right child, return
 * the sentinel value NONE.
 */
int PQHeap::getRightChildIndex(int parent) const {
    return 2*parent+2;
}


/*
 * This function takes input of DataPoint and its current index, then swap it
 * upward if its parent's priority value is greater than current's value itself.
 */

void PQHeap::swapUp(DataPoint elem, int curIndex) {

    // Get parent index and elements
    int indexParent = getParentIndex(curIndex);
    DataPoint parent = _elements[indexParent];

    // base case
    // return if current element's priority value is greater than parent's
    if(parent.priority <= elem.priority){
        return;
    }

    // recursive case
    // swap if current element's priority value is smaller than parent's
    if (elem.priority < parent.priority){
        DataPoint temp = parent;
        _elements[indexParent] = elem;
        _elements[curIndex] = temp;
        swapUp(elem,indexParent);
    }
    else{
        return;
    }
}


/*
 * This function takes input of DataPoint and its current index, then
 * swap it downward if one of its childrens has priority value is smaller
 * than current's value itself.
 */
void PQHeap::swapDown(DataPoint elem, int curIndex) {
    int leftChildIndex = getLeftChildIndex(curIndex);
    int rightChildIndex = getRightChildIndex(curIndex);
    DataPoint chosen;
    int swapIndex;

    // base case 1: out-of-bound condition
    if(leftChildIndex >= _numFilled){
        return;
    }

    // only righChild out-of-bound
    else if(leftChildIndex == _numFilled - 1){
        chosen = _elements[leftChildIndex];
        swapIndex = leftChildIndex;
    }

    else {
        // choose the smaller child to swap
        if (_elements[leftChildIndex].priority >= _elements[rightChildIndex].priority) {
            chosen = _elements[rightChildIndex];
            swapIndex = rightChildIndex;
        }
        else if (_elements[leftChildIndex].priority < _elements[rightChildIndex].priority) {
            chosen = _elements[leftChildIndex];
            swapIndex = leftChildIndex;
        }
    }

    // base case 2: no smaller children
    if (chosen.priority >= elem.priority) {
        return;
    }
    // recursive case
    else if (chosen.priority < elem.priority) {

        DataPoint temp = _elements[curIndex];
        _elements[curIndex] = chosen;
        _elements[swapIndex] = temp;
        swapDown(elem, swapIndex);
    }
    else {
        return;
    }
}


/* * * * * * Function used for TEST * * * * * */
void pqSortHeap(Vector<DataPoint>& v) {
    PQHeap pq;

    /* Using the Priority Queue data structure as a tool to sort, neat! */

    /* Add all the elements to the priority queue. */
    for (int i = 0; i < v.size(); i++) {
        pq.enqueue(v[i]);
    }

    /* Extract all the elements from the priority queue. Due
     * to the priority queue property, we know that we will get
     * these elements in sorted order, in order of increasing priority
     * value. Store elements back into vector, now in sorted order.
     */
    for (int i = 0; !pq.isEmpty(); i++) {
        v[i] = pq.dequeue();
    }
}


/*
 * This function takes stream of data and int k as input.
 * It then returns the top k element with highest priority.
 *
 * Since we use the PQHeap, we will keep the array
 * of size k, if a new enqueue value is greater than the
 * top of the current queue (using peek), we will dequeue and
 * enqueue the new value. After finish reading the stream,
 * we put all of the to a vector using dequeue and
 * then reverse to get the top K.
 */
Vector<DataPoint> topKHeap(istream& stream, int k) {
    Vector<DataPoint> kTop = {};
    PQHeap pq;
    DataPoint cur;
    while (stream >> cur) {
        // if there is still space in cur
        if (pq.size() < k) {
            pq.enqueue(cur);
        }
        // compare incoming value
        else {
            DataPoint last = pq.peek();
            if (last.priority < cur.priority) {
                // remove min value
                pq.dequeue();
                // add new value
                pq.enqueue(cur);
            }
        }
    }
    // put queue to vector
    while (!pq.isEmpty()) {
        DataPoint current = pq.dequeue();
        kTop.add(current);
    }
    Stack<DataPoint> reverseTopk;
    while(!pq.isEmpty()){
        reverseTopk.push(pq.dequeue());
    }
    while (!reverseTopk.isEmpty()) {
        kTop.add(reverseTopk.pop());

    }

    return kTop;
}


/* Helper function that, given a list of data points, produces a stream from them. */
stringstream asStreamHeap(const Vector<DataPoint>& dataPoints) {
    stringstream result;
    for (const DataPoint& pt: dataPoints) {
        result << pt;
    }
    return result;
}

/* Helper function that, given range start and stop, produces a stream of values. */
stringstream asStreamHeap(int start, int stop) {
    stringstream result;
    for (int i = start; i <= stop; i++) {
        DataPoint pt = { "", i };
        result << " " << pt << " ";
    }
    return result;
}


/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("PQHeap filled size") {
    PQHeap pq;
    EXPECT_EQUAL(pq.size(), 0);
    pq.enqueue( { "Ama", -3 } );
    pq.enqueue( { "Lora", 10 } );
    pq.enqueue( { "Bella", 6 } );
    EXPECT_EQUAL(pq.size(), 3);
    pq.enqueue( { "Yamrda", 9 } );
    EXPECT_EQUAL(pq.size(), 4);
    pq.dequeue();
    pq.dequeue();
    pq.dequeue();
    EXPECT_EQUAL(pq.size(), 1);
}

STUDENT_TEST("PQHeap filled size") {
    PQHeap pq1;
    pq1.enqueue( { "5", 5 } );
    pq1.enqueue( { "17", 17 } );
    pq1.enqueue( { "3", 10 } );
    pq1.enqueue( { "10", 10 } );
    pq1.enqueue( { "9", 9 } );
    pq1.enqueue( { "15", 15 } );
    count << pq1<<endl;

}
STUDENT_TEST("PQHeap peek, always return smallest value") {
    PQHeap pq;
    EXPECT_ERROR(pq.peek());
    pq.enqueue( { "Ama", -3 } );
    EXPECT_EQUAL(pq.peek().priority, -3);
    pq.enqueue( { "Lora", 10 } );
    EXPECT_EQUAL(pq.peek().priority, -3);
    pq.enqueue( { "Keora", -4 } );
    EXPECT_EQUAL(pq.peek().priority, -4);
    pq.enqueue( { "Jony", 0 } );
    EXPECT_EQUAL(pq.peek().priority, -4);
    pq.enqueue( { "Kim", 99 } );
    EXPECT_EQUAL(pq.peek().priority, -4);
    pq.enqueue( { "Tony", -11 } );
    EXPECT_EQUAL(pq.peek().priority, -11);
}

STUDENT_TEST("PQHeap isEmpty") {
    PQHeap pq;
    EXPECT_EQUAL(pq.isEmpty(), true);
    pq.enqueue( { "Ama", -3 } );
    EXPECT_EQUAL(pq.isEmpty(), false);
}

STUDENT_TEST("pqSort time trial size 100000") {
    int startSize = 100000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> v;
        for (int i = 0; i < n; i++) {
            v.add({ "", randomInteger(1, n) });
        }
        TIME_OPERATION(n, pqSortHeap(v));
    }
}

STUDENT_TEST("pqSort time trial size 800000") {
    int startSize = 800000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> v;
        for (int i = 0; i < n; i++) {
            v.add({ "", randomInteger(1, n) });
        }
        TIME_OPERATION(n, pqSortHeap(v));
    }
}


STUDENT_TEST("topK: time trial same k = 10, size = 50000") {
    int startSize = 50000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStreamHeap(input);
        TIME_OPERATION(n, topKHeap(stream, k));
    }
}

STUDENT_TEST("topK: time trial same size = 200000, k = 10000") {
    int startSize = 200000;
    int k = 10000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStreamHeap(input);
        TIME_OPERATION(n, topKHeap(stream, k));
    }
}

STUDENT_TEST("topK: time trial same size = 200000, k = 20000") {
    int startSize = 200000;
    int k = 20000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStreamHeap(input);
        TIME_OPERATION(n, topKHeap(stream, k));
    }
}

STUDENT_TEST("topK: time trial same size = 200000, k = 40000") {
    int startSize = 200000;
    int k = 40000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStreamHeap(input);
        TIME_OPERATION(n, topKHeap(stream, k));
    }
}

STUDENT_TEST("topK: time trial same size = 200000, k = 80000") {
    int startSize = 200000;
    int k = 80000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStreamHeap(input);
        TIME_OPERATION(n, topKHeap(stream, k));
    }
}
/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("PQHeap example from writeup, validate each step") {
    PQHeap pq;
    Vector<DataPoint> input = {
        { "R", 4 }, { "A", 5 }, { "B", 3 }, { "K", 7 }, { "G", 2 },
        { "V", 9 }, { "T", 1 }, { "O", 8 }, { "S", 6 } };

    pq.validateInternalState();
    for (DataPoint& dp : input) {
        pq.enqueue(dp);
        pq.validateInternalState();

    }

    while (!pq.isEmpty()) {
        pq.dequeue();
        pq.validateInternalState();
    }
}
