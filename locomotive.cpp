/* ASSIGNMENT NO. : 1
 * SUBMITTED BY   : MUSTAFA HUSSAIN
 * ENTRY NO.      : 2008JCA2056
 */

#include<GL/glut.h>
#include<math.h>
#include<string>
#include<string.h>
#include <fstream>
#include"classdef.h"
#include<sstream>
#include<vector>

#include <iostream>
using namespace std;

#define WIDTH 720
#define HEIGHT 1280
#define DEPTH 500
#define DRAW 0
#define ROTATE 1
#define SCALE 2
#define TRAN 3
#define CAMERA 10.0
#define MAX_NODES 20000
#define PI 3.141592654

#define BLACK   0.0, 0.0, 0.0
#define RED		1.0, 0.0, 0.0
#define GREEN	0.0, 1.0, 0.0
#define YELLOW	1.0, 1.0, 0.0
#define BLUE	0.0, 0.0, 1.0
#define MAGENTA	1.0, 0.0, 1.0
#define CYAN	0.0, 1.0, 1.0
#define WHITE	1.0, 1.0, 1.0
#define GREY 	0.5, 0.5, 0.5
#define DIM 	0.2, 0.2, 0.2

vector<Face> faces;
vector<Vector3DH> normals;
vector<Vector3DH> orig_pts;
vector<Vector3DH> pts;

vector<Object3D> objects;

//vector<Vector3DH> pts;
//vector<Vector3DH> orig_pts;

Vector3DH dragStart, dragEnd;
Matrix3DH transform;
int numVertices=0, numFaces=0;

GLdouble theta = 0.0;

GLfloat ka[4];
GLfloat kd[4];
GLfloat ks[4];


//String constants for displaying messages on screen
char str[5][100]= {"Draw - Click four points to draw a quad",
				   "Rotate - Drag mouse to rotate [t:translate s:scale]",
				   "Scale - l-click expand, r-click shrink [t:translate r:rotate]",
				   "Translate - Drag to translate [r:rotate s:scale]"
				   };

static int state=ROTATE;		//current state i.e. current mode of operation.


void display()
{
//	cout<<"Inside display"<<endl;//testing

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);	
	glColor3f(BLACK);
	
//	glBegin(GL_LINES);
//		glVertex3f(100,0,0);
//		glVertex3f(-100,0,0);
//	glEnd();	

	glLoadIdentity();	
	theta+=5.0;

	glPushMatrix();	
	glTranslatef(0.308,-0.03,-0.737);	
	glRotatef(theta,0.1f,0.0f,0.0f);
	glTranslatef(-0.308,0.03,0.737);	
	objects[15].draw();

	glPushMatrix();	
	glLoadIdentity();		
	glTranslatef(0.308,-0.03,-3.647);	
	glRotatef(theta,0.1f,0.0f,0.0f);
	glTranslatef(-0.308,0.03,3.647);	
	objects[14].draw();
	
	
	glPushMatrix();	
	glLoadIdentity();		
	glTranslatef(0.314,-0.028,-6.576);	
	glRotatef(theta,0.1f,0.0f,0.0f);
	glTranslatef(-0.314,0.028,6.576);	
	objects[13].draw();	

	glPushMatrix();	
	glLoadIdentity();		
	
	GLdouble yXlate = 0.5 * (cos(theta*PI/180.0) - 1.0 );
	GLdouble zXlate = 0.5 * (sin(theta*PI/180.0) - 1.0 );
	
	
	glTranslatef(0.0,yXlate ,zXlate + 0.5);	
	
//	cout<<cos((theta-3.0)*PI/180.0)<<","<<cos(theta*PI/180.0)<<endl;

	objects[12].draw();	
	
	glPushMatrix();	
	glLoadIdentity();		
	
//	zXlate = 0.5 * (sin(theta*PI/180.0) );
	yXlate = 0.5 * (cos(theta*PI/180.0));

	double adj =  0.5 * (cos(theta*PI/180.0));
	double hyp = 4.96274813;
	
