/*
 CircularBuffer.h - Circular buffer library for Arduino.
 Copyright (c) 2017 Roberto Lo Giacco.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as 
 published by the Free Software Foundation, either version 3 of the 
 License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __CIRCULAR_BUFFER__
#define __CIRCULAR_BUFFER__
#include <inttypes.h>

#ifndef CIRCULAR_BUFFER_XS
#define __CB_ST__ uint16_t
#else
#define __CB_ST__ uint8_t
#endif

#ifdef CIRCULAR_BUFFER_DEBUG
#include <Print.h>
#endif

template<typename T, __CB_ST__ S> class CircularBuffer {
public:

	CircularBuffer();

	~CircularBuffer();

	/**
	 * Adds an element to the beginning of buffer: the operation returns `false` if the addition caused overwriting an existing element.
	 */
	bool unshift(T value);

	/**
	 * Adds an element to the end of buffer: the operation returns `false` if the addition caused overwriting an existing element.
	 */
	bool push(T value);

	/**
	 * Removes an element from the beginning of the buffer.
	 */
	T shift();

	/**
	 * Removes an element from the end of the buffer.
	 */
	T pop();

	/**
	 * Returns the element at the beginning of the buffer.
	 */
	T inline first();

	/**
	 * Returns the element at the end of the buffer.
	 */
	T inline last();

	/**
	 * Array-like access to buffer
	 */
	T operator [] (__CB_ST__ index);

	/**
	 * Returns how many elements are actually stored in the buffer.
	 */
	__CB_ST__ inline size();

	/**
	 * Returns how many elements can be safely pushed into the buffer.
	 */
	__CB_ST__ inline available();

	/**
	 * Returns how many elements can be potentially stored into the buffer.
	 */
	__CB_ST__ inline capacity();

	/**
	 * Returns `true` if no elements can be removed from the buffer.
	 */
	bool inline isEmpty();

	/**
	 * Returns `true` if no elements can be added to the buffer without overwriting existing elements.
	 */
	bool inline isFull();

	/**
	 * Resets the buffer to a clean status, dropping any reference to current elements
	 * and making all buffer positions available again.
	 */
	void inline clear();

	#ifdef CIRCULAR_BUFFER_DEBUG
	void inline debug(Print* out);
	void inline debugFn(Print* out, void (*printFunction)(Print*, T));
	#endif

private:
	T buffer[S];
	T *head;
	T *tail;
	uint16_t count;
};

#include "CircularBuffer.tpp"
#endif
