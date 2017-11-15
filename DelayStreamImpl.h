#pragma once

#include "StreamImpl.h"

namespace stream
{

template <typename T>
class Stream;

template <typename T, typename F>
class DelayStreamImpl: public StreamImpl<T> {
private:
	Optional<Stream<T>> memo;
	F thunk;
public:
	using value_type = T;

	DelayStreamImpl(F&& t): thunk(std::forward<F>(t)) {}

	Optional<value_type> next() override {
		if (!memo)
			memo = thunk();
		return memo->next();
	}
};

}