//	cout<<acos(yXlate/hyp)*180/PI<<endl;
	
	zXlate = hyp * sin((double)acos(adj/hyp)) - 4.938 + 0.5 * (sin(theta*PI/180.0) );
	
//	cout<<acos(adj/hyp)*180/PI<<endl; 
	
	glTranslatef(0.0,0.0 ,zXlate);		
//	cout<<cos((theta-3.0)*PI/180.0)<<","<<cos(theta*PI/180.0)<<endl;

	objects[0].draw();		
	
	glPushMatrix();	
	glLoadIdentity();		
	
	zXlate = 0.5 * (sin(theta*PI/180.0) );
	yXlate = 0.5 * (cos(theta*PI/180.0));	
	
	glTranslatef(-0.511875,0.019294,1.267070);	
	glRotatef(90 - acos(adj/hyp)*180/PI -5.7824 ,0.1f,0.0f,0.0f);
	glTranslatef(0.511875,-0.019294,-1.267070);	
	
	glTranslatef(0.0,0.0 ,zXlate);	

	
	
//	cout<<cos((theta-3.0)*PI/180.0)<<","<<cos(theta*PI/180.0)<<endl;

	objects[11].draw();		

	glPushMatrix();	
	glLoadIdentity();	
	
	for (int o=1;o<11;o++)
	{
		objects[o].draw();
	}

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();
	glPopMatrix();		
	glPopMatrix();			
	glPopMatrix();				
	glPopMatrix();					
	// Flush drawing commands
	glutSwapBuffers();
	usleep(25000);	
	glutPostRedisplay();	
}

void doTranslation(int x, int y, int z)
{
	Matrix3DH translate;
	translate.setTranslation(x,y,z);

	//apply the matrix to matrix so far.
	transform.multiply(&translate);

	for (int i=0;i<numVertices;i++)
	{
		pts[i] = orig_pts[i].applyTransform(&transform);
	}
}	


void doMouse(int button, int mouseState, int x, int y)
{
	if (state == TRAN)
	{
		//get the start and end point of user drag to find the translation vector.
		if (button==GLUT_LEFT_BUTTON && mouseState==GLUT_DOWN)
		{
			dragStart.x = (x - HEIGHT);
			dragStart.y = (WIDTH - y);
		}
		else if (button==GLUT_LEFT_BUTTON && mouseState==GLUT_UP)
		{
			dragEnd.x = (x - HEIGHT);
			dragEnd.y = (WIDTH - y);

			//dx and dy are the displacements along x and y axis for the transform.
			float dx = ((float)dragEnd.x - dragStart.x)*2*CAMERA/WIDTH;
			float dy = ((float)dragEnd.y - dragStart.y)*2*CAMERA/HEIGHT;

			doTranslation(dx,dy,0);

			glutPostRedisplay();
		} 
	}
	else if (state==ROTATE)
	{
		if (button==GLUT_LEFT_BUTTON && mouseState==GLUT_DOWN)
		{
			dragStart.x = ((float)(x - HEIGHT/2)/WIDTH);
			dragStart.y = ((float)(WIDTH/2 - y)/HEIGHT);
			dragStart.z = (sqrt(1-dragStart.x*dragStart.x-dragStart.y*dragStart.y));
		}
		else if (button==GLUT_LEFT_BUTTON && mouseState==GLUT_UP)
		{
			float a,b;
			dragEnd.x = ((float)(x - WIDTH/2)/WIDTH);
			dragEnd.y = ((float)(HEIGHT/2 - y)/HEIGHT);	
			dragEnd.z = (sqrt(1-dragEnd.x*dragEnd.x-dragEnd.y*dragEnd.y));	
			
			Vector3DH axis = dragStart.crossProduct(dragEnd);
//			cout<<axis.print().c_str()<<endl;//testing
			Vector3DH uprime(0.0f,axis.y,axis.z,1.0);
			
			Vector3DH z(0.0f,0.0f,1.0f,1.0);
			Matrix3DH alpha,ralpha;
			a = uprime.angle(z);
			alpha.setXRotation(a);
			ralpha.setXRotation(-a);
			
			Vector3DH udprime = axis.applyTransform(&alpha);
			Matrix3DH beta,rbeta;
			b = udprime.angle(z);
			beta.setYRotation(b);
			rbeta.setYRotation(-b);
			
			Matrix3DH theta;
			theta.setZRotation(dragStart.angle(dragEnd));
//			theta.setYRotation(dragStart.angle(dragEnd));
			
			transform.multiply(&alpha);
			transform.multiply(&beta);
			transform.multiply(&theta);
			transform.multiply(&rbeta);
			transform.multiply(&ralpha);
			
			for (int i=0;i<numVertices;i++)
			{
				pts[i] = orig_pts[i].applyTransform(&transform);
			}
			//reset the transform matrix to an Identity matrix.
			glutPostRedisplay();
			
		}			
	}
}

