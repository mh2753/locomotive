#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include<math.h>
#include<string>
#include<string.h>
#include <fstream>
#include"classdef.h"
#include"globalv.h"
#include<sstream>
#include<vector>
#include<unistd.h>

#include <iostream>
using namespace std;

vector<Face> faces;
vector<Vector3DH> normals;
vector<Vector3DH> orig_pts;
vector<Vector3DH> pts;

vector<Object3D> objects;

Vector3DH dragStart, dragEnd;
int numVertices=0, numFaces=0;

GLdouble theta = 0.0;

GLfloat ka[4];
GLfloat kd[4];
GLfloat ks[4];

void processMiscObjects(double theta) 
{
    glPushMatrix();    
    glLoadIdentity();    
    
    for (int o=1;o<11;o++)
    {
        objects[o].draw();
    }
}

void processHypotenusRod(double theta)
{
    double zXlate = 0.5 * (sin(theta*PI/180.0) );
    double yXlate = 0.5 * (cos(theta*PI/180.0));    
    double adj =  0.5 * (cos(theta*PI/180.0));
    double hyp = 4.96274813;
    
    glPushMatrix();    
    glLoadIdentity();        
    
    glTranslatef(0.0,0.0 ,zXlate);    

    glTranslatef(-0.511875,0.019294,1.267070);    
    glRotatef(90 - acos(adj/hyp)*180/PI -5.7824 ,0.1f,0.0f,0.0f);
    glTranslatef(0.511875,-0.019294,-1.267070);    
    
    objects[11].draw();        
}

void processPistonRod(double theta) 
{
    glPushMatrix();    
    glLoadIdentity();        
    
    double yXlate = 0.5 * (cos(theta*PI/180.0));

    double adj =  0.5 * (cos(theta*PI/180.0));
    double hyp = 4.96274813;
    
    double zXlate = hyp * sin((double)acos(adj/hyp)) - 4.938 + 0.5 * (sin(theta*PI/180.0) );
    
    glTranslatef(0.0,0.0 ,zXlate);        

    objects[0].draw();        
}

void processHorizontalRod(double theta) 
{
    glPushMatrix();    
    glLoadIdentity();        
    
    GLdouble yXlate = 0.5 * (cos(theta*PI/180.0) - 1.0 );
    GLdouble zXlate = 0.5 * (sin(theta*PI/180.0) - 1.0 );
    
    glTranslatef(0.0,yXlate ,zXlate + 0.5);    
    
    objects[12].draw();    
}

void processWheels(double theta) 
{
    glPushMatrix();    
    //glLoadIdentity(); //TODO Do we need this
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
}

/*
-y    x
|    /
|   /
|  /
| /
|/
o-------------+z
*/
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);    
    glColor3f(BLACK);
    
    glLoadIdentity();    
    theta+=5.0;

    processWheels(theta);

    processHorizontalRod(theta); 

    processPistonRod(theta);
    
    processHypotenusRod(theta);

    processMiscObjects(theta);
    
    for (int i=0;i<8;i++) { 
        glPopMatrix();
    }
    
    // Flush drawing commands
    glutSwapBuffers();
    usleep(27500);    
    glutPostRedisplay();    
}

void setRC()
{
    glClearColor(WHITE,1.0f);
    glColor3f (BLACK);
    glLineWidth(3);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60.0, 1.6, 1.0, 2000.0);

    gluLookAt (8.0, 2.0, -1.5,
                0.0, 1.5, -1.5, 
                0.0, 0.5, 0.0);    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glEnable(GL_LIGHTING);
    glFrontFace(GL_CW);

    GLfloat light0_ambient[] = { BLACK, 0.5 };
    GLfloat light0_diffuse[] = { DIM, 0.5 };
    GLfloat light0_specular[] = { DIM, 0.5 };
    GLfloat light0_position[] = { 0, 0, 5, 5 };
//    GLfloat spot_direction[] = {0.1,0.0,0.1};
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
    
    float x,y,z;
    int mi = -1;                    //material index for a face. 
    int vadj = 0, vc=0;
    int tadj = 0, tc=0;
    int nadj = 0, nc=0;
        
    while (inFile.good() && (inFile >> identifier))
    {
        if (identifier == "#")        //comment
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
//            nadj+=nc;
//            nc=0;
            
            inFile>>objects.back().name;
        }        
        else if (identifier == "v")        // vertex data
        {
            inFile >> x >> y >> z;
            orig_pts.push_back(Vector3DH(x,y,z,1));
            pts.push_back(Vector3DH(x,y,z,1));
        }
        else if(identifier == "vn")    // vertex normal
        {
            inFile >> x >> y >> z;
            normals.push_back(Vector3DH(x,y,z,1));            
//            hasNormalData = TRUE;
        }
        else if (identifier == "f")    //polygon face
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

//            faces.push_back(f);
            objects.back().faces.push_back(f);            
//            cout<<faces[faces.size()-1].print().c_str()<<endl;//testing
        }//face processing ends
    }
    
    inFile.close();
    
    cout<<"done reading obj file"<<endl;cout.flush();//testing
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
    glutInitWindowSize(HEIGHT,WIDTH);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Assignment No. 1");

    readOBJ(argv[1]);
    setRC();

    glutDisplayFunc(display);
    
    glutMainLoop();

    return 0;
}
