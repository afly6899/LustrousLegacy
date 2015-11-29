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

enum Selection {
	Play_Game = 1,
	Load_Game = 2,
	Settings = 3,
	Exit = 4,
};

enum Cursor_Direction {
	Down,
	Up
};

enum Font_Size {
	Small = 14,
	Default = 18, 
	Large = 24,
	Big = 32
};

enum Source {
	Idle = 1
};

#endif