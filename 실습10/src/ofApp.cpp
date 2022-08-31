
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(15); // Limit the speed of our program to 15 frames per second

    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush
    ofBackground(255, 255, 255);
    ofSetLineWidth(4);

    draw_flag = 0;
    load_flag = 0;
    water_flag = 0;
    dot_diameter = 10.0f;
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(127, 23, 31);  // Set the drawing color to brown

    // Draw shapes for ceiling and floor
    ofDrawRectangle(0, 0, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofDrawRectangle(0, 728, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofSetLineWidth(5);


    ofSetLineWidth(5);
    if (draw_flag) {


        /* COMSIL1-TODO 3 : Draw the line segment and dot in which water starts to flow in the screen.
         Note that after drawing line segment and dot, you have to make selected water start dot colored in red.
         */

        for (int i = 0; i < num_of_line; i++) {
            ofDrawLine(Lines[i].x1, Lines[i].y1, Lines[i].x2, Lines[i].y2); // draw the line
        }
        for (int i = 0; i < num_of_dot; i++) {
            if (i == curdot) ofSetColor(ofColor::red); // activated dot: red color!
            else ofSetColor(ofColor::black); // else black color
            
            ofDrawCircle(Dots[i].x1, Dots[i].y1, dot_diameter); // draw the circle(dot)
        }

         // 2nd week portion.
        ofSetLineWidth(2);
    }
    if (water_flag) {
        waterline->draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 'v') {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_" + ofGetTimestampString() + ".png");
    }
    if (key == 'q') {
        // Reset flags
        draw_flag = 0;

        // Free the dynamically allocated memory exits.
        if (Lines) delete(Lines);
        if (Dots) delete(Dots);

        if (waterline) delete waterline;
        
        //if (linex1) delete(linex1);
        //if (liney1) delete(liney1);
        //if (linex2) delete(linex2);
        //if (liney2) delete(liney2);
        //if (dotx1) delete(dotx1);
        //if (doty1) delete(doty1);
        //if (rankx1) delete(rankx1);

        cout << "Every dynamically allocated memory has been freed." << endl;

        _Exit(0);
    }
    if (key == 'd') {
        if (!load_flag) return;
        draw_flag = 1;
    }
    if (key == 's') {
        if (load_flag) {
            resetWater();
            if (waterline->calc_complete == 0) waterline->calculate_path(Lines, num_of_line);
        }
        if (draw_flag) {
            ofEnableSmoothing();
            water_flag = 1;
        }
    }
    if (key == 'e') {
        if (draw_flag)
            ofDisableSmoothing();
            water_flag = 0;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    if (key == 'l') {
        // Open the Open File Dialog
        ofFileDialogResult openFileResult = ofSystemLoadDialog("Select a only txt for Waterfall");

        // Check whether the user opened a file
        if (openFileResult.bSuccess) {
            ofLogVerbose("User selected a file");

            // We have a file, so let's check it and process it
            processOpenFileSelection(openFileResult);
            load_flag = 1;
        }
    }

    /* COMSIL1-TODO 4: This is selection dot control part.
     You can select dot in which water starts to flow by left, right direction key (<- , ->).
     */

    if (key == OF_KEY_RIGHT) {
        if (water_flag) {
            cout << "The water is flowing! " << endl;
            return;
        }
        water_flag = 0;
        curdot = (curdot + 1) % num_of_dot; // use the modular (num_of_dot)

        cout << "Selcted Dot Coordinate is (" << Dots[curdot].x1 << ", " << Dots[curdot].y1 << ")" << endl;
        resetWater();
    }
    if (key == OF_KEY_LEFT) {
        if (water_flag) {
            cout << "The water is flowing! " << endl;
            return;
        }
        water_flag = 0;
        curdot = (num_of_dot + curdot - 1) % num_of_dot; // uses the modular(num_of_dot)

        cout << "Selcted Dot Coordinate is (" << Dots[curdot].x1 << ", " << Dots[curdot].y1 << ")" << endl;
        resetWater();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) {
    //Path to the comma delimited file
    //string fileName = "input.txt";

    string fileName = openFileResult.getName();
    ofFile file(fileName);

    if (!file.exists()) cout << "Target file does not exists." << endl;
    else cout << "We found the target file." << endl;

    ofBuffer buffer(file);

    /* This variable is for indicating which type of input is being received.
     IF input_type == 0, then work of getting line input is in progress.
     IF input_type == 1, then work of getting dot input is in progress.
     */
    int input_type = 0;


    /* COMSIL1-TODO 1 : Below code is for getting the number of line and dot, getting coordinates.
     You must maintain those information. But, currently below code is not complete.
     Also, note that all of coordinate should not be out of screen size.
     However, all of coordinate do not always turn out to be the case.
     So, You have to develop some error handling code that can detect whether coordinate is out of screen size.
    */

    int linecount = 0;
    int dotcount = 0;
    int currank;
    int temp;
    // Read file line by line
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;

        // Split line into strings
        vector<string> words = ofSplitString(line, " ");

        if (words.size() == 1) {
            if (input_type == 0) { // Input for the number of lines.
                
                num_of_line = atoi(words[0].c_str());
                cout << "The number of line is: " << num_of_line << endl;
                
                if (!Lines) Lines = new Line[num_of_line];

                //if (!linex1) linex1 = new int[num_of_line];
                //if (!liney1) liney1 = new int[num_of_line];
                //if (!linex2) linex2 = new int[num_of_line];
                //if (!liney2) liney2 = new int[num_of_line]; // memory allocate for line component arrays
            }
            else { // Input for the number of dots.
                num_of_dot = atoi(words[0].c_str());
                cout << "The number of dot is: " << num_of_dot << endl;

                if (!Dots) Dots = new Dot[num_of_dot];

                //if (!dotx1) dotx1 = new int[num_of_dot];
                //if (!doty1) doty1 = new int[num_of_dot];
                //if (!rankx1) rankx1 = new int[num_of_dot]; // memory allocate for dot component arrays
            }
        }
        else if (words.size() >= 2) {
            int x1, y1, x2, y2;
            if (input_type == 0) { // Input for actual information of lines
                x1 = atoi(words[0].c_str());
                y1 = atoi(words[1].c_str());
                x2 = atoi(words[2].c_str());
                y2 = atoi(words[3].c_str());
                if (x1 < 0 || x1 >= ofGetWidth() || x2 < 0 || x2 >= ofGetWidth() || y1 >= ofGetHeight() || y2 >= ofGetHeight()) {
                    cout << "The coordinate of line is out of range!" << endl;
                    return;
                }
                else {
                    Lines[linecount].x1 = x1;
                    Lines[linecount].y1 = y1;
                    Lines[linecount].x2 = x2;
                    Lines[linecount].y2 = y2; // save the component to array, 2nd week: re-implemented
                    if (x2 != x1) Lines[linecount++].gradient = (double)(y2 - y1) / (x2 - x1); // 2nd week addition!
                    else Lines[linecount++].gradient = INFINITE;
                    if (linecount == num_of_line) input_type++; // if we save num_of_line lines, then now we have to save the component of dots.
                }
            }
            else { // Input for actual information of dots.
                x1 = atoi(words[0].c_str());
                y1 = atoi(words[1].c_str());
                if (x1 < 0 || x1 >= ofGetWidth() || y1 >= ofGetHeight()) {
                    cout << "The coordinate of dot is out of range!" << endl;
                    return;
                }
                else {
                    Dots[dotcount].x1 = x1;
                    Dots[dotcount++].y1 = y1; // save the component to array
                }
            }
        } // End of else if.
    } // End of for-loop (Read file line by line).
    input_type--;
    curdot = 0;
    /*
    // dotx1 이 작은 순위의 idx를 rankx1에 저장
    for (int i = 0; i < num_of_dot; i++) {
        temp = dotx1[i];
        currank = 0;
        for (int j = 0; j < num_of_dot; j++) {
            if (j == i) continue;
            if (dotx1[i] > dotx1[j]) currank++;
        }
        while (rankx1[currank]) currank++; // 동점 대비
        rankx1[currank] = i;
    }
    */

    //initializeWaterLines();

    waterline = new WaterLine(num_of_line, Dots[curdot].x1, Dots[curdot].y1);
    // 초기화
    resetWater();
}

void ofApp::resetWater() {
    waterline->start_x = Dots[curdot].x1;
    waterline->start_y = Dots[curdot].y1;
    waterline->reset();
}

void ofApp::initializeWaterLines() {
    ;
}

WaterLine::WaterLine(int num_of_line, int start_x, int start_y) {
    //draw_complete = 0;
    calc_complete = 0;

    color = ofColor::dodgerBlue;

    start_x = start_x;
    start_y = start_y;
    path_idx = 0;
    path = new Dot[2 * num_of_line + 2];
    radius = 5;
    draw_number = 1;
}

WaterLine::~WaterLine() {
    delete[] path;
}

void WaterLine::draw() {
    if (calc_complete) {
        //ofSetLineWidth(5);
        int count = 0;
        double leng;
        double dx, dy;
        double rx, ry;
        ofSetColor(color);
        for (int i = 0; i < path_idx - 1; i++) {
            //ofDrawLine(path[i].x1 - 0.5, path[i].y1 - 0.5, path[i + 1].x1 + 0.5, path[i + 1].y1 + 0.5);
            leng = (double)sqrt(((path[i].x1 - path[i + 1].x1) * (path[i].x1 - path[i + 1].x1) + (path[i].y1 - path[i + 1].y1) * (path[i].y1 - path[i + 1].y1)));
            dx = (double)4*(path[i + 1].x1 - path[i].x1) / (leng);
            dy = (double)4*(path[i + 1].y1 - path[i].y1) / (leng);
            rx = path[i].x1;
            ry = path[i].y1;
            //cout << leng << "//" << dx << "//" << dy << "//" << rx << "//" << ry << endl;
            while (ry < path[i + 1].y1) {
                ofSetColor(color);
                ofDrawCircle((float)rx, (float)ry, radius);
                rx += dx;
                ry += dy;
                //cout << ry << "///" << path[i + 1].y1 << endl;
                count++;
                if (count >= draw_number) {
                    draw_number++;
                    return;
                }
                
            }
        }
        /*
        for (int i = start_y; i < ofGetHeight() - 40; start_y++) {
            if (start_y > path[curpath + 1].y1) curpath++; // 현재 보고있는 선분의 끝 점보다 y좌표 커지면 다음 선분으로 가기
            tempx;
        }
        */
    }
    //draw_complete = 1;
}

void WaterLine::reset() {
    // 배수구를 바꿀 시 리셋할 필요가 있음
    calc_complete = 0;
    //draw_complete = 0;
    path_idx = 0;
    draw_number = 1;
}

void WaterLine::calculate_path(Line* lines, int num_of_line) {
    path[path_idx].x1 = start_x;
    path[path_idx++].y1 = start_y;
    int height = ofGetHeight();
    double gradient;
    while (start_y < height - 40) {
        for (int i = 0; i < num_of_line; i++) {
            if (lines[i].x1 < lines[i].x2 && (start_x < lines[i].x1 || (start_x > lines[i].x2))) continue;
            else if (lines[i].x2 <= lines[i].x1 && (start_x < lines[i].x2 || (start_x > lines[i].x1))) continue;
            // x coordinate
            if (lines[i].y1 <= start_y && lines[i].y2 <= start_y) continue;

            if (start_x != lines[i].x1) {
                gradient = (double)(start_y - lines[i].y1) / (start_x - lines[i].x1);
            }
            else
            {
                gradient = INFINITE;
            }
            // this is the gradient of (start_x, start_y), (x1, y1 in current line)

            if (abs(gradient - lines[i].gradient) < EPSILON) {
                // if gradients are same (it's like eps-delta)
                path[path_idx].x1 = start_x;
                path[path_idx++].y1 = start_y; // path에 좌표 기록
                if (lines[i].gradient >= 0) {
                    path[path_idx - 1].x1--;
                    start_x = lines[i].x2;
                    start_y = lines[i].y2 - 1; // 고의적으로 조금 높이 설정, 물이 선반 위에 있음을 나타내기 위함
                }
                else {
                    path[path_idx - 1].x1++;
                    start_x = lines[i].x1;
                    start_y = lines[i].y1 - 1;
                }
                // gradient에 따라 좌표 처리가 달라짐, 양수: 시작점을 살짝 왼쪽으로 해서 선반을 완전히 덮도록
                // 음수: 오른쪼긍로 해서 완전히 덮도록
                // 다음 시작점의 경우 현재 line의 밑의 점으로 하되 y좌표는 1 낮춰서 선반을 완전히 덮도록 한다
                path[path_idx].x1 = start_x;
                path[path_idx++].y1 = start_y;

            }
        }
        start_y++;
    }
    path[path_idx].x1 = start_x;
    path[path_idx++].y1 = start_y; // 밑에서 5픽셀 위

    calc_complete = 1; // 계산 끝
}
