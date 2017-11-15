#pragma once

#include <new>
#include <utility>

namespace stream
{

template <typename T>
class Optional {
private:
	using StoragetType = typename std::aligned_storage<sizeof(T), alignof(T)>::type;
	StoragetType storage;
	bool valid;

	T* get() {
		return reinterpret_cast<T*>(&storage);
	}
	const T* get() const {
		return reinterpret_cast<const T*>(&storage);
	}
	T* getOrNull() {
		return valid ? get() : nullptr;
	}
	const T* getOrNull() const {
		return valid ? get() : nullptr;
	}
public:
	Optional(): valid(false) {}
	Optional(const Optional<T>& rhs): valid(rhs.valid) {
		if (valid)
			new (get()) T(*rhs.get());
	}
	Optional(Optional<T>&& rhs): valid(rhs.valid) {
		if (valid)
		{
			new (get()) T(std::move(*rhs.get()));
			rhs.valid = false;
		}
	}
	Optional(const T& rhs): valid(true) {
		new (get()) T(rhs);
	}
	Optional(T&& rhs): valid(true) {
		new (get()) T(std::move(rhs));
	}
	~Optional() { clear(); }

	Optional<T>& operator=(const Optional<T>& rhs) {
		if (this != &rhs)
		{
			clear();
			valid = rhs.valid;
			if (rhs.valid)
				new (get()) T(*rhs.get());
		} 
		return *this;
	}
	Optional<T>& operator=(Optional<T>&& rhs) {
		if (this != &rhs)
		{
			clear();
			valid = rhs.valid;
			if (rhs.valid)
			{
				new (get()) T(std::move(*rhs.get()));
				rhs.valid = false;
			}
		} 
		return *this;
	}
	Optional<T>& operator=(const T& rhs) {
		clear();
		valid = true;
		new (get()) T(rhs);
		return *this;
	}
	Optional<T>& operator=(T&& rhs) {
		clear();
		valid = true;
		new (get()) T(std::move(rhs));
		return *this;
	}

	explicit operator bool() const { return valid; }

	T* operator->() { return getOrNull(); }
	const T* operator->() const { return getOrNull(); }
	T& operator*() { return *getOrNull(); }
	const T& operator*() const { return *getOrNull(); }

	void clear() {
		if (valid)
		{
			get()->~T();
			valid = false;
		}
	}
};

}
