#ifndef CLASSDEF_H_
#define CLASSDEF_H_

#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <iostream>

using namespace std;

class Matrix3DH
{
	double arr[4][4];

public:

	Matrix3DH()	//initialize matrix to identity matrix
	{
		for (int i=0;i<4;i++) for (int j=0;j<4;j++)
			if(i==j) arr[i][j]=1.0; else arr[i][j]=0.0;
	}

	void setTranslation(double tx, double ty, double tz)
	{
		arr[0][3]=tx;
		arr[1][3]=ty;
		arr[2][3]=tz;
	}

	void setScaling(double sx, double sy, double sz)
	{
		arr[0][0]=sx;
		arr[1][1]=sy;
		arr[2][2]=sz;
	}

	void setZRotation(double theta)
	{
		arr[0][0]=cos(theta);
		arr[0][1]=-sin(theta);
		arr[1][0]=sin(theta);
		arr[1][1]=cos(theta);
	}

	void setXRotation(double theta)
	{
		arr[1][1]=cos(theta);
		arr[1][2]=-sin(theta);
		arr[2][1]=sin(theta);
		arr[2][2]=cos(theta);
	}

	void setYRotation(double theta)
	{
		arr[0][0]=cos(theta);
		arr[0][2]=sin(theta);
		arr[2][0]=-sin(theta);
		arr[2][2]=cos(theta);
	}

	void multiply(Matrix3DH* m);


	friend class Vector3DH;
};

class Vector3DH
{
	public:
	double x,y,z,w;

	Vector3DH() {x=y=z=0;w=1;}

	Vector3DH(double x,double y,double z,double w)
	{
		this->x=x;
		this->y=y;
		this->z=z;
		this->w=w;
	}

	Vector3DH applyTransform(Matrix3DH *m);

	double magnitude();

	double angle(Vector3DH v);

	string print()
	{
		stringstream buff(stringstream::in|stringstream::out);
		buff<<"("<<x<<","<<y<<","<<z<<")";
		return buff.str();

	}

	Vector3DH crossProduct(Vector3DH v)
	{
		return Vector3DH(y*v.z-z*v.y,
						 z*v.x-x*v.z,
						 x*v.y-y*v.x,
						 1.0);
	}

};

class Face
{
public:
	vector<int> vertices;
	vector<int> tVertices;
	vector<int> nVertices;
	int matIndex;
	int texIndex;
	
	string print();
};

class Material
{
	public:
	string name;
	GLfloat ka[4];
	GLfloat kd[4];
	GLfloat ks[4];
	
	int theight, twidth;
	void *texture;
	
	Material()
	{
		texture = NULL;
		ka[0]=ka[1]=ka[2]=1;
		kd[0]=kd[1]=kd[2]=1;
		ks[0]=ks[1]=ks[2]=1;
		ka[3]=ks[3]=kd[3]=1.0;
		theight=twidth=0;
	}
	
	string print();
	
	//~Material(){cout<<"freeing material"<<endl;free(texture);}
};

class AABB
{
	public:
	Vector3DH min;
	Vector3DH max;
	
	bool intersects(AABB* box);
	string print(string);
	void display();
//	BoundingBox();
};

class Object3D
{
	public:
	string name;
	vector<Vector3DH> vertices;	
	vector<Vector3DH> tVertices;
//	vector<Vector3DH> nVertices;
	
	vector<Face> faces;
	bool movable;
	bool canPickup;
	int hitPoints;
	Object3D* parent;
	Object3D* leftmostChild;
	Object3D* rightSibling;
	AABB bBox;
	
	Object3D();
	void setAABB();
	void translate(double tx, double ty, double tz);
	void addChild(Object3D*);
	void rotateAboutY(double angle, Vector3DH* point);
	bool deleteChild(string name);
	void draw();
	string print();
	
	private:
	void translateRecursive(double tx, double ty, double tz);
};

#endif /*CLASSDEF_H_*/

