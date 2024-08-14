#pragma once
#include <cstddef>
#include <array>

struct Dummy { int i = 1; };	// For intellisense to work with templated types
 
template <typename Comp>
class SparseArray {
private:
	static const size_t MaxEnts = 1000;
	const size_t NULL_ELEMENT = -1;
	size_t length;

	int sparse[MaxEnts];
	size_t denseIndices[MaxEnts];
	std::array<Comp, MaxEnts> dense;  

public:
	SparseArray() : length(0)
	{
		for ( size_t i = 0; i < MaxEnts; ++i ) {
			sparse[i] = NULL_ELEMENT;  // Initialize sparse array
		}
	}

	auto begin()
	{
		return this->dense.begin();
	}

	auto end()
	{
		return this->dense.end();
	}

	int getLength()
	{
		return length;
	}

	void add(size_t e, const Comp & comp)
	{
		if ( e >= MaxEnts ) {
			return; // Out of bounds check
		}
		if ( sparse[e] != NULL_ELEMENT ) {
			return; // Component already exists
		}
		if ( length >= MaxEnts ) {
			return; // Maximum capacity reached
		}
		dense[length] = comp;
		denseIndices[length] = e;
		sparse[e] = length;
		length++;
	}

	void remove(size_t e)
	{
		if ( e >= MaxEnts || sparse[e] == NULL_ELEMENT ) {
			return; // No component to remove or out of bounds
		}
		size_t indexToRemove = sparse[e];
		Comp lastComponent = dense[length - 1];
		size_t lastEntity = denseIndices[length - 1];

		dense[indexToRemove] = lastComponent;
		denseIndices[indexToRemove] = lastEntity;
		sparse[lastEntity] = indexToRemove;

		sparse[e] = NULL_ELEMENT;
		length--;
	}

	Comp * get(size_t e)
	{
		if ( e >= MaxEnts || sparse[e] == NULL_ELEMENT ) {
			return nullptr; // No component found or out of bounds
		}
		return &dense[sparse[e]];
	}

	void set(size_t e, Comp c)
	{
		dense[e] = c;
	}
};
