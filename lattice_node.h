#include "vec3.h"
class lattice_node{
public:
	P3 place; //位置 | Place of node
	P3 velocity; //速度 | Velocity of node
	P3 force; // 受力 | Force of node
	double mass; //质量 | mass of node
	bool moveable; //是否可以移动 | Whether being eligable to move

	lattice_node()
	{
		place = P3(0.0, 0.0, 0.0);
		velocity = P3(0.0, 0.0, 0.0);
		force = P3(0.0, 0.0, 0.0);
		mass = 0.0;
		moveable = true;
	}
};