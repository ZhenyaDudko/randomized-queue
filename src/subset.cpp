#include "subset.h"

#include "randomized_queue.h"

void subset(unsigned long k, std::istream & in, std::ostream & out)
{
    randomized_queue<std::string> queue;
    std::string line;
    while (k > 0 && std::getline(in, line)) {
        queue.enqueue(line);
        --k;
    }
    while (!queue.empty()) {
        out << queue.dequeue() << std::endl;
    }
}
