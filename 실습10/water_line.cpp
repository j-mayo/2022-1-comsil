#include "ofMain.h"
#include "water_line.h"
#include "ofApp.h"

WaterLine::WaterLine(int num_of_line) {
	draw_complete = 0;
	calc_complete = 0;

	color = ofColor::dodgerBlue;

	path_idx = 0;
	path = new Dot[2 * num_of_line + 4];
}

WaterLine::~WaterLine() {
	delete(path);
}
