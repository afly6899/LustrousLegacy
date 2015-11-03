namespace speed {

	enum {
		Slow = 4, // 64/speed = num iterations before you stop walking?
		Normal = 8,
		Fast = 16,
		Fastest = 32
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
	South, West, East, North
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