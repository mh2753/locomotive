//#ifndef INVENTORY_H_
#define INVENTORY_H_

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
#define RED        1.0, 0.0, 0.0
#define GREEN    0.0, 1.0, 0.0
#define YELLOW    1.0, 1.0, 0.0
#define BLUE    0.0, 0.0, 1.0
#define MAGENTA    1.0, 0.0, 1.0
#define CYAN    0.0, 1.0, 1.0
#define WHITE    1.0, 1.0, 1.0
#define GREY     0.5, 0.5, 0.5
#define DIM     0.2, 0.2, 0.2

extern vector<Face> faces;
extern vector<Vector3DH> normals;
extern vector<Vector3DH> orig_pts;
extern vector<Vector3DH> pts;

extern vector<Object3D> objects;
//////// game state //////////////
extern vector<Object3D> inventory;
extern GLfloat ka[4];
extern GLfloat kd[4];
extern GLfloat ks[4];

//#endif
