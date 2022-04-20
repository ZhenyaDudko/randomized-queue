#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <type_traits>
#include <utility>
#include <vector>

template <class T>
class randomized_queue
{
public:
    randomized_queue()
        : mersenne(std::random_device{}())
    {
    }

    std::size_t size() const
    {
        return queue.size();
    }

    bool empty() const
    {
        return queue.empty();
    }

    void enqueue(T element)
    {
        queue.push_back(std::move(element));
    }

    const T & sample() const
    {
        return queue[random_index()];
    }

    T dequeue()
    {
        std::size_t ind = random_index();
        T temp = std::move(queue[ind]);
        std::swap(queue[ind], queue.back());
        queue.pop_back();
        return temp;
    }

    template <bool is_const>
    class Iterator
    {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = std::conditional_t<is_const, const T, T>;
        using pointer = value_type *;
        using reference = value_type &;
        using iterator_category = std::forward_iterator_tag;

        Iterator() = default;

        friend bool operator==(const Iterator & lhs, const Iterator & rhs)
        {
            return lhs.order == rhs.order && lhs.index == rhs.index;
        }

        friend bool operator!=(const Iterator & lhs, const Iterator & rhs)
        {
            return !(lhs == rhs);
        }

        reference operator*() const
        {
            return *order[index];
        }

        pointer operator->() const
        {
            return order[index];
        }

        Iterator & operator++()
        {
            index++;
            if (order.size() == index) {
                order.clear();
                index = 0;
            }
            return *this;
        }

        Iterator operator++(int)
        {
            auto tmp = *this;
            operator++();
            return tmp;
        }

    private:
        friend class randomized_queue;

        std::vector<pointer> order;
        std::size_t index = 0;

        Iterator(std::conditional_t<is_const, const randomized_queue, randomized_queue> & q)
            : order(q.size())
        {
            for (std::size_t i = 0; i < q.size(); ++i) {
                order[i] = &q.queue[i];
            }
            std::shuffle(order.begin(), order.end(), q.mersenne);
        }
    };

    using iterator = Iterator<false>;
    using const_iterator = Iterator<true>;

    iterator begin() { return {*this}; }
    iterator end() { return {}; }
    const_iterator cbegin() const { return {*this}; }
    const_iterator cend() const { return {}; }
    const_iterator begin() const { return cbegin(); }
    const_iterator end() const { return cend(); }

private:
    std::vector<T> queue;
    mutable std::mt19937 mersenne;

    std::size_t random_index() const
    {
        std::uniform_int_distribution<std::size_t> distribution(0, size() - 1);
        return distribution(mersenne);
    }
};
