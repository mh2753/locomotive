#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <iostream>
#include "classdef.h"
#include "globalv.h"

#define TRUE 1
#define FALSE 0


using namespace std;

double Vector3DH::angle(Vector3DH v)
{
	if (this->magnitude()==0 || v.magnitude()==0) return 0.0;
	
	int sign=1;
	return acos((x*v.x+y*v.y+z*v.z)/(this->magnitude()*v.magnitude()));
}

double Vector3DH::magnitude()
{
	return sqrt(x*x + y*y + z*z);
}


Vector3DH Vector3DH::applyTransform(Matrix3DH* m)
{
	return Vector3DH(
					(m->arr[0][0]*x + m->arr[0][1]*y + m->arr[0][2]*z + m->arr[0][3]*w),
					(m->arr[1][0]*x + m->arr[1][1]*y + m->arr[1][2]*z + m->arr[1][3]*w),
					(m->arr[2][0]*x + m->arr[2][1]*y + m->arr[2][2]*z + m->arr[2][3]*w),
					(m->arr[3][0]*x + m->arr[3][1]*y + m->arr[3][2]*z + m->arr[3][3]*w)
					);
}


void Matrix3DH::multiply(Matrix3DH *m)
{
	double temp[4][4];

	for (int i=0;i<4;i++) for (int j=0;j<4;j++) temp[i][j]=0.0;

	for (int i=0;i<4;i++) for (int j=0;j<4;j++) for (int k=0;k<4;k++)
	{
		temp[i][j]+=arr[i][k]*m->arr[k][j];
	}

	for (int i=0;i<4;i++) for (int j=0;j<4;j++)
	{
		arr[i][j]=temp[i][j];
	}
}

string Face::print()
{
	stringstream buff(stringstream::in|stringstream::out);
	buff<<"\nvertices: ";
	for (int i=0;i<vertices.size();i++) buff<<vertices[i]+1<<" ";
	buff<<"textures: ";		
	for (int i=0;i<tVertices.size();i++) buff<<tVertices[i]+1<<" ";
	buff<<"normals: ";		
	for (int i=0;i<nVertices.size();i++) buff<<nVertices[i]+1<<" ";				
	buff<<"material index: "<<matIndex;
	return buff.str();
}

string Material::print()
{
	stringstream buff(stringstream::in|stringstream::out);
	buff<<"\nMaterial: "<<name;
	buff<<"\nka: ";
	for (int i=0;i<3;i++) buff<<ka[i]<<" ";
	buff<<"\nkd: ";		
	for (int i=0;i<3;i++) buff<<kd[i]<<" ";
	buff<<"\nks: ";		
	for (int i=0;i<3;i++) buff<<ks[i]<<" ";				
	buff<<"\ntexture: "<<" - "<<theight<<", "<<twidth;
	return buff.str();
}

Object3D::Object3D()
{
	rightSibling=NULL;
	leftmostChild = NULL;
	parent = NULL;
	movable = false;
	canPickup = false;
	hitPoints=-1;
}

void Object3D::addChild(Object3D* child)
{
	if (leftmostChild==NULL) 
	{
		leftmostChild=child;
	}
	else
	{
		Object3D* curr;
		for (curr = leftmostChild;curr->rightSibling!=NULL;curr=curr->rightSibling)
			;
		curr->rightSibling = child;
	}
}

bool Object3D::deleteChild(string dc)
{
	if (leftmostChild==NULL) 
	{
		return false;
	}
	else
	{
		if (leftmostChild->name == dc)
		{
			Object3D *rc = leftmostChild->rightSibling;
			delete leftmostChild;
			leftmostChild = rc;
			return true;
		}
		Object3D* curr,*prev=leftmostChild;
		for (curr = leftmostChild->rightSibling;curr->rightSibling!=NULL;curr=curr->rightSibling)
		{
			if (curr->name == dc)		
			{
				prev->rightSibling = curr->rightSibling; 
				delete curr;
				return true;
			}
			prev = curr;
		}
	}	
	return false;
}

void Object3D::setAABB()
{
	Face* currFace;
	
	bBox.min.x = 1000;
	bBox.min.y = 1000;
	bBox.min.z = 1000;		
	
	bBox.max.x = -1000;
	bBox.max.y = -1000;
	bBox.max.z = -1000;		
 	
 	for (int i=0;i<this->vertices.size();i++)
 	{
 		if (vertices[i].x < bBox.min.x) bBox.min.x = vertices[i].x;
 		if (vertices[i].y < bBox.min.y) bBox.min.y = vertices[i].y;
 		if (vertices[i].z < bBox.min.z) bBox.min.z = vertices[i].z;

 		if (vertices[i].x > bBox.max.x) bBox.max.x = vertices[i].x;
 		if (vertices[i].y > bBox.max.y) bBox.max.y = vertices[i].y;
 		if (vertices[i].z > bBox.max.z) bBox.max.z = vertices[i].z;
 	}
 		
// 	cout<<"bb for object "<<this->name.c_str()<<bBox.min.print().c_str()<<bBox.max.print().c_str()<<endl;//testing
}

