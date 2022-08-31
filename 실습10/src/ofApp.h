#pragma once

#include "ofMain.h"
//#include "Water_line.h"
#include <cstdlib>
#include <math.h>
#define EPSILON 0.01
#define INFINITE 99999999

typedef struct _line {
	int x1, x2, y1, y2;
	double gradient;
} Line;

typedef struct _dot {
	int x1, y1;
} Dot;


class WaterLine {

public:
	WaterLine(int num_of_line, int start_x, int start_y);
	~WaterLine();

	void reset();
	//void update();
	void draw();
	void calculate_path(Line* lines, int num_of_line);
	int radius;

	Dot* path;

	ofColor color;
	int path_idx;
	//int draw_complete;
	int calc_complete;
	int draw_number;

	int start_x;
	int start_y;
	

};

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	/* WaterFall-related member variables Regions */

	// flag variables
	int draw_flag;
	int load_flag;
	int water_flag;

	// Line segment and dot related variables
	int num_of_line, num_of_dot;
	float dot_diameter;
	// additional variables
	
	Line* Lines;
	Dot* Dots;
	/*
	int* linex1;
	int* liney1;
	int* linex2;
	int* liney2;
	int* dotx1;
	int* doty1;
	*/
	//int* rankx1;
	// idx번째로 작은 x1좌표가 있는 dotx1의 인덱스값을 가짐
	int curdot;

	WaterLine* waterline;

	/* WaterFall-related member functions */
	void resetWater();

	void processOpenFileSelection(ofFileDialogResult openFileResult);
	void initializeWaterLines(); // 2nd week portion.

};



