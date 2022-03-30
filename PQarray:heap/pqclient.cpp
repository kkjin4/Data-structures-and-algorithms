#include "pqclient.h"
#include "pqarray.h"
#include "pqheap.h"
#include "vector.h"
#include "strlib.h"
#include <sstream>
#include "stack.h"
#include "testing/SimpleTest.h"
using namespace std;

/* Using the Priority Queue data structure as a tool to sort the element
 * in a Vector, then dequeue the sorted element back to the Vecotr.
 */
void pqSort(Vector<DataPoint>& v) {
    PQArray pq;

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
 * This function takes an istream of DataPoints. Using sorted ADT's
 * it finds the k biggest values in the istream.
 */
Vector<DataPoint> topK(istream& stream, int k) {
    Vector<DataPoint> topKs = {};
    PQArray pq;
    DataPoint cur;
    if(k == 0){
        return{};
    }
    while(stream >> cur){
        // if the number of element is less than k
        if(pq.size() < k){
            pq.enqueue(cur);
        }
        //compare new elements with exist elements
        else{
            DataPoint smallest = pq.peek();
            if (cur.priority > smallest.priority){
                pq.dequeue();
                pq.enqueue(cur);
            }
        }
    }

    // Use stack to reverse the Vector
    Stack<DataPoint> reverseTopk;
    while(!pq.isEmpty()){
        reverseTopk.push(pq.dequeue());
    }
    while (!reverseTopk.isEmpty()) {
        topKs.add(reverseTopk.pop());

    }

    return topKs;
}



/* * * * * * Test Cases Below This Point * * * * * */

/* Helper function that, given a list of data points, produces a stream from them. */
stringstream asStream(const Vector<DataPoint>& dataPoints) {
    stringstream result;
    for (const DataPoint& pt: dataPoints) {
        result << pt;
    }
    return result;
}

/* Helper function that, given range start and stop, produces a stream of values. */
stringstream asStream(int start, int stop) {
    stringstream result;
    for (int i = start; i <= stop; i++) {
        DataPoint pt = { "", double(i) };
        result << pt;
    }
    return result;
}

/* Helper function to fill vector with n random DataPoints. */
void fillVector(Vector<DataPoint>& vec, int n) {
    vec.clear();
    for (int i = 0; i < n; i++) {
        DataPoint pt = { "", randomReal(0, 100) };
        vec.add(pt);
    }
}


STUDENT_TEST("pqSort time trial size 1000") {
    int startSize = 1000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> v;
        for (int i = 0; i < n; i++) {
            v.add({ "", randomInteger(1, n) });
        }
        TIME_OPERATION(n, pqSort(v));
    }
}

