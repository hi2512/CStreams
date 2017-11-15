#pragma once

#include "DelayStreamImpl.h"
#include "EmptyStreamImpl.h"
#include "StreamImpl.h"
#include "SingletonStreamImpl.h"
#include "ChainStreamImpl.h"
#include "FilterStreamImpl.h"
#include "MapStreamImpl.h"

#include <memory>
#include <unistd.h>

namespace stream {

template<typename T>
class Stream {
private:
	std::shared_ptr<StreamImpl<T>> impl;

	Stream(const std::shared_ptr<StreamImpl<T>>& i) :
			impl(i) {
	}

	// Make those functions friend of Stream<T>.
	// They are't static members of Stream<T> since we want to avoid the users having to explicitly specify T
	template<typename U> friend Stream<U> empty();
	template<typename U> friend Stream<U> once(U val);
	template<typename U> friend Stream<U> chain(const Stream<U>& first,
			const Stream<U>& second);
	template<typename U> friend Stream<U> take(const Stream<U>& stream,
			size_t n);
	template<typename U, typename F> friend Stream<U> filter(
			const Stream<U>& stream, F&& f);

	template<typename F> friend decltype(std::declval<F>()()) delay(F&&);

	template<typename S, typename F> friend Stream<
			decltype(std::declval<F>()(std::declval<S>()) )> map(
			const Stream<S>& stream, F&& f);

	//friend Stream<int> sieve(Stream<int>);
	//friend Stream<> prime();

	Stream<int> chain(int);

public:
	using value_type = T;

