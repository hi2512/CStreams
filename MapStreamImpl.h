#pragma once

#include "StreamImpl.h"
#include <iostream>


namespace stream
{

template <typename T>
class Stream;

template <typename T, typename F>
class MapStreamImpl: public StreamImpl<T> {
private:

	std::shared_ptr<StreamImpl<T>> strm;
	F fPointer;

public:

	MapStreamImpl(std::shared_ptr<StreamImpl<T>> streamPointer, F&& t): fPointer(std::forward<F>(t)) {
		//std::cout << "filter MADE"<< std::endl;
		this->strm = streamPointer;
	}

	Optional<T> next() {
		Optional<T> res = strm->next();
		if (!res) {
			return Optional<T>();
		}
		return Optional<T>(fPointer(*res));
	}

};

}




