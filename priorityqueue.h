#pragma once

#include <algorithm>
#include <cstdlib>
#include <list>
#include <utility>

// An implementation of a priority queue
template <typename T>
class PriorityQueue {
protected:
	const static size_t CAPACITY = 100; // Queue capacity

	class Node {
	public:
		T value;
		size_t pri; // Node priority, lower value = higher priority
		size_t seq; // Node sequence number
		bool operator< (const Node& n) // is less = is lower priority
			{ return n.pri < pri || (pri == n.pri && n.seq < seq); }
	};

	Node entries[CAPACITY]; // Queue entries
	size_t entry_count; // Number of entries current in the queue
	size_t sequence_count; // Sequence numbers used

	// Given index, determine parent, left child, or right child
	size_t parent (size_t idx) const { return (idx - 1) / 2; }
	size_t left(size_t idx) const { return 2 * idx + 1; }
	size_t right(size_t idx) const { return 2 * idx + 2; }

	// Push node at index downward (toward leaves) in the binary heap
	void push_down(size_t idx);

	// Push node at index upward (toward root) in the binary heap
	void push_up (size_t idx);

public:
	// Initialize empty queue, and sequence count
	PriorityQueue () { entry_count = sequence_count = 0; }

	// Add an entry (value) to the queue with priority value
	void enqueue (const T& value, size_t pri);

	// Return and remove top-pirority entry from queue
	T dequeue();

	// Return list of queue entries (pairs, sequence number, entry value)
	std::list<std::pair<size_t, T> > list() const;

	// Return number of entries in queue
	size_t size() const { return entry_count; }

	// void set_priority (size_t seq, size_t pri);
};

template <typename T>
T PriorityQueue<T>::dequeue() {

	// Get value to be returned
	T value = entries[0].value;

	// Copy last element to first element, and reduce entry count
	entries[0] = entries[entry_count - 1];
	entry_count--;

	// Push node at index 0 downward into its correct position
	if (entry_count > 0)
		push_down(0);

	// Return top-priority value
	return value;
}

template <typename T>
void PriorityQueue<T>::enqueue (const T& value, size_t pri) {

	// Add value and priority to next available entry, with next sequence number
	entries[entry_count].value = value;
	entries[entry_count].pri = pri;
	entries[entry_count].seq = ++sequence_count;
	entry_count++;

	// Push entry upward into its correct position within binary heap
	push_up(entry_count - 1);
}

template <typename T>
void PriorityQueue<T>::push_down(size_t idx) {

	size_t l = left(idx); // Index of left child
	size_t r = right(idx); // Index of right child
	size_t max = idx; // Index of highest priority of the three

	// Determine which of the three is the highest priority
	if (l < entry_count && entries[max] < entries[l])
		max = l;
	if (r < entry_count && entries[max] < entries[r])
		max = r;

	// Swap if necessary
	if (max != idx) {
		std::swap(entries[idx], entries[max]);
		push_down(max); // Recursively push downward into the binary heap
	}
}

template <typename T>
void PriorityQueue<T>::push_up (size_t idx) {
	
	// If not at the root, find correct position in the binary heap
	if (idx > 0) {
		// Get index of parent node
		size_t p = parent(idx);
		// If higher priority than parent, swap with parent
		if (entries[p] < entries[idx]) {
			std::swap(entries[p], entries[idx]);
			push_up(p); // Recursively push uploard into the binary heap
		}
	}
}

// template <typename T>
// void PriorityQueue<T>::set_priority (size_t seq, size_t pri) {
// 	size_t i = 0;
// 	while (i < entry_count && entries[i].seq != seq)
// 		i++;

// 	if (i == entry_count)
// 		return; // entry not found

// 	if (pri < entries[i].pri) {
// 		entries[i].pri = pri;
// 		push_up(i);
// 	}
// 	else if (pri > entries[i].pri) {
// 		entries[i].pri = pri;
// 		push_down(i);
// 	}
// }

template <typename T>
std::list<std::pair<size_t, T> > PriorityQueue<T>::list() const {
	
	// Build list of <sequence, value> of nodes in the binary heap
	std::list<std::pair<size_t, T> > l;
	for (size_t i = 0; i < entry_count; i++)
		l.push_back(std::pair<size_t, T>(entries[i].seq, entries[i].value));
	
	// Return the list
	return l;
}