	Optional<T> next() {
		return impl->next();
	}
};

template<typename T>
Stream<T> empty() {
	return Stream<T>(std::make_shared<EmptyStreamImpl<T>>());
}

template<typename F>
decltype(std::declval<F>()()) delay(F&& func) {
	using V = decltype(std::declval<F>()());
	using T = typename V::value_type;
	return V(std::make_shared<DelayStreamImpl<T, F>>(std::forward < F > (func)));
}

// You need to implement all the funtions listed below. Read the handout for more information
// Here is a checklist: 
// - once()
// - chain()
// - take()
// - filter()
// - map()
// - prime()
// - hamming()
// - pi()

template<typename T>
Stream<T> once(T val) {
	// Fill in the implementation here
	return Stream<T>(std::make_shared < SingletonStreamImpl < T >> (val));
}

template<typename T>
Stream<T> chain(const Stream<T>& first, const Stream<T>& second) {
	// Fill in the implementation here
	return Stream<T>(
			std::make_shared < ChainStreamImpl < T >> (first.impl, second.impl));
}

template<typename T>
Stream<T> take(const Stream<T>& stream, size_t n) {
	// Fill in the implementation here
	Stream<T> res = Stream<T>(std::make_shared<EmptyStreamImpl<T>>());
	std::shared_ptr<StreamImpl<T>> sImpl = stream.impl;
	Optional<T> op = sImpl->next();
	while (op && (n > 0)) {
		res = chain(res, once(*op));
		op = sImpl->next();
		n--;
	}
	return res;
}

template<typename T, typename F>
Stream<T> filter(const Stream<T>& stream, F&& f) {
	// Fill in the implementation here
	/*
	 Stream<T> res = Stream<T>(std::make_shared<EmptyStreamImpl<T>>());
	 std::shared_ptr<StreamImpl<T>> sImpl = stream.impl;
	 Optional<T> op = sImpl->next();
	 while (op) {
	 if (f(*op)) {
	 res = chain(res, once(*op));
	 }
	 op = sImpl->next();

	 }
	 return res;
	 */
	return Stream<T>(
			std::make_shared<FilterStreamImpl<T, F>>(stream.impl,
					std::forward < F > (f)));
}

template<typename S, typename F>
Stream<decltype(std::declval<F>()(std::declval<S>()))> map(
		const Stream<S>& stream, F&& f) {
	// Fill in the implementation here
	/*
	 Stream<S> res = Stream<S>(std::make_shared<EmptyStreamImpl<S>>());
	 std::shared_ptr<StreamImpl<S>> sImpl = stream.impl;
	 Optional<S> op = sImpl->next();
	 while (op) {

	 res = chain(res, once(f(*op)));

	 op = sImpl->next();

	 }
	 return res;
	 */
	return Stream<S>(
			std::make_shared<MapStreamImpl<S, F>>(stream.impl,
					std::forward < F > (f)));
}

Stream<int> counter(int a) {
	return chain(once(a), delay([a] () {return counter(a + 1);}));
}

Stream<double> counter(double a) {
	return chain(once(a), delay([a] () {return counter(a + 1);}));
}

Stream<int> repeat() {
	return chain(once(1), delay(repeat));
}

bool multiple(int a) {
	return (a % 3 == 0);
}

Stream<int> sieve(Stream<int> s) {

	Optional<int> op = s.next();
	int pr = *(op);
	//std::cout << pr << ": pr value" << std::endl;
	auto mul = [pr] (int num) {return (num % pr != 0);};
	//std::cout << "mul test: " << mul(4) << std::endl;

	return chain(once(pr), delay([=] () {return filter(sieve(s), mul);}));

	//return Stream<size_t>(std::make_shared < PrimeStreamImpl <size_t>> (s.impl));

}

// The prime() function returns a stream of prime numbers, in increasing order
// A prime number is a natural number greater than 1 that has no positive divisors other than 1 and itself
// The first 20 prime numbers are { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71 }
Stream<int> prime() {
	// Fill in the implementation here
	return sieve(counter(2));
}

// (THIS IS A HELPER FUNCTION FOR hamming())
// The mergeUnique function takes two streams s0 and s1 as parameters
// Both s0 and s1 are sequences of increasing numbers (potentially infinite)
// Return a new stream of increasing numbers that contains all numbers from s0 and s1, removing duplicates
// e.g. if s0 = { 4, 6, 8, 10 } and s1 = { 3, 6, 9, 12, 15 }, the returning stream should be { 3, 4, 6, 8, 9, 10, 12, 15 }
Stream<int> mergeUnique(Stream<int> s0, Stream<int> s1) {
	// Fill in the implementation here
	int s0num = *(s0.next());
	int s1num = *(s1.next());
	//std::cout << "Deciding to add number " << s0num << " or " << s1num << "\n";
	if (s0num == s1num) {
		return chain(once(s0num), delay([=] () {return mergeUnique(s0, s1);}));
	} else if (s0num < s1num) {
		//unput?
		return chain(once(s0num), delay([=] () {return mergeUnique(s0, chain(once(s1num), s1));}));
	} else {
		return chain(once(s1num), delay([=] () {return mergeUnique(chain(once(s0num), s0), s1);}));
	}

}

// The hamming() function returns a stream of hamming numbers, in increasing order
// A hamming sequence enumerate, in ascending order with no repetitions, all positive integers with no prime factors other than 2, 3, or 5
// The first 20 hamming numbers are { 1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 16, 18, 20, 24, 25, 27, 30, 32, 36 }
Stream<int> hamming() {
	// Fill in the implementation here
	auto x2 = [] (int num) {return num * 2;};
	auto x3 = [] (int num) {return num * 3;};
	auto x5 = [] (int num) {return num * 5;};


	return chain(once(1),
			delay(
					[x2, x3, x5] () {return mergeUnique(map(counter(1), x2), mergeUnique(map(counter(1), x3), map(counter(1), x5) ));}));

	//return chain(once(1), mergeUnique(map(counter(1), x2), mergeUnique(map(counter(1), x3), map(counter(1), x5) )));
}

// (THIS IS A HELPER FUNCTION FOR pi())
// The partialSum function takes a streams s of doubles (potentially infinite) and returns a new stream representing the partial sum of the input stream
// e.g. if s = { 1, 2, 3, 4, 5 }, the returning stream should be { 1, 3, 6, 10, 15 }
Stream<double> partialSum(Stream<double> s, double prev, bool sign) {
	// Fill in the implementation here
	double val = *(s.next());
	if (!sign) {
		val *= -1;
	}
	return chain(once( (val + prev)),
			delay( [s, val, prev, sign] () {return partialSum(s, val + prev, !sign);}  ) );

}



// The pi() function returns a stream of increasingly accurate approximations of pi.
// Use the expansion series for pi/4 for the approximation: 
// pi/4 = 1 - 1/3 + 1/5 - 1/7 + 1/9 - ...
Stream<double> pi() {
	// Fill in the implementation here
	auto div = [] (double bottom) {return 1.0 / bottom;};
	auto x2 = [] (double num) {return num * 2;};
	auto m1 = [] (double num) {return num - 1;};
	auto x4 = [] (double num) {return num * 4;};
	Stream<double> i = counter(1.0);
	Stream <double> i1 = counter(0.0);
	//Stream<int> i2 = counter(1);
	Stream<double> in =  map( map( map(i, x2), m1 ), div);
	return map(partialSum(in, 0.0, true), x4);


}

}