void Object3D::translate(double tx, double ty, double tz)
{
	for (int i=0;i<this->vertices.size();i++)
	{
		vertices[i].x+=tx;
		vertices[i].y+=ty;
		vertices[i].z+=tz;
	}
	
	bBox.min.x+=tx;
	bBox.min.y+=ty;
	bBox.min.z+=tz;
	
	bBox.max.x+=tx;
	bBox.max.y+=ty;
	bBox.max.z+=tz;
	
	if (leftmostChild!=NULL) leftmostChild->translateRecursive(tx,ty,tz);
}

void Object3D::translateRecursive(double tx, double ty, double tz)
{
	for (int i=0;i<this->vertices.size();i++)
	{
		vertices[i].x+=tx;
		vertices[i].y+=ty;
		vertices[i].z+=tz;
	}
	
	bBox.min.x+=tx;
	bBox.min.y+=ty;
	bBox.min.z+=tz;
	
	bBox.max.x+=tx;
	bBox.max.y+=ty;
	bBox.max.z+=tz;
	
	if (rightSibling!=NULL) rightSibling->translateRecursive(tx,ty,tz);
	if (leftmostChild!=NULL) leftmostChild->translateRecursive(tx,ty,tz);
}

void Object3D::rotateAboutY(double angle, Vector3DH* point)
{
//	double tx, tz;
//	double sx = sin(angle);
//	double cx = cos(angle);
//	
//	for (int f=0;f<this->faces.size();f++)
//	{
//		for (int i=0;i<this->faces[f].vertices.size();i++)
//		{
//			tx = (this->faces[f].vertices[i].x - point->x) * cx + (this->faces[f].vertices[i].z - point->z) * sx + point->x;	
//			tz = (this->faces[f].vertices[i].z - point->z) * cx - (this->faces[f].vertices[i].x - point->x) * sx + point->z;

//			this->faces[f].vertices[i].x = tx;
//			this->faces[f].vertices[i].z = tz;
//		}
//	}
	
	setAABB();
}

string Object3D::print()
{
	stringstream buff(stringstream::in|stringstream::out);
	buff<<"\nObject: "<<name<<endl;
//	buff<<"faces: ";
//	for (int i=0;i<faces.size();i++) buff<<faces[i].print().c_str()<<" ";
	buff<<"v0 "<<vertices[0].print();
	return buff.str();
}

void Object3D::draw()
{
	for (int i=0;i<this->faces.size();i++)
	{
//		glBegin(GL_LINE_LOOP);
		glBegin(GL_POLYGON);
		for (int j=0;j<this->faces[i].vertices.size();j++)
		{
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ka);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, kd);	
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,ks);	
		
			glNormal3f(normals[this->faces[i].nVertices[j]].x, 
					   normals[this->faces[i].nVertices[j]].y, 
					   normals[this->faces[i].nVertices[j]].z);	
		
			glVertex3f(pts[this->faces.at(i).vertices[j]].x,
					   pts[this->faces.at(i).vertices[j]].y,
					   pts[this->faces.at(i).vertices[j]].z);
		}
		glEnd();
	}	

}

bool AABB::intersects(AABB* box)
{
	if(max.x<box->min.x || min.x>box->max.x)
	return false;
	if(max.y<box->min.y || min.y>box->max.y)
	return false;
	if(max.z<box->min.z || min.z>box->max.z)
	return false;	
	
	return true;
}

string AABB::print(string name)
{
	stringstream buff(stringstream::in|stringstream::out);
	buff<<"bb: "<<name<<" "<<min.print().c_str()<<","<<max.print().c_str();
	return buff.str();
}

void AABB::display()
{
	glBegin(GL_LINE);
		glVertex3f(min.x,min.y,min.z);
		glVertex3f(max.x,min.y,min.z);			
	glEnd();		

	glBegin(GL_LINE);
		glVertex3f(min.x,min.y,min.z);
		glVertex3f(min.x,max.y,min.z);			
	glEnd();		
	
	glBegin(GL_LINE);
		glVertex3f(min.x,min.y,min.z);
		glVertex3f(min.x,min.y,max.z);			
	glEnd();		
//////////
	glBegin(GL_LINE);
		glVertex3f(max.x,max.y,max.z);
		glVertex3f(min.x,max.y,max.z);			
	glEnd();		

	glBegin(GL_LINE);
		glVertex3f(max.x,max.y,max.z);
		glVertex3f(max.x,min.y,max.z);			
	glEnd();		

	glBegin(GL_LINE);
		glVertex3f(max.x,max.y,max.z);
		glVertex3f(max.x,max.y,min.z);			
	glEnd();		
///////////

	glBegin(GL_LINE);
		glVertex3f(max.x,min.y,min.z);
		glVertex3f(max.x,min.y,max.z);			
	glEnd();		

	glBegin(GL_LINE);
		glVertex3f(max.x,min.y,min.z);
		glVertex3f(max.x,max.y,min.z);			
	glEnd();		

	glBegin(GL_LINE);
		glVertex3f(max.x,max.y,min.z);
		glVertex3f(min.x,max.y,min.z);			
	glEnd();		

///////////

	glBegin(GL_LINE);
		glVertex3f(min.x,max.y,min.z);			
		glVertex3f(min.x,max.y,max.z);			
	glEnd();		

	glBegin(GL_LINE);
		glVertex3f(min.x,max.y,max.z);			
		glVertex3f(min.x,min.y,max.z);			
	glEnd();		

	glBegin(GL_LINE);
		glVertex3f(min.x,min.y,max.z);			
		glVertex3f(max.x,min.y,max.z);			
	glEnd();		

}