void setRC()
{
	glClearColor(WHITE,1.0f);
	glColor3f (BLACK);
	glLineWidth(3);
	
	
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(80.0, 1.6, 1.0, 2000.0);
	gluLookAt (7.0, 0.0, 0.0,
	 		   0.0, 0.0, 0.0, 
	 		   0.0, 1.0, 0.0);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glEnable(GL_LIGHTING);
	glFrontFace(GL_CW);

	GLfloat light0_ambient[] = { BLACK, 0.5 };
	GLfloat light0_diffuse[] = { DIM, 0.5 };
	GLfloat light0_specular[] = { DIM, 0.5 };
	GLfloat light0_position[] = { 0, 0, 5, 5 };
//	GLfloat spot_direction[] = {0.1,0.0,0.1};
	GLfloat spot_position[4]={5,5,5,1};	
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, spot_position);
	glEnable(GL_LIGHT0);	
	
	ka[0]=ka[1]=ka[2]=0.5;
	kd[0]=kd[1]=kd[2]=0.5;
	ks[0]=ks[1]=ks[2]=0.5;
	ka[3]=ks[3]=kd[3]=1.0;
}

void readOBJOld(char* fileName)
{
	ifstream inFile;
	
	inFile.open(fileName);
	
	if (!inFile) 
	{
    	cerr << "Unable to open file "<<fileName<<endl;
    	exit(1);   // call system to stop
	}
	
	string line;
	
	int line_no = 1;
	
	while(true)
	{
		getline(inFile,line);
		if (inFile.eof()) break;
	
//		cout<<"Reading line "<<line_no<<endl; cout.flush();
		line_no++;
			
		if (line[0]=='#') continue;	//skip comments
		
		if (line[0]=='v' || line[0]=='V')
		{
			float x,y,z;
			stringstream s(line);
			string temp;
			s>>temp>>x>>y>>z;
			orig_pts.push_back(Vector3DH(x,y,z,1));
			pts.push_back(Vector3DH(x,y,z,1));
			numVertices++;
		}
		
		if (line[0]=='f' || line[0]=='F')
		{
			char * token;
			int nv=0;
			char str[100];
			strcpy(str,line.c_str());
			token = strtok(str," ");			
			while(token!=NULL)
			{
				token=strtok(NULL," ");
				nv++;
			}
			
			Face f;
			
			stringstream s(line);
			string temp;
			s>>temp;		//discard f
			for (int i=0;i<nv-1;i++)
			{
				int v;
				s>>v;
				f.vertices.push_back(v-1);
			}
			
			faces.push_back(f);
			numFaces++;
			cout<<faces[numFaces-1].print().c_str()<<endl;//testing
		}
	}
}				

