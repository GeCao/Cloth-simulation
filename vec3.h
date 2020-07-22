#ifndef __VEC3_H__
#define __VEC3_H__ 
using namespace std;
class P3{
public:
	double x, y, z;

	//初始化函数
	P3(double x_0 = 0, double y_0 = 0, double z_0 = 0) : x(x_0), y(y_0), z(z_0){}

	//算子重载：
	//===================================
	//====        "+-" for P3        ====
	//===================================
	P3 operator+(const P3& a)const {
		return P3(a.x + x, a.y + y, a.z + z);
	}
	P3 operator-(const P3& a)const {
		return P3(x - a.x, y - a.y, z - a.z);
	}
	P3& operator+=(const P3&a) { return *this = *this + a; }
	P3& operator-=(const P3&a) { return *this = *this - a; }
	P3& operator*=(double p) { return *this = *this * p; }
	P3& operator/=(double p) { return *this = *this / p; }
	//===================================
	//====    "+-*/" for constant    ====
	//===================================
	P3 operator+(const double a)const {
		return P3(a + x, a + y, a + z);
	}
	P3 operator-(const double a)const {
		return P3(x - a, y - a, z - a);
	}
	P3 operator*(const double a)const {
		return P3(a * x, a * y, a * z);
	}
	P3 operator/(const double a)const {
		if (a == 0){ std::cout << "There's a problem that a number divides 0!" << endl; exit(1); }
		return P3(x / a, y / a, z / a);
	}
	//===================================
	//====         By reverse        ====
	//===================================
	P3 operator-()const {
		return P3(-x, -y, -z);
	}


	friend ostream& operator<<(ostream &os, const P3& a)
	{
		os << "Point/Vector::(" << a.x << ", " << a.y << ", " << a.z << " )";
		return os;
	}


	bool operator==(const P3&a) const { return x == a.x && y == a.y && z == a.z; }
	bool operator!=(const P3&a) const { return x != a.x || y != a.y || z != a.z; }

	//函数部分
	double maximum() const {//返回三维向量中的最大值
		return x>y&&x>z ? x : y>z ? y : z; 
	}
	double dot(const P3& a, const P3& b)const {//向量的点乘
		return (a.x*b.x + a.y*b.y + a.z*b.z);
	}
	double dot(const P3& a)const {//向量的点乘
		return (a.x*x + a.y*y + a.z*z);
	}
	P3 mult(const P3&a) const { return P3(x*a.x, y*a.y, z*a.z); }
	double length()const {//向量的模长
		return sqrt(x*x + y*y + z*z);
	}
	double length_2()const {//向量模长的平方
		return (x*x + y*y + z*z);
	}
	P3 norm()const {//向量的正则化
		double n_norm = this->length();
		return (*this) / n_norm;
	}
	P3 cross_product(const P3& a, const P3& b)const {//向量的叉乘
		return P3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
	}
	P3 cross_product(const P3& a)const {//向量的叉乘
		return P3(y*a.z - z*a.y, z*a.x - x*a.z, x*a.y - y*a.x);
	}
	P3 clip(double r0 = 0, double r1 = 1) const { //进行范围内的切片
		return P3(x>r1 ? r1 : x<r0 ? r0 : x,
			y>r1 ? r1 : y<r0 ? r0 : y,
			z>r1 ? r1 : z<r0 ? r0 : z); 
	}
	

	//光路部分
	P3 reflect(const P3& n)const {//光的反射
		//Part1. 光路射入平面：theta_i belongs (90,180)，那么反射光会射出平面
		//Part2. 光路射出平面：theta_i belongs (0,90)，那么反射光会射入平面
		double cos_theta_i = this->dot(n.norm());
		return -(n*(2.0*cos_theta_i) - *this);//在原始公式的基础【wr = 2n(n*wi) - wi】上加了一个负号，使得总有一个射入与一个射出。
	}
	P3 refract(const P3&n, double ni, double nr) const { //光的折射
		double cos_theta_i = this->dot(n.norm());
		double ni_r = ni / nr;
		double cosr2 = 1. - ni_r*ni_r*(1 - cos_theta_i*cos_theta_i);
		if (cosr2 <= 0){//全反射
			return P3();
		}
		double cosr = sqrt(cosr2);
		if (cos_theta_i < 0){//入射光线使得根号下的项变号
			cosr = -cosr;
		}
		return ((*this)*ni_r - n*(ni_r*cos_theta_i - cosr)).norm();
	}
};

#endif // __VEC3_H__