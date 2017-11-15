#pragma once

#include "StreamImpl.h"

namespace stream
{

template <typename T>
class EmptyStreamImpl: public StreamImpl<T> {
public:
	EmptyStreamImpl() = default;

	Optional<T> next() {
		return Optional<T>();
	}
};

}
