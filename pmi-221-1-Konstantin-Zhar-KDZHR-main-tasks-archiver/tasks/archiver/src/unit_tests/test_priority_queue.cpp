#include <catch.hpp>
#include <queue>

#include "../priority_queue.h"

struct CustomStruct {
    explicit CustomStruct(int val) : val(val) {
    }
    int val;
    bool operator<(const CustomStruct& other) const {
        return val < other.val;
    }
    bool operator==(const CustomStruct& other) const {
        return val == other.val;
    }
};

bool Comparator(const CustomStruct& first, const CustomStruct& second) {
    return second < first;
}

const int DEL = INT32_MIN;

TEST_CASE("PriorityQueue") {
    PriorityQueue<CustomStruct> custom_pq(Comparator);
    std::priority_queue<CustomStruct, std::vector<CustomStruct>, decltype(&Comparator)> stl_pq(Comparator);
    std::vector<int> operations = {5,   DEL, 78,  23,  1,   43,  2,   42,  32,  33,
                                   DEL, DEL, DEL, DEL, DEL, DEL, 234, DEL, DEL, DEL};
    for (int op : operations) {
        if (op == DEL) {
            REQUIRE(custom_pq.Pop() == stl_pq.top());
            stl_pq.pop();
            REQUIRE(custom_pq.Size() == stl_pq.size());
        } else {
            custom_pq.Push(CustomStruct(op));
            stl_pq.push(CustomStruct(op));
            REQUIRE(custom_pq.Size() == stl_pq.size());
        }
    }
}
