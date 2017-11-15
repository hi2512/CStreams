#pragma once

#include "Optional.h"

namespace stream
{

template <typename T>
class StreamImpl {
public:
	using value_type = T;

	virtual Optional<T> next() = 0;

	virtual ~StreamImpl() = 0;
};

template <typename T>
StreamImpl<T>::~StreamImpl() = default;

}
