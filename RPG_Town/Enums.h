#ifndef ENUMS_H_
#define ENUMS_H_

enum Speed{
	Slow = 1,
	Normal = 4,
	Fast = 8,
	Fastest = 16
};

enum System{
	Tilesize = 64
};

enum Direction {
	South, 
	West, 
	East, 
	North,
	Null
};

enum Layer{
	Background_1,
	Background_2,
	Field,
	Collision_Objects,
	Collision_Boxes,
	Event_Layer,
	Overlay
};

#endif