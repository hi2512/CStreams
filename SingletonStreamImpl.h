#pragma once

#include "StreamImpl.h"

namespace stream {

template<typename T>
class SingletonStreamImpl: public StreamImpl<T> {

	T i;
	bool used;

public:
	SingletonStreamImpl<T>(T t) {
		this->i = t;
		this->used = false;
	}

	Optional<T> next() {
		if(!used) {
			used = true;
			return Optional<T>(this->i);
		} else {
			return Optional<T>();
		}

	}
};

}
