#include "Stream.h"

#include <iostream>
#include <cassert>

using namespace stream;

int main() {
	// Put your "unit test" codes here

	// Example:

	// auto testStream = take(hamming(), 20);
	// while (auto elem = testStream.next())
	//	std::cout << *elem << std::endl;
	auto s = stream::empty<int>(); // Creates an empty stream of integers
	for (int i = 0; i < 100; i++)
		assert(!s.next());
	//std::cout << "DONE" << std::endl;

	auto d = stream::once(2);
	auto opInt = d.next();
	//Optional<int> opInt = 2;
	// The first element pulled from s0 holds 2
	std::cout << "opint is " << *opInt << std::endl;
	assert(opInt && *opInt == 2);
	// Subsequent next() invocation always yield empty value
	for (int i = 0; i < 100; i++)
		assert(!d.next());

	auto e = chain(once(2), once(3));
	auto first = e.next();
	assert(first && *first == 2);
	auto second = e.next();
	assert(second && *second == 3);
	assert(!s.next());

	// Produce a stream with three elements
	auto s0 = chain(once(1.1), chain(once(1.2), once(1.3)));
	// Produce a stream with size 100
	auto s1 = once(0);
	for (int i = 1; i < 99; ++i)
		s1 = chain(s1, once(i));

	e = chain(once(2), once(3));
	auto f = chain(once(6), once(7));
	auto combo = chain(e, f);
	//auto taken = take(combo, 3);
	auto fed =
			[] (int num) {/*return num % 2 == 0;*/return ((num == 3) || (num == 8));};
	auto fit = [] (int num) {return num * 3;};

	auto h = chain(once(2), once(3));
	auto k = chain(once(6), once(8));
	auto hk = chain(h, k);

	auto filterTest = filter(hk, fed);
	Optional<int> filtCur = filterTest.next();
	while (filtCur) {
		std::cout << "stream filter: " << *filtCur << std::endl;
		filtCur = filterTest.next();
	}

	auto taken = map(combo, fit);

	Optional<int> cur = taken.next();
	while (cur) {
		std::cout << "stream map: " << *cur << std::endl;
		cur = taken.next();
	}
	std::cout << "DONE" << std::endl;
	auto co = stream::counter(2);
	auto it = co.next();
	int sdf = 0;

	while (it && (sdf < 100)) {
		std::cout << "stream counter: " << *it << std::endl;
		it = co.next();
		sdf++;
	}

	auto p = take(prime(), 20);
	std::cout << "PRIME LIST\n";
	while (auto elem = p.next())
		std::cout << *elem << '\n';

	auto ham = take(hamming(), 20);
	std::cout << "hamming LIST?\n";
	while (auto ele = ham.next())
		std::cout << *ele << '\n';

	auto pie = take(pi(), 500);
	std::cout << "pi LIST?\n";
	while (auto el = pie.next())
		std::cout << *el << '\n';

}
