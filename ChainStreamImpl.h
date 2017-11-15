#pragma once

#include "StreamImpl.h"

#include <stdlib.h>;
#include <queue>;
#include <memory>;

#include <iostream>;

namespace stream {

template<typename T>
class ChainStreamImpl: public StreamImpl<T> {

	//std::queue<T> stuff;
	std::shared_ptr<StreamImpl<T>> first;
	std::shared_ptr<StreamImpl<T>> second;
public:
	ChainStreamImpl<T>(std::shared_ptr<StreamImpl<T>> f,
			std::shared_ptr<StreamImpl<T>> s) {
/*
		Optional<T> firstOp = f->next();
		while (firstOp) {
			stuff.push(*firstOp);
			firstOp = f->next();
		}

		Optional<T> secondOp = s->next();
		while (secondOp) {
			stuff.push(*secondOp);
			secondOp = s->next();
		}
*/
		first = f;
		second = s;
	}

	Optional<T> next() {
		/*
		if (stuff.empty()) {
			return Optional<T>();
		}
		T val = stuff.front();
		stuff.pop();
		return Optional<T>(val);
		*/
		//return Optional<T>();
		Optional<T> res = first->next();
		if(res){
			return res;
		} else {
			return second->next();
		}
	}

};

}
