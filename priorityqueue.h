#pragma once

#include <algorithm>
#include <cstdlib>
#include <list>
#include <utility>

template <typename T>
class PriorityQueue {
protected:
	const static size_t CAPACITY = 100; // Queue capacity

	class Node {
	public:
		T value;
		size_t pri; // Node priority
		size_t seq; // Node sequence number
		bool operator< (const Node& n) // Lower priority is less
			{ return pri < n.pri || (pri == n.pri && n.seq < seq); }
	};

	Node entries[CAPACITY]; // Queue entries
	size_t entry_count; // Number of entries current in the queue
	size_t sequence_count; // Sequence numbers used

	size_t parent (size_t idx) const { return (idx - 1) / 2; }
	size_t left(size_t idx) const { return 2 * idx + 1; }
	size_t right(size_t idx) const { return 2 * idx + 2; }

	void push_down(size_t idx);
	void push_up (size_t idx);

public:
	PriorityQueue () { entry_count = sequence_count = 0; }

	void enqueue (const T& value, size_t pri);
	T dequeue();

	std::list<std::pair<size_t, T> > list() const;

	size_t size() const { return entry_count; }
};

template <typename T>
T PriorityQueue<T>::dequeue() {
	T value = entries[0].value;

	// Copy last element to first element
	entries[0] = entries[entry_count - 1];
	entry_count--;

	// Push node at index 0 into its correct position
	if (entry_count > 0)
		push_down(0);

	return value;
}

template <typename T>
void PriorityQueue<T>::enqueue (const T& value, size_t pri) {
	entries[entry_count].value = value;
	entries[entry_count].pri = pri;
	entries[entry_count].seq = ++sequence_count;
	entry_count++;
	push_up(entry_count - 1);
}

template <typename T>
void PriorityQueue<T>::push_down(size_t idx) {
	size_t l = left(idx); // Index of left child
	size_t r = right(idx); // Index of right child
	size_t max = idx; // Index of highest priority of the three
	if (l < entry_count && entries[max] < entries[l])
		max = l;
	if (r < entry_count && entries[max] < entries[r])
		max = r;
	// Swap if necessary
	if (max != idx) {
		std::swap(entries[idx], entries[max]);
		push_down(max);
	}
}

template <typename T>
void PriorityQueue<T>::push_up (size_t idx) {
	if (idx > 0) {
		size_t p = parent(idx);
		if (entries[p] < entries[idx]) {
			std::swap(entries[p], entries[idx]);
			push_up(p);
		}
	}
}

template <typename T>
std::list<std::pair<size_t, T> > PriorityQueue<T>::list() const {
	std::list<std::pair<size_t, T> > l;
	for (size_t i = 0; i < entry_count; i++)
		l.push_back(std::pair<size_t, T>(entries[i].seq, entries[i].value));
	return l;
}
