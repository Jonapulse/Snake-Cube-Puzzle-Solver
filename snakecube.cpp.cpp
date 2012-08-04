

#include <iostream>
#include <string>
//simpio.h is a library used in intro CS classes at Stanford that has convenient shortcut functions.
#include "simpio.h"
#include "vector.h"
using namespace std;

enum Direction{
	MINUS_X, MINUS_Y, MINUS_Z, PLUS_X, PLUS_Y, PLUS_Z
};

int snakeSolver(int maxX, int maxY, int maxZ, string config);
bool snakeMaker(int maxX, int maxY, int maxZ, Vector<Vector<Vector<bool> > > cubeSpace, string configLeft, 
				int x, int y, int z, Direction dir);
void updateXYZ(int & x, int & y, int & z, Direction dir, int state);
Direction turnDir(Direction dir, int whichTurn);
void debugDrawCube(int numsLeft, Vector<Vector<Vector<bool> > > & cube, int maxX, int maxY, int maxZ);


int main() {
   cout << "Good morning, Sir or Madam.  It has come to our attention that you have received a "
	   << "snake cube puzzle, and the staff doesn't want to see you waste your time solving it if "
	   << "it isn't possible.  If you wouldn't mind...?" << endl;
   int maxX = 3; int maxY = 3; int maxZ = 3;
   if(snakeSolver(maxX, maxY, maxZ, "001110110111010111101010100")==1){
	   cout << "It's solvable, Sir (or Madam.)";
   } else {
	   cout << "You've been tricked!  It isn't solvable at all!";
   }
   return 0;
}

int snakeSolver(int maxX, int maxY, int maxZ, string config){
	if(config.length() != maxX*maxY*maxZ) return 0;
	Vector<Vector<Vector<bool> > > cubeSpace(maxX, Vector<Vector<bool> >(maxY, Vector<bool>(maxZ)));
	for(int i = 0; i < maxX; i++){
		for(int j = 0; j < maxY; j++){
			for(int k = 0; k < maxZ; k++){
				cubeSpace[i][j][k] = true;
				if(i - 1 > 0){
					if(snakeMaker(maxX, maxY, maxZ, cubeSpace, config.substr(1), i, j, k, MINUS_X)) return 1;
				}
				if(i + 1 < maxX){
					if(snakeMaker(maxX, maxY, maxZ, cubeSpace, config.substr(1), i, j, k, PLUS_X)) return 1;
				}
				if(j - 1 > 0){
					if(snakeMaker(maxX, maxY, maxZ, cubeSpace, config.substr(1), i, j, k, MINUS_Y)) return 1;
				}
				if(j + 1 < maxY){
					if(snakeMaker(maxX, maxY, maxZ, cubeSpace, config.substr(1), i, j, k, PLUS_Y)) return 1;
				}
				if(k - 1 > 0){
					if(snakeMaker(maxX, maxY, maxZ, cubeSpace, config.substr(1), i, j, k, MINUS_Z)) return 1;
				}
				if(k + 1 < maxZ){
					if(snakeMaker(maxX, maxY, maxZ, cubeSpace, config.substr(1), i, j, k, PLUS_Z)) return 1;
				}
				cubeSpace[i][j][k] = false;
			}
		}
	}
	//if false
	return 0;
}

/* Recursive: draws all possible extensions of the snake cubes fitting the input configuration
* string, but ceases drawing if what is drawn ever leaves the bool cube or overlaps with previously
* drawn cubes.  
*/
bool snakeMaker(int maxX, int maxY, int maxZ, Vector<Vector<Vector<bool> > > cubeSpace, string configLeft, 
				int x, int y, int z, Direction dir){
	while(configLeft[0] == '0'){
		updateXYZ(x, y, z, dir, 1);
		if(x < 0 || x >= maxX || y < 0 || y >= maxY || z < 0 || z >= maxZ || cubeSpace[x][y][z] == true){
			return false;
		}
		else{
			cubeSpace[x][y][z] = true;
			configLeft = configLeft.substr(1);
			debugDrawCube(configLeft.length(), cubeSpace, maxX, maxY, maxZ);
			if(configLeft.length() == 0) return true;
		}
	}
	updateXYZ(x, y, z, dir, 1);
	if(x < 0 || x >= maxX || y < 0 || y >= maxY || z < 0 || z >= maxZ || cubeSpace[x][y][z] == true){
		return false;
	}
	cubeSpace[x][y][z] = true;
	configLeft = configLeft.substr(1);
	debugDrawCube(configLeft.length(), cubeSpace, maxX, maxY, maxZ);
	//if(configLeft.length() == 0) return true;
	for(int i = -2; i <= 2; i++){
		if(i==0) continue;
		Direction newDir = turnDir(dir,i);
		updateXYZ(x, y, z, newDir, 1);
		if(x >= 0 && x < maxX && y >= 0 && y < maxY && z >= 0 && z < maxZ && cubeSpace[x][y][z]==false) {
			updateXYZ(x, y, z, newDir, -1);
			if(snakeMaker(maxX, maxY, maxZ, cubeSpace, configLeft, x, y, z, newDir)) return true;
		} else {
			updateXYZ(x, y, z, newDir, -1);
		}
	}
	return false;
}

/* updates x,y,z based on the direction.  If state is -1 it reverses the direction.
*/
void updateXYZ(int & x, int & y, int & z, Direction dir, int state){
	if(state == -1) dir = turnDir(dir,3);
	if(dir==MINUS_X) x--; if(dir==PLUS_X) x++; if(dir==MINUS_Y) y--;
	if(dir==PLUS_Y) y++; if(dir==MINUS_Z) z--; if(dir==PLUS_Z) z++;
}

/* allows changes in the Direction enum to wrap around, for instance PLUS_Y + 2 = MINUS_X.
* A Direction + 3 returns the opposite direction.
*/
Direction turnDir(Direction dir, int whichTurn){
	return Direction((dir + whichTurn) % 6);
}

void debugDrawCube(int numsLeft, Vector<Vector<Vector<bool> > > & cube, int maxX, int maxY, int maxZ){
	cout << "blocks left: " << numsLeft << endl;
	for(int i = 0; i < maxX; i++){
		for(int j = 0; j < maxY; j++){
			for(int k = 0; k < maxZ; k++){
				if(cube[i][j][k] == true) cout << "T";
				else cout << "F";
			}
			cout << endl;
		}
		cout << endl << endl;
	}
	cout << "----------" << endl;
}
