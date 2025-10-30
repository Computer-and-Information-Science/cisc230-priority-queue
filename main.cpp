#include "priorityqueue.h"
#include <iostream>
#include <string>
using namespace std;

void print (const PriorityQueue<string>& q) {
	int count = 0;
    for (auto entry: q.list())
        cout << count++ << ":" << entry.first << "/" << entry.second << endl;
}

int main() {
	PriorityQueue<string> queue;
	queue.enqueue("dog", 50);
	queue.enqueue("cat", 30);
	queue.enqueue("bird", 40);
	queue.enqueue("turtle", 60);
	queue.enqueue("chicken", 40);
	queue.enqueue("duck", 40);
	// queue.set_priority(1, 100);
	print(queue);

	while (queue.size()) {
		cout << queue.dequeue() << endl;
	}
}
