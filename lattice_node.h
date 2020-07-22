#include "vec3.h"
class lattice_node{
public:
	P3 place; //λ�� | Place of node
	P3 velocity; //�ٶ� | Velocity of node
	P3 force; // ���� | Force of node
	double mass; //���� | mass of node
	bool moveable; //�Ƿ�����ƶ� | Whether being eligable to move

	lattice_node()
	{
		place = P3(0.0, 0.0, 0.0);
		velocity = P3(0.0, 0.0, 0.0);
		force = P3(0.0, 0.0, 0.0);
		mass = 0.0;
		moveable = true;
	}
};