
#include <cstdint>
#include <type_traits>
#include <concepts>
#include <limits>

namespace mewt::types::numeric
{

	/*
	
	Key Concepts:

	Units:
	- Byte
	- Kilobyte
	- Second
	- Day
	- Game Session
	- Meter
	- Kilogram
	- Player
	- Vehicle

	Relative Units
	- Bytes per Kilobyte
	- Bytes per struct Foo
	- Bytes per Page
	- Bytes in a file
	- Seconds per day
	- Seconds per Game Session
	- Meters per height of vehicle
	- Meters per second
	- Kilograms per vehicle
	- Meters per second of a vehicle in a race [Do we allow for an arbitrary number of relation levels?]
	
	Discrete vs Continuous
	- Bytes are discrete, seconds can be continuous

	Dimensionality
	- Counts have only one dimension
	- Cells per row is 1D
	- Rows per table is 1D
	- A cell is 0-dimensional - a count of n-D units is (n+1) dimensional
	- If each cell is a 4D pixel colour, then we need to handle this distinction.
	- A cell unit is 0-D, a cell value has a unit that is 4-D
	- Eg, a vehicle unit is 0-d, but a vehicle is actually a 3D object
	- Can each unit have a discrete dimensionality, and also a continuous dimensionality?
	- - Byte is 0D discrete and 0D continuous
	- - Vehicle velocity is 0D discrete and 3D continuous
	- - Array of Bytes is 1D discrete and 0D continuous
	- Maybe we don't worry about dimensionality - we leave that up to value customisation.
	- A table row is only 1-D because it is number of cell widths * cell height, it's merely semantics

	Value types
	- Count
	- Index
	- Bitfield [Do we need an Index to represent a Bit index?]
	- Data (eg, a byte in emulation RAM)
	- Relative Metric (eg, length, 3D size, 3D velocity, duration, etc)
	- Absolute Metric (eg, position, time-stamp etc)

	Signedness
	- Perhaps have indices as unsigned and counts as signed?
	- 7-bit unsigned can store indices 0, ..., 127
	- 8-bit signed can store deltas -128, ..., 127 ; -128 could represent the (negated) size of the space??

	Value space
	- Memory layout of struct Foo
	- Container of items
	- 3D space
	- Game Session

	Ideas
	- Count should be of just a unit - units can then be broken down, ie, Count<Unit>, Count<Ratio<Ratio<Ratio<Ratio<Meters, Second>, Vehicle>, Race>>
	
	*/

	constexpr auto x = std::numeric_limits<char>::min();

}
