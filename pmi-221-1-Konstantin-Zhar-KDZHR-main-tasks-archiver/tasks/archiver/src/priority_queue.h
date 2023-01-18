#pragma once

#include <functional>

template <typename T>
class PriorityQueue {
public:
    explicit PriorityQueue(bool (*comparator)(const T&, const T&)) : comparator_(comparator){};
    inline size_t GetLeftChild(size_t vertex) {
        return 2 * vertex + 1;
    }
    inline size_t GetRightChild(size_t vertex) const {
        return 2 * vertex + 2;
    }
    inline size_t GetParent(size_t vertex) const {
        if (vertex == 0) {
            return 0;
        }
        return (vertex - 1) / 2;
    }
    void NormalizeDown(size_t cur_vertex) {
        while (GetLeftChild(cur_vertex) < heap_.size()) {
            size_t left = GetLeftChild(cur_vertex);
            size_t right = GetRightChild(cur_vertex);
            size_t candidate = (right < heap_.size() && comparator_(heap_[left], heap_[right]) ? right : left);
            if (comparator_(heap_[cur_vertex], heap_[candidate])) {
                std::swap(heap_[cur_vertex], heap_[candidate]);
                cur_vertex = candidate;
            } else {
                break;
            }
        }
    }
    void NormalizeUp(size_t cur_vertex) {
        while (comparator_(heap_[GetParent(cur_vertex)], heap_[cur_vertex])) {
            std::swap(heap_[cur_vertex], heap_[GetParent(cur_vertex)]);
            cur_vertex = GetParent(cur_vertex);
        }
    }
    T Pop() {
        std::swap(heap_.front(), heap_.back());
        T deleted_elem = heap_.back();
        heap_.pop_back();
        NormalizeDown(0);
        return deleted_elem;
    }
    void Push(const T& elem) {
        heap_.push_back(elem);
        NormalizeUp(heap_.size() - 1);
    }
    size_t Size() {
        return heap_.size();
    }

private:
    bool (*comparator_)(const T&, const T&);
    std::vector<T> heap_;
};
