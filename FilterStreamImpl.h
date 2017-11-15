#pragma once

#include "StreamImpl.h"
#include <iostream>


namespace stream
{

template <typename T>
class Stream;

template <typename T, typename F>
class FilterStreamImpl: public StreamImpl<T> {
private:

	std::shared_ptr<StreamImpl<T>> strm;
	F fPointer;

public:

	FilterStreamImpl(std::shared_ptr<StreamImpl<T>> streamPointer, F&& t): fPointer(std::forward<F>(t)) {
		//std::cout << "filter MADE"<< std::endl;
		this->strm = streamPointer;
	}

	Optional<T> next() {
		Optional<T> res = strm->next();
		//std::cout << "res is: " << std::endl;
		if (!res) {
			return Optional<T>();
		}
		while (!fPointer(*res)) {
			//std::cout << "res is: " << *res << std::endl;
			res = strm->next();
			if (!res) {
				return Optional<T>();
			}
		}
		return res;
	}

};

}




