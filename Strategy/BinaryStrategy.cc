#include "binary_strategy.h"

bool BinaryStrategy::HasNext() {
	int size = index_.size();
	if (is_end_) {
		return get_end_index();
	} else if (cur_ < size){
		return true;
	} else {
		  return get_new_index();
  }
}

int BinaryStrategy::Next() {
	int val = index_[cur_++];
  is_used_[val] = true;
  return val;
}

bool BinaryStrategy::get_new_index() {
      std::vector<int> a_index;
      for (auto end : end_index_) {
      	for (auto old_num : index_) {
        	int a_num = (old_num + end) / 2;
          if (!is_used_[a_num]) {
          	a_index.push_back(a_num);
          }
        } 
			}
      if (a_index.empty()) {
			  cur_ = 0;
        return false;
			} else {
        index_ = a_index;
        cur_ = 0;
        return true;
      }
	}
}


bool BinaryStrategy::get_end_index() {
	int size = index_.size();
      if (cur_ < size) {
        return true;
      } else{
        is_end_ = false;
        int mid = (begin_ + end_) / 2;
        if (is_used_[mid]) {
          return false;
        } else {
          index_.clear();
          index_.push_back(mid);
          cur_ = 0;
          return true;
        }
      }
	}
}
