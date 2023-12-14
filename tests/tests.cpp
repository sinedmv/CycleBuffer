#include <gtest/gtest.h>
#include <classes/classes.h>
#include <classes/extended.h>

TEST (CircBuffer, Simple) {
    CCircularBuffer<int> a = {1, 2, 3, 4, 5};
    auto it = a.begin();

    ASSERT_EQ(*it, 1);
    ASSERT_EQ(*(it + 1), 2);
    ASSERT_EQ(*(it + 2), 3);
    ASSERT_EQ(*(it + 3), 4);
    ASSERT_EQ(*(it + 4), 5);
}

TEST (CircBuffer, Clear) {
    CCircularBuffer<int> a = {1, 2, 3, 4, 5};
    a.clear();
    ASSERT_EQ(a.size(), 0);
    ASSERT_EQ(a.capacity(), 5);
    ASSERT_TRUE(a.empty());
}

TEST (CircBuffer, EmptyTest) {
    CCircularBuffer<int> a;
    ASSERT_EQ(a.size(), 0);
    ASSERT_EQ(a.capacity(), 0);
    ASSERT_TRUE(a.empty());
}


TEST (CircBuffer, EmptyPushPop) {
    CCircularBuffer<int> a;
    a.push_back(1);
    a.pop_front();
    ASSERT_EQ(a.size(), 0);
    ASSERT_TRUE(a.empty());
}

TEST (CircBuffer, InsertEmptyTest) {
    CCircularBuffer<int> a;
    auto it = a.begin();
    a.insert(it, 3, 4);
    ASSERT_EQ(a.size(), 3);
}

TEST (CircBuffer, InsertTestBegin) {
    CCircularBuffer<int> a = {1, 2, 3};
    auto it = a.begin();
    a.insert(it, 3, 4);
    ASSERT_EQ(a.size(), 6);
    ASSERT_EQ(*a.begin(), 4);
}

TEST (CircBuffer, InsertTestEnd) {
    CCircularBuffer<int> a = {1, 2, 3};
    auto it = a.end();
    a.insert(it, 5, 99);
    ASSERT_EQ(a.size(), 8);
    ASSERT_EQ(*(a.end() - 1), 99);
}

TEST (CircBuffer, InsertTestIl) {
    CCircularBuffer<int> a = {1, 2, 3};
    std::initializer_list<int> il = {99, 98, 97};
    auto it = a.end();
    a.insert(it, il);
    ASSERT_EQ(a.size(), 6);
    it = a.end();
    ASSERT_EQ(*(it - 1), 97);
    ASSERT_EQ(*(it - 2), 98);
    ASSERT_EQ(*(it - 3), 99);
    ASSERT_EQ(*(it - 4), 3);
}

TEST (CircBuffer, EraseSimple) {
    CCircularBuffer<int> a = {1, 2, 3};
    auto it = a.begin();
    it++;
    ASSERT_EQ(*(a.erase(it)), 3);
    ASSERT_EQ(a.size(), 2);
}

TEST (CircBuffer, EraseRange) {
    CCircularBuffer<int> a = {1, 2, 3, 4, 5, 6};
    a.pop_front();
    auto it1 = a.begin();
    auto it2 = a.begin();
    it2 += 2;
    a.erase(it1, it2);
    auto it = a.begin();
    ASSERT_EQ(*it, 4);
    ASSERT_EQ(*(it + 1), 5);
    ASSERT_EQ(*(it + 2), 6);
}

TEST (CircBuffer, PushBack) {
    CCircularBuffer<int> a = {1, 2};
    a.push_back(3);
    //When buffer's capacity == size, push-commands are rewriting data
    ASSERT_EQ(a.size(), 2);
    ASSERT_EQ(*(a.end() - 1), 3);
    ASSERT_EQ(*(a.begin()), 2);
}


TEST (CircBuffer, PushFront) {
    CCircularBuffer<int> a = {1, 2};
    a.push_front(3);
    //When buffer's capacity == size, push-commands are rewriting data
    ASSERT_EQ(a.size(), 2);
    ASSERT_EQ(*(a.end() - 1), 1);
    ASSERT_EQ(*(a.begin()), 3);
}

TEST (CircBuffer, PopFront) {
    CCircularBuffer<int> a = {1, 2, 3};
    a.pop_front();
    ASSERT_EQ(*a.begin(), 2);
    ASSERT_EQ(*(a.end() - 1), 3);
}

