#ifndef STRATEGY_BINARY_STRATEGY_H
#define STRATEGY_BINARY_STRATEGY_H
#include "Strategy"

class BinaryStrategy: public Strategy {
public:
	BinaryStrategy(int begin=0, int end=0): Strategy(begin, end) {
	}

	bool HasNext();
		
	int Next();

private:
	bool get_new_index();

	bool get_end_index();
	
};

#endif
