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
            ofDrawLine(linex1[i], liney1[i], linex2[i], liney2[i]); // draw the line
        }
        for (int i = 0; i < num_of_dot; i++) {
            if (i == curdot) ofSetColor(ofColor::red); // activated dot: red color!
            else ofSetColor(ofColor::black); // else black color
            
            ofDrawCircle(dotx1[i], doty1[i], dot_diameter); // draw the circle(dot)
        }

         // 2nd week portion.
        ofSetLineWidth(2);
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
        if (linex1) delete(linex1);
        if (liney1) delete(liney1);
        if (linex2) delete(linex2);
        if (liney2) delete(liney2);
        if (dotx1) delete(dotx1);
        if (doty1) delete(doty1);
        if (rankx1) delete(rankx1);

        cout << "Every dynamically allocated memory has been freed." << endl;

        _Exit(0);
    }
    if (key == 'd') {
        if (!load_flag) return;

        /* COMSIL1-TODO 2: This is draw control part.
        You should draw only after when the key 'd' has been pressed.
        */
        draw_flag = 1;
    }
    if (key == 's') {
        // 2nd week portion.
    }
    if (key == 'e') {
        // 2nd week portion.
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
        curdot = (curdot + 1) % num_of_dot; // use the modular (num_of_dot)

        cout << "Selcted Dot Coordinate is (" << dotx1[curdot] << ", " << doty1[curdot] << ")" << endl;
    }
    if (key == OF_KEY_LEFT) {
        curdot = (num_of_dot + curdot - 1) % num_of_dot; // uses the modular(num_of_dot)

        cout << "Selcted Dot Coordinate is (" << dotx1[curdot] << ", " << doty1[curdot] << ")" << endl;
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
                
                if (!linex1) linex1 = new int[num_of_line];
                if (!liney1) liney1 = new int[num_of_line];
                if (!linex2) linex2 = new int[num_of_line];
                if (!liney2) liney2 = new int[num_of_line]; // memory allocate for line component arrays
            }
            else { // Input for the number of dots.
                num_of_dot = atoi(words[0].c_str());
                cout << "The number of dot is: " << num_of_dot << endl;

                if (!dotx1) dotx1 = new int[num_of_dot];
                if (!doty1) doty1 = new int[num_of_dot];
                if (!rankx1) rankx1 = new int[num_of_dot]; // memory allocate for dot component arrays
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
                    linex1[linecount] = x1;
                    liney1[linecount] = y1;
                    linex2[linecount] = x2;
                    liney2[linecount++] = y2; // save the component to array
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
                    dotx1[dotcount] = x1;
                    doty1[dotcount++] = y1; // save the component to array
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
}

void ofApp::initializeWaterLines() {
    ;
}