TEST (CircBuffer, EmptyPopFront) {
    CCircularBuffer<int> a;
    a.pop_front();
    // When buffer is empty, pop-commands don't do anything
    ASSERT_EQ(a.size(), 0);
}

TEST (CircBuffer, PopBack) {
    CCircularBuffer<int> a = {1, 2, 3};
    a.pop_back();
    ASSERT_EQ(*a.begin(), 1);
    ASSERT_EQ(*(a.end() - 1), 2);
}

TEST (CircBuffer, EmptyPopBack) {
    CCircularBuffer<int> a;
    a.pop_back();
    // When buffer is empty, pop-commands don't do anything
    ASSERT_EQ(a.size(), 0);
}

//////////////////////////////
/// More complicated tests

TEST (CircBuffer, ComplicatedTest1) {
    CCircularBuffer<int> a(2);
    a.push_front(1);
    ASSERT_EQ(*a.begin(), 1);
    a.push_back(2);
    ASSERT_EQ(*(a.end() - 1), 2);
    a.pop_front();
    ASSERT_EQ(a.size(), 1);
}

TEST (CircBuffer, ComplicatedTest2) {
    CCircularBuffer<int> a = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto it1 = a.begin();
    auto it2 = it1 + 5;
    it1 += 2;
    a.erase(it1, it2);
    ASSERT_EQ(*a.begin(), 1);
    ASSERT_EQ(*(a.begin() + 1), 2);
    ASSERT_EQ(*(a.begin() + 2), 6);
    ASSERT_EQ(*(a.begin() + 3), 7);
    ASSERT_EQ(*(a.begin() + 4), 8);
    ASSERT_EQ(*(a.begin() + 5), 9);
    a.insert(a.begin(), 1);
    ASSERT_EQ(*a.begin(), 1);
    a.pop_front();
    a.push_front(1000);
    ASSERT_EQ(*a.begin(), 1000);
}

/////////////////////////////
/// Tests for extended
/// Differences between Extended and not-Extended buffers: push_back and push_front
/// Now these commands do not overwrite anything

TEST (CircBufferExt, EmptyPushBack) {
    CCircularBufferExt<int> a;
    a.push_back(100);
    ASSERT_EQ(a.back(), 100);
}

TEST (CircBufferExt, PushBack) {
    CCircularBufferExt<int> a = {1, 2, 3};
    a.push_back(100);
    ASSERT_EQ(a.size(), 4);
    ASSERT_EQ(a.back(), 100);
}

TEST (CircBufferExt, EmptyPushFront) {
    CCircularBufferExt<int> a;
    a.push_front(100);
    ASSERT_EQ(a.front(), 100);
}

TEST (CircBufferExt, PushFront) {
    CCircularBufferExt<int> a = {1, 2, 3};
    a.push_front(100);
    ASSERT_EQ(a.size(), 4);
    ASSERT_EQ(a.front(), 100);
}

TEST (CircBufferExt, ComplicatedTest1) {
    CCircularBufferExt<int> a;
    a.push_front(1);
    a.push_back(2);
    a.push_back(-1);
    a.push_front(-2);
    ASSERT_EQ(a.front(), -2);
    ASSERT_EQ(a.back(), -1);
    ASSERT_EQ(*(a.begin() + 1), 1);
    ASSERT_EQ(a.size(), 4);
}

TEST (Algo, AlgoTest1) {
    CCircularBuffer<int> a = {414414, 2112, 1, 222, 412};
    ASSERT_FALSE(std::is_sorted(a.cbegin(), a.cend()));
    sort(a.begin(), a.end());
    ASSERT_TRUE(std::is_sorted(a.cbegin(), a.cend()));
    ASSERT_EQ(*a.begin(), 1);
    ASSERT_EQ(*(a.begin() + 1), 222);
    ASSERT_EQ(*(a.begin() + 2), 412);
    ASSERT_EQ(*(a.begin() + 3), 2112);
    ASSERT_EQ(*(a.begin() + 4), 414414);
}

TEST (Algo, AlgoTest2) {
    CCircularBuffer<int> a = {414414, 2112, 1, 222, 412};
    sort(a.begin(), a.end());
    ASSERT_EQ(*std::lower_bound(a.begin(), a.end(), 1000), 2112);
    ASSERT_EQ(*std::upper_bound(a.begin(), a.end(), 1), 222);
}

