namespace speed {

	enum {
		Slow = 40,
		Normal = 60,
		Fast = 80,
		Fastest = 100
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