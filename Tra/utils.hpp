#include <vector>
#include <list>
#include <utility>
#include <algorithm>

using namespace std;

template<class container, class T>
typename container::iterator insert(container& arr, const T& val) {
	bool contain = false;
	typename container::iterator itor = arr.begin();
	while (itor != arr.end()) {
		if (*itor == val) {
			contain = true;
			break;
		} else if (*itor > val) {
			break;
		}
		itor++;
	}
	if (!contain) {
		arr.insert(itor, val);
		return (--itor);
	} else {
		return itor;
	}
}

template <class T>
bool order_by_itor(const std::pair<const list<T>*, typename std::list<T>::const_iterator>& pair1, const std::pair<const list<T>*, typename std::list<T>::const_iterator>& pair2) {
	return (*(pair1.second)) > (*(pair2.second));
}

template <class T>
void MergeKList(const std::vector<const std::list<T>*>& neighbour_lists, std::list<T>& candidates) {
	if (neighbour_lists.empty())
		return;

	std::vector<pair<const std::list<T>*, typename std::list<T>::const_iterator>> heap;
	for (int i = 0; i < neighbour_lists.size(); i++) {
		const std::list<T>* p_list = neighbour_lists[i];
		if (!p_list->empty())	
			heap.push_back(std::make_pair(neighbour_lists[i], neighbour_lists[i]->cbegin()));
	}
	
	std::make_heap(heap.begin(), heap.end(), order_by_itor<T>);
	T pre = NAN;
	while (!heap.empty()) {
		std::pop_heap(heap.begin(), heap.end(), order_by_itor<T>);
		pair<const list<T>*, typename list<T>::const_iterator> min = heap.back();
		heap.pop_back();
		T min_val = *(min.second);
		if (min_val != pre) {
			candidates.insert(candidates.end(), min_val);
			pre = min_val;
		}
		min.second++;
		if (min.second != min.first->cend()) {
			heap.push_back(min);
			push_heap(heap.begin(), heap.end(), order_by_itor<T>);
		}	
	}
}
