#include <gl/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <string>
using namespace std;

class ObjLoader{

public:

	ObjLoader(string filename);
	void Draw();

private:

	vector<vector<GLfloat>>vSets;
	vector<vector<GLint>>fSets;

};

