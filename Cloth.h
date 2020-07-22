#include "lattice_node.h"
#include "const.h"
#include <time.h>
class Spring{
public:
	vector<int> indices;

	Spring(){
		indices = vector<int>{0, 0};
	}
	Spring(int i_0, int j_0){
		vector<int> temp_indices{ i_0, j_0 };
		indices = temp_indices;
	}
};
class Cloth{
public:
	lattice_node Nodes[l + 1][m + 1]; // k = j*(l + 1) + i; i = k % (l + 1); j = k / (l + 1);
	double ks; //弹簧弹性系数
	double kd; //弹簧阻尼系数
	double L0; //弹簧松弛时的长度
	vector<Spring > spring_system; //N

	Cloth()
	{
		int i = 0, j = 0, k = 0;
		
		//构建弹簧系统
		spring_system.clear();
		for (i = 0; i < l; i++)
		{
			for (j = 0; j < m; j++)
			{
				k = j*(l + 1) + i;
				spring_system.push_back(Spring(k, k + 1));
				spring_system.push_back(Spring(k + 1, k + (l + 1)));
				spring_system.push_back(Spring(k + (l + 1), k));
			}
		}
		j = m;
		for (i = 0; i < l; i++)
		{
			k = j*(l + 1) + i;
			spring_system.push_back(Spring(k, k + 1));
		}
		i = l;
		for (j = 0; j < m; j++)
		{
			k = j*(l + 1) + i;
			spring_system.push_back(Spring(k + (l + 1), k));
		}

		//设置节点的信息
		int h_a = -10, h_b = 10;
		srand((unsigned int)(time(NULL)));
		for (i = 0; i <= l; i++)
		{
			for (j = 0; j <= m; j++)
			{
				Nodes[i][j].moveable = true;
				Nodes[i][j].force = P3(0.0, 0.0, 0.0);
				Nodes[i][j].velocity = P3(0.0, 0.0, 0.0);
				Nodes[i][j].place = P3(i*0.025, j*0.025, 0.0); // ((rand() % (h_b - h_a + 1)) + h_a)*0.05);
				Nodes[i][j].mass = mass0;
			}
		}
		i = 0;
		for (j = 0; j <= m; j++)
		{
			k = j*(l + 1) + i;
			Nodes[i][j].moveable = true;
		}
		Nodes[0][0].moveable = false;
		Nodes[0][m].moveable = false;
		Nodes[l][m].moveable = false;
		Nodes[l][0].moveable = false;

		ks = const_Ks;
		kd = const_Kd;
		L0 = const_L_0;
	}

	void compute_spring_force(vector<int>& indices);

	void move_node(int indice);

	void reset_forces();

	void compute();

	void visualization();
};

void Cloth::compute_spring_force(vector<int>& indices)
{
	P3 force1, force2;
	int k1 = indices[0], k2 = indices[1];
	int i1 = k1 % (l + 1), j1 = k1 / (l + 1);
	int i2 = k2 % (l + 1), j2 = k2 / (l + 1);
	P3 x21 = Nodes[i2][j2].place - Nodes[i1][j1].place;
	P3 v21 = Nodes[i2][j2].velocity - Nodes[i1][j1].velocity;
	force1 = ((x21).norm())*(ks*(x21.length()) - L0); //弹性力
	force1 += kd*((x21).norm().dot(v21)); //阻尼力
	force2 = -force1;

	if (Nodes[i1][j1].moveable){ Nodes[i1][j1].force += force1; }
	if (Nodes[i2][j2].moveable){ Nodes[i2][j2].force += force2; }
}

void Cloth::move_node(int indice)
{
	int i = indice % (l + 1), j = indice / (l + 1);
	P3 accelerate = Nodes[i][j].force / Nodes[i][j].mass;
	Nodes[i][j].velocity += accelerate*dt;
	Nodes[i][j].place += Nodes[i][j].velocity*dt;
}

void Cloth::reset_forces()
{
	int i = 0, j = 0;
#pragma omp parallel for private(i,j)
	for (i = 0; i <= l; i++)
	{
		for (j = 0; j <= m; j++)
		{
			Nodes[i][j].force = P3(0, 0, 0);
		}
	}
}

void Cloth::compute()
{
	int i = 0, j = 0, k = 0, k1 = 0, k2 = 0;
	int n_springs = spring_system.size();

	//计算受力 | Compute forces
#pragma omp parallel for private(i)
	for (i = 0; i < n_springs; i++)
	{
		compute_spring_force(spring_system[i].indices);
	}

	//移动节点 | Move nodes
#pragma omp parallel for private(i,j)
	for (i = 0; i <= l; i++)
	{
		for (j = 0; j <= m; j++)
		{
			k = j*(l + 1) + i;
			if (Nodes[i][j].moveable){ move_node(k); }
		}
	}

	reset_forces();
}

void Cloth::visualization()
{
	int i = 0, j = 0, k = 0;
	glPushMatrix();
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(true);
	glColor4f(0.8, 0.8, 0.8, 0.4);
	if (visual_style == Wire){ glBegin(GL_LINES); }
	else if (visual_style == Solid){ glBegin(GL_TRIANGLES); }
	for (i = 0; i < l; i++)
	{
		for (j = 0; j < m; j++)
		{
			glVertex3f(Nodes[i][j].place.x, Nodes[i][j].place.y, Nodes[i][j].place.z);
			glVertex3f(Nodes[i + 1][j].place.x, Nodes[i + 1][j].place.y, Nodes[i + 1][j].place.z);
			glVertex3f(Nodes[i + 1][j + 1].place.x, Nodes[i + 1][j + 1].place.y, Nodes[i + 1][j + 1].place.z);

			if (visual_style == Wire)
			{
				glVertex3f(Nodes[i + 1][j].place.x, Nodes[i + 1][j].place.y, Nodes[i + 1][j].place.z);
				glVertex3f(Nodes[i][j].place.x, Nodes[i][j].place.y, Nodes[i][j].place.z);
				glVertex3f(Nodes[i + 1][j + 1].place.x, Nodes[i + 1][j + 1].place.y, Nodes[i + 1][j + 1].place.z);
				glVertex3f(Nodes[i][j + 1].place.x, Nodes[i][j + 1].place.y, Nodes[i][j + 1].place.z);
			}

			glVertex3f(Nodes[i][j].place.x, Nodes[i][j].place.y, Nodes[i][j].place.z);
			glVertex3f(Nodes[i + 1][j + 1].place.x, Nodes[i + 1][j + 1].place.y, Nodes[i + 1][j + 1].place.z);
			glVertex3f(Nodes[i][j + 1].place.x, Nodes[i][j + 1].place.y, Nodes[i][j + 1].place.z);
		}
	}
	glEnd();
	glPopMatrix();
}