void readOBJ(char* fileName)
{
	cout<<"Reading obj file"<<endl;//testing
	
	ifstream inFile;

	inFile.open(fileName);

	if (!inFile)
	{
    	cerr << "Unable to open file "<<fileName<<endl;
    	exit(1);   // call system to stop
	}

	string buffer, identifier;
	
//	int vi=0,fi=0;
	float x,y,z;
	int mi = -1;					//material index for a face. 
	int vadj = 0, vc=0;
	int tadj = 0, tc=0;
	int nadj = 0, nc=0;
		
	while (inFile.good() && (inFile >> identifier))
	{
		if (identifier == "#")		//comment
		{				
			;//do nothing
		}
		else if (identifier == "o")		
		{
			objects.push_back(Object3D());
			vadj+=vc;
			vc =0;
			tadj+=tc;
			tc =0;
//			nadj+=nc;
//			nc=0;
			
			inFile>>objects.back().name;

//			cout<<"on object "<<objects.back().name<<endl;cout.flush();//testing
			
		}		
		else if (identifier == "v")		// vertex data
		{
			inFile >> x >> y >> z;
			orig_pts.push_back(Vector3DH(x,y,z,1));
			pts.push_back(Vector3DH(x,y,z,1));
		}
		else if(identifier == "vn")	// vertex normal
		{
			inFile >> x >> y >> z;
			normals.push_back(Vector3DH(x,y,z,1));			
//			hasNormalData = TRUE;
		}
		else if (identifier == "f")	//polygon face
		{
			Face f;

			int i = 0;
			int vIndex, tIndex, nIndex;
			char c;

			getline(inFile, buffer);
			stringstream line(buffer);
			while(line.good())
			{
				line >> vIndex >> c >> c >> nIndex;
				f.vertices.push_back(vIndex - 1);					
				f.nVertices.push_back(nIndex - 1);
			}

//			faces.push_back(f);
			objects.back().faces.push_back(f);			
//			cout<<faces[faces.size()-1].print().c_str()<<endl;//testing
		}//face processing ends
	}
	
	inFile.close();
	
	cout<<"done reading obj file"<<endl;cout.flush();//testing
}

void keyboard(unsigned char key, int x, int y)
{
	//Change the state based on the key input by the user. 
	if (state!=DRAW)
	{
		switch (key)
		{
			case 'r': state = ROTATE;break;
			case 't': state = TRAN;break;
			case 's': state = SCALE;break;		
			case 27: exit(0);break;						 
		}
		glutPostRedisplay();
	}	
}

void specialKeys(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			if (state == SCALE)
			{
				doTranslation(0,0,CAMERA/10);
			}
			else if (state == TRAN)
			{
				doTranslation(0,-CAMERA/10,0);
			}
			break;
		case GLUT_KEY_DOWN: 
			if (state == SCALE)
			{
				doTranslation(0,0,-CAMERA/10);
			}
			else if (state == TRAN)
			{
				doTranslation(0,CAMERA/10,0);
			}		
			break;
		case GLUT_KEY_RIGHT:
			if (state == TRAN)
			{
				doTranslation(-CAMERA/10,0,0);
			}		
			break;
		case GLUT_KEY_LEFT:
			if (state == TRAN)
			{
				doTranslation(CAMERA/10,0,0);
			}		
			break;			
		}
		glutPostRedisplay();
} 
		

int main(int argc, char* argv[])
{
	//Initial setup
	
	if (argc!=2)
	{
		cout<<"Usage: opengl <object.obj>"<<endl;
		return 0;
	}
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE |GLUT_DEPTH);	
//	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	glutInitWindowSize(HEIGHT,WIDTH);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Assignment No. 1");

	readOBJ(argv[1]);
	setRC();

	glutDisplayFunc(display);
	glutMouseFunc(doMouse);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	
//	glutReshapeFunc(reshape);
	//set window cordinates

	glutMainLoop();

	return 0;
}