STUDENT_TEST("pqSort time trial size 8000") {
    int startSize = 8000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> v;
        for (int i = 0; i < n; i++) {
            v.add({ "", randomInteger(1, n) });
        }
        TIME_OPERATION(n, pqSort(v));
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
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

STUDENT_TEST("topK: time trial same k = 10, size = 400000") {
    int startSize = 400000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

STUDENT_TEST("topK: time trial same size = 200000, k = 1000") {
    int startSize = 200000;
    int k = 1000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

STUDENT_TEST("topK: time trial same size = 200000, k = 2000") {
    int startSize = 200000;
    int k = 2000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

STUDENT_TEST("topK: time trial same size = 200000, k = 4000") {
    int startSize = 200000;
    int k = 4000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

STUDENT_TEST("topK: time trial same size = 200000, k = 8000") {
    int startSize = 200000;
    int k = 8000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}



STUDENT_TEST("Test topK with k greater than stream items"){
    Vector<DataPoint> input = {
        { "A", 1 }, { "B", 2 }, { "C", 3 }, { "D", 4 }
    };
    stringstream stream = asStream(input);

    Vector<DataPoint> expected = {
        { "D", 4 }, { "C", 3 }, { "B", 2 }, { "A", 1 }
    };

    EXPECT(topK(stream, 8) == expected);
}

STUDENT_TEST("Test topK with k equal to zero"){
    Vector<DataPoint> input = {
        { "A", 1 }, { "B", 2 }, { "C", 3 }, { "D", 4 }
    };
    stringstream stream = asStream(input);

    Vector<DataPoint> expected = {};

    EXPECT(topK(stream, 0) == expected);
}


STUDENT_TEST("Test topK with identical input"){
    Vector<DataPoint> input = {
        { "A", 1 }, { "A", 1 }, { "A", 1 }, { "D", 4 }
    };
    stringstream stream = asStream(input);

    Vector<DataPoint> expected = {
        { "D", 4 }, { "A", 1 }, { "A", 1 }, { "A", 1 }
    };

    EXPECT(topK(stream, 4) == expected);
}



STUDENT_TEST("Test runtimes of topK, varying n"){
    int startSize = 200000;
    int k = 10;
    for (int n = startSize; n < 12*startSize; n += 200000) {
        Vector<DataPoint> input;
        for (int i = 0; i < n; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

STUDENT_TEST("Test runtimes of topK, varying k"){
    int k = 10;
    for (int n = k; n < 12*k; n += 10) {
        Vector<DataPoint> input;
        for (int i = 0; i < 200000; i++) {
            input.add({ "", randomInteger(1, n) });
        }
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, n));
    }
}





/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("pqSort: vector of random elements") {
    setRandomSeed(137); // why might it be a good idea to set seed here?

    Vector<DataPoint> input;
    Vector<double> expected;
    fillVector(input, 100);
    for (DataPoint dp : input) {
        expected.add(dp.priority);
    }
    expected.sort();

    pqSort(input);
    for (int i = 0; i < input.size(); i++) {
        EXPECT_EQUAL(input[i].priority, expected[i]);
    }
}

PROVIDED_TEST("pqSort: time trial") {
    int startSize = 1000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> v;
        fillVector(v, n);
        TIME_OPERATION(n, pqSort(v));
    }
}


/* Constant used for sizing the tests below this point. */
const int kMany = 100000;

PROVIDED_TEST("topK: stream 0 elements, ask for top 1") {
    stringstream emptyStream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(emptyStream, 1), expected);
}

PROVIDED_TEST("topK: stream 1 element, ask for top 1") {
    stringstream stream = asStream({ { "" , 1 } });
    Vector<DataPoint> expected = { { "" , 1 } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("topK: small hand-constructed input") {
    Vector<DataPoint> input = { { "A", 1 }, { "B", 2 }, { "C", 3 }, { "D", 4 } };

    stringstream stream = asStream(input);
    Vector<DataPoint> expected = { { "D", 4 }, { "C", 3 } };
    EXPECT_EQUAL(topK(stream, 2), expected);

    stream = asStream(input);
    expected = { { "D", 4 }, { "C", 3 }, { "B", 2 }, { "A", 1 } };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("topK: stream many elements, ask for top 1") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> expected = { { "" , kMany } };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("topK: stream many elements, ask for top 5") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> expected = {
        { "" , kMany     },
        { "" , kMany - 1 },
        { "" , kMany - 2 },
        { "" , kMany - 3 },
        { "" , kMany - 4 },
    };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("topK: stress test, many elements, random values") {
    Vector<double> expected;
    Vector<DataPoint> points;
    fillVector(points, 10000);
    for (DataPoint dp : points) {
        expected.add(dp.priority);
    }
    sort(expected.begin(), expected.end(), greater<double>());

    stringstream stream = asStream(points);
    int k = 10;
    Vector<DataPoint> result = topK(stream, k);
    EXPECT_EQUAL(result.size(), k);
    for (int i = 0; i < k; i++) {
        EXPECT_EQUAL(result[i].priority, expected[i]);
    }
}

PROVIDED_TEST("topK: stress test - many elements, ask for top half") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> result = topK(stream, kMany/2);
    EXPECT_EQUAL(result.size(), kMany/2);
    EXPECT_EQUAL(result[0].priority, kMany);
    EXPECT_EQUAL(result[result.size()-1].priority, kMany - result.size() + 1);
}

PROVIDED_TEST("topK: time trial") {
    int startSize = 200000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input;
        fillVector(input, n);
        stringstream stream = asStream(input);
        TIME_OPERATION(n, topK(stream, k));
    }
}

