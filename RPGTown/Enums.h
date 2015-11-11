namespace speed {

	enum {
		Slow = 1,
		Normal = 4,
		Fast = 6,
		Fastest = 8
	};

}

// design and implement these states

namespace states {
	enum {
		PAUSE,
		IDLE,
		WALKING,
		DEATH
	};
}

// include other defined enums here (player enums)

enum Direction {
	South, West, East, North, Null
};

namespace Layer {
	enum {
		Background_1,
		Background_2,
		Field,
		Collision_Objects,
		Collision_Boxes,
		Event_Layer,
		Overlay
	};
}