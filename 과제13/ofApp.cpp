/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.
	
	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

	http://www.spout.zeal.co

    =========================================================================
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    =========================================================================

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h" // Menu addon

using namespace std;

float d; // 방의 가로세로

typedef struct _line {
	int x1, x2, y1, y2;
} line
;
Node** _stack;
int* stack_x;
int* stack_y;
int stack_top;
Node** stack_log;
int stack_log_top;
line* stack_log_xy;

Node** _queue;
int* queue_x;
int* queue_y;
int queue_front;
int queue_rear;
Node** queue_log;
int queue_log_top;
line* queue_log_xy;


bool BFS(int HEIGHT, int WIDTH, Node*** maze, int* isBFS) {
	_queue = new Node * [HEIGHT * WIDTH];
	queue_x = new int[HEIGHT * WIDTH];
	queue_y = new int[HEIGHT * WIDTH];
	queue_front = -1;
	queue_rear = -1;

	queue_log = new Node * [HEIGHT * WIDTH];
	queue_log_xy = new line[HEIGHT * WIDTH];
	queue_log_top = -1;

	_queue[++queue_rear] = maze[0][0];
	maze[0][0]->if_visited = 1;
	int flag = 0;
	Node* temp;
	line linetemp;
	linetemp.x1 = 0;
	linetemp.x2 = 0;
	linetemp.y1 = 0;
	linetemp.y2 = 0;
	queue_x[queue_rear] = 0;
	queue_y[queue_rear] = 0;

	int tempx = 0;
	int tempy = 0;
	// cout << HEIGHT << " " << WIDTH << endl;
	while (queue_front != queue_rear) {
		temp = _queue[++queue_front];
		tempx = queue_x[queue_front];
		tempy = queue_y[queue_front];
		//stack_log[++stack_log_top] = temp;
		//cout << temp->x << " " << temp->y << endl;
		if (tempx == WIDTH - 1 && tempy == HEIGHT - 1) {
			flag = 1;
			break;
		}


		if (temp->left_link && !(temp->left_link->if_visited)) {
			temp->left_link->if_visited = 1;
			temp->left_link->path = temp; // 직전 경로를 저장, 이렇게 하면 끝 점에서부터 역 탐색으로 정확한 경로를 찾을 수 있다.
			//temp->left_link->beforex = temp->x; temp->left_link->beforey = temp->y; // 정답 경로 저장을 위한 이전 좌표 저장
			queue_log[++queue_log_top] = temp->left_link;
			linetemp.x1 = tempx;
			linetemp.y1 = tempy;
			linetemp.x2 = tempx - 1;
			linetemp.y2 = tempy;
			queue_log_xy[queue_log_top] = linetemp;

			_queue[++queue_rear] = temp->left_link;
			queue_x[queue_rear] = tempx - 1;
			queue_y[queue_rear] = tempy;
		}

		if (temp->right_link && !(temp->right_link->if_visited)) {
			temp->right_link->if_visited = 1;
			temp->right_link->path = temp; // 직전 경로를 저장, 이렇게 하면 끝 점에서부터 역 탐색으로 정확한 경로를 찾을 수 있다.
			//temp->left_link->beforex = temp->x; temp->left_link->beforey = temp->y;
			queue_log[++queue_log_top] = temp->left_link;
			linetemp.x1 = tempx;
			linetemp.y1 = tempy;
			linetemp.x2 = tempx + 1;
			linetemp.y2 = tempy;
			queue_log_xy[queue_log_top] = linetemp;

			_queue[++queue_rear] = temp->right_link;
			queue_x[queue_rear] = tempx + 1;
			queue_y[queue_rear] = tempy;
		}
		if (temp->up_link && !(temp->up_link->if_visited)) {
			temp->up_link->if_visited = 1;
			temp->up_link->path = temp; // 직전 경로를 저장, 이렇게 하면 끝 점에서부터 역 탐색으로 정확한 경로를 찾을 수 있다.
			//temp->left_link->beforex = temp->x; temp->left_link->beforey = temp->y;
			queue_log[++queue_log_top] = temp->up_link;
			linetemp.x1 = tempx;
			linetemp.y1 = tempy;
			linetemp.x2 = tempx;
			linetemp.y2 = tempy - 1;
			queue_log_xy[queue_log_top] = linetemp;

			_queue[++queue_rear] = temp->up_link;
			queue_x[queue_rear] = tempx;
			queue_y[queue_rear] = tempy - 1;
		}
		if (temp->down_link && !(temp->down_link->if_visited)) {
			temp->down_link->if_visited = 1;
			temp->down_link->path = temp; // 직전 경로를 저장, 이렇게 하면 끝 점에서부터 역 탐색으로 정확한 경로를 찾을 수 있다.
			//temp->left_link->beforex = temp->x; temp->left_link->beforey = temp->y;
			queue_log[++queue_log_top] = temp->down_link;
			linetemp.x1 = tempx;
			linetemp.y1 = tempy;
			linetemp.x2 = tempx;
			linetemp.y2 = tempy + 1;
			queue_log_xy[queue_log_top] = linetemp;

			_queue[++queue_rear] = temp->down_link;
			queue_x[queue_rear] = tempx;
			queue_y[queue_rear] = tempy + 1;
		}
	}

	if (flag) (*isBFS) = 1;

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			maze[i][j]->if_visited = 0;
		}
	}
	delete[] _queue;
	delete[] queue_x;
	delete[] queue_y;
	return flag ? true : false;
}

void bfsdraw(int HEIGHT, int WIDTH, Node*** maze) {
	ofColor correct = ofColor::lightGreen;
	ofColor incorrect = ofColor::magenta;

	ofSetColor(incorrect);
	float half_d = d / 2;
	Node* temp;

	for (int i = 0; i < queue_log_top; i++) {

		//cout << stack_log_xy[i].x1 << " / " << stack_log_xy[i].y1 << " / " << stack_log_xy[i].x2 << " / " << stack_log_xy[i].y2 << endl;
		ofDrawLine(d * (queue_log_xy[i].x1 + 1) + half_d, d * (queue_log_xy[i].y1 + 1) + half_d, d * (queue_log_xy[i].x2 + 1) + half_d, d * (queue_log_xy[i].y2 + 1) + half_d);
	}
	//printf("정답 전\n");

	ofSetColor(correct);
	int curx = HEIGHT - 1;
	int cury = WIDTH - 1;
	int tempx, tempy;
	temp = maze[curx][cury];
	while (!(curx == 0 && cury == 0)) {
		if (temp->path == temp->left_link) {
			tempx = curx;
			tempy = cury - 1;
		}
		else if (temp->path == temp->right_link) {
			tempx = curx;
			tempy = cury + 1;
		}
		else if (temp->path == temp->up_link) {
			tempx = curx - 1;
			tempy = cury;
		}
		else {
			tempx = curx + 1;
			tempy = cury;
		}
		temp = temp->path;

		ofDrawLine(d * (tempy + 1) + half_d, d * (tempx + 1) + half_d, d * (cury + 1) + half_d, d * (curx + 1) + half_d);
		// cout << tempx << " " << tempy << "/" << curx << " " << cury << endl;
		curx = tempx;
		cury = tempy;
	}

	return;
}

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isDFS = 0;
	isBFS = 0;
	isOpen = 0;
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

	// Load a font rather than the default
	myFont.loadFont("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	//menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	menu->AddPopupItem(hPopup, "Show DFS");
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS"); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check

	// Set the menu to the window
	menu->SetWindowMenu();

} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		//if (isOpen) freeMemory();
		readFile();
	}
	if(title == "Exit") {
		freeMemory();
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		if (isOpen)
		{
			if (!isDFS) DFS();
			isBFS = 0;
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		//doTopmost(bChecked); // Use the checked value directly
		if (isOpen)
		{
			//if (!isBFS) BFS();
			isDFS = 0;

			if (!isBFS) BFS(HEIGHT,WIDTH, maze, &isBFS);
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;

	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(100);
	ofSetLineWidth(5);
	int i, j;
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here
	if (isOpen) {
		float length = min(ofGetHeight()-20, ofGetWidth());
		d = length / (float)max(HEIGHT + 2, WIDTH + 2);
		/*
		float dy = length / (float)(HEIGHT + 2);
		float dx = length / (float)(WIDTH + 2);
		*/
		/*
		float dy = d * (HEIGHT + 2);
		float dx = d * (WIDTH + 2);
		*/
		/*
		ofDrawLine(dx, dy, dx * (WIDTH + 1), dy); // 첫째 가로줄
		ofDrawLine(dx * (WIDTH + 1), dy, dx * (WIDTH + 1), dy * (HEIGHT + 1)); //가장 오른쪽 세로줄
		for (i = 0; i < HEIGHT; i++) {
			for (j = 0; j < WIDTH; j++) {
				if (!(maze[i][j]->left_link)) ofDrawLine(dx * (j + 1), dy * (i + 1), dx * (j + 1), dy * (i + 2)); // 벽이 있다면 그려준다.
				if (!(maze[i][j]->down_link)) ofDrawLine(dx * (j + 1), dy * (i + 2), dx * (j + 2), dy * (i + 2));
			}
		}
		*/
		ofDrawLine(d, d, d * (WIDTH + 1), d); // 첫째 가로줄
		ofDrawLine(d * (WIDTH + 1), d, d * (WIDTH + 1), d * (HEIGHT + 1)); //가장 오른쪽 세로줄
		for (i = 0; i < HEIGHT; i++) {
			for (j = 0; j < WIDTH; j++) {
				if (!(maze[i][j]->left_link)) ofDrawLine(d * (j + 1), d * (i + 1), d * (j + 1), d * (i + 2)); // 벽이 있다면 그려준다.
				if (!(maze[i][j]->down_link)) ofDrawLine(d * (j + 1), d * (i + 2), d * (j + 2), d * (i + 2));
			}
		}
	}


	if (isDFS)
	{
		ofSetColor(200);
		ofSetLineWidth(5);
		if (isOpen) {
			isBFS = 0;
			dfsdraw();
		}
		else
			cout << "You must open file first" << endl;
	}

	if (isBFS)
	{
		if (isOpen) {
			isDFS = 0;
			//bfsdraw();
			bfsdraw(HEIGHT, WIDTH, maze);
		}
		else
			cout << "You must open file first" << endl;
	}
	if(1) {
		// Show keyboard duplicates of menu functions
		//myFont.loadFont("verdana.ttf", 12, true, true);
		ofSetColor(ofColor::darkGray);
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else { 
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU)); 
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");
	string filePath;
	size_t pos;
	if (input) freeMemory();
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		cout << "file name is " << fileName << endl;
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
				isDFS = 0;
				isBFS = 0;
			}

			ofBuffer buffer(file);

			// Input_flag is a variable for indication the type of input.
			// If input_flag is zero, then work of line input is progress.
			// If input_flag is one, then work of dot input is progress.
			int input_flag = 0;

			// Idx is a variable for index of array.
			int idx = 0;

			// Read file line by line
			int cnt = 0; 
			
			int left_flag = 0;

			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다

			
			ofBuffer::Line it = buffer.getLines().begin();
			string line = *it;
			vector<string> words = ofSplitString(line, "+"); // +를 기준으로 split
			WIDTH = words.size() -2 ; // 미로 폭
			/*
			cout << words[0] << words[1] << words[10] << words[11] << endl;
			cout << WIDTH << endl;
			*/
			
			HEIGHT = 0;
			
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
				HEIGHT++;
			}
			HEIGHT /= 2;
			// HEIGHT++; // 미로의 세로 길이를 구해야 한다.

			// 미로 정보를 저장하는 문자열 배열
			input = new char* [2*HEIGHT+1];
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
				input[cnt] = new char[2 * WIDTH + 1];
				line = *it;
				vector<char> temp(line.begin(), line.end());
				std::copy(temp.begin(), temp.end(), input[cnt++]); // 문자열을 input 배열로 복사해줌
			}


			// 2차원 node 배열 선언하기
			// Node pointer가 원소인 2차원 배열이다.
			maze = new Node **[HEIGHT];
			for (int i = 0; i < HEIGHT; i++) {
				maze[i] = new Node *[WIDTH];
				for (int j = 0; j < WIDTH; j++) {
					maze[i][j] = new Node;
					//maze[i][j]->x = i; maze[i][j]->y = j;
					maze[i][j]->if_visited = 0;
					//maze[i][j]->beforex = -1; maze[i][j]->beforey = -1;
				}
			} // 미로 생성을 위한 자료구조 생성

			int curline = 0; // 현재까지 읽은 줄 수를 기록하는 데 사용하기로 함
			for (int i = 0; i < HEIGHT; i++) { // 자료구조에 미로 저장

				for (int j = 0; j < WIDTH; j++) {
					// 상하 벽
					if (!(input[2 * i][2 * j + 1] == '-')) maze[i][j]->up_link = maze[i - 1][j];
					else maze[i][j]->up_link = nullptr;

					if (!(input[2 * i + 2][2 * j + 1] == '-')) maze[i][j]->down_link = maze[i + 1][j];
					else maze[i][j]->down_link = nullptr;
					// 좌우 벽
					if (!(input[2*i+1][j * 2] == '|')) maze[i][j]->left_link = maze[i][j - 1]; // 왼쪽 벽 정보 기록
					else maze[i][j]->left_link = nullptr;

					if (!(input[2*i+1][j * 2 + 2] == '|')) maze[i][j]->right_link = maze[i][j + 1]; // 오른쪽 벽 정보 기록
					else maze[i][j]->right_link = nullptr;
				}

			}

			/*
			for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
				string line = *it;
				// ofBuffer로부터 한 line씩 파일을 읽자

				
				if (horizon_flag == 0) { // 각 방의 위 아래 벽 확인 단계
					vector<string> words = ofSplitString(line, "+"); // +를 기준으로 split
					float length = min(ofGetHeight(), ofGetWidth()); // 화면 높이와 폭 중 작은 쪽을 기준으로
					float interval = length / (float)(words.size() + 2); // 미로 각 방의 간격을 설정한다
					// 미로 시작 예정 위치: (interval, interval)
					for (int i = 0; i = words.size(); i++) {

					}
				}
			}
			*/
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}
void ofApp::freeMemory() {
	// 할당한 메모리는 input, maze에 있다.
	//cout << "freememory!\n";
	for (int i = 0; i < 2*HEIGHT+1; i++) {
		delete[] input[i];
	}
	delete[] input;

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			delete maze[i][j];
		}
		delete[] maze[i];
	}
	delete[] maze;

	if (stack_log) delete[] stack_log;
	if (queue_log) delete[] queue_log;
	if (stack_log_xy) delete[] stack_log_xy;
	//delete[] stack_log_y;
	if (queue_log_xy) delete[] queue_log_xy;
	//delete[] queue_log_y;

	//TO DO
	// malloc한 memory를 free해주는 함수
}

bool ofApp::DFS()//DFS탐색을 하는 함수
{
	_stack = new Node * [4 * HEIGHT * WIDTH];
	stack_x = new int[4 * HEIGHT * WIDTH];
	stack_y = new int[4 * HEIGHT * WIDTH];
	stack_top = -1;

	stack_log = new Node * [4 * HEIGHT * WIDTH];
	stack_log_xy = new line[4 * HEIGHT * WIDTH];
	stack_log_top = -1;


	_stack[++stack_top] = maze[0][0];
	line linetemp;
	linetemp.x1 = 0;
	linetemp.x2 = 0;
	linetemp.y1 = 0;
	linetemp.y2 = 0;
	stack_x[stack_top] = 0;
	stack_y[stack_top] = 0;
	maze[0][0]->if_visited = 1;
	int flag = 0;
	Node* temp;
	int testcnt = 0;
	int tempx = 0;
	int tempy = 0;
	// cout << HEIGHT << " " << WIDTH << endl;
	while (stack_top >= 0) {
		temp = _stack[stack_top];
		tempx = stack_x[stack_top];
		tempy = stack_y[stack_top--];
		//stack_log[++stack_log_top] = temp;
		//cout << tempx << " / " << tempy << endl;
		if (tempy == HEIGHT - 1 && tempx == WIDTH - 1) {
			flag = 1;
			break;
		}

		if (temp->left_link && !(temp->left_link->if_visited)) {
			temp->left_link->if_visited = 1;
			temp->left_link->path = temp; // 직전 경로를 저장, 이렇게 하면 끝 점에서부터 역 탐색으로 정확한 경로를 찾을 수 있다.
			//temp->left_link->beforex = temp->x; temp->left_link->beforey = temp->y; // 정답 경로 저장을 위한 이전 좌표 저장

			// 아직 나머지 3개를 탐색하지 못했으니 다시 넣어준다.
			stack_log[++stack_log_top] = temp;
			linetemp.x1 = tempx;
			linetemp.y1 = tempy;
			linetemp.x2 = tempx;
			linetemp.y2 = tempy;
			stack_log_xy[stack_log_top] = linetemp;

			stack_log[++stack_log_top] = temp->left_link;
			linetemp.x1 = tempx;
			linetemp.y1 = tempy;
			linetemp.x2 = tempx - 1;
			linetemp.y2 = tempy;
			stack_log_xy[stack_log_top] = linetemp;


			_stack[++stack_top] = temp; // 아직 나머지 3방향을 탐색하지 않았으므로 temp를 다시 스택에 넣는다
			stack_x[stack_top] = tempx; // x, y좌표도 함께 넣어준다
			stack_y[stack_top] = tempy;

			_stack[++stack_top] = temp->left_link;
			stack_x[stack_top] = tempx - 1;
			stack_y[stack_top] = tempy;
			testcnt++;
			continue;
		}

		if (temp->right_link && !(temp->right_link->if_visited)) {
			temp->right_link->if_visited = 1;
			temp->right_link->path = temp; // 직전 경로를 저장, 이렇게 하면 끝 점에서부터 역 탐색으로 정확한 경로를 찾을 수 있다.
			//temp->left_link->beforex = temp->x; temp->left_link->beforey = temp->y;

			// 아직 나머지 2개를 탐색하지 못했으니 다시 넣어준다.
			stack_log[++stack_log_top] = temp;
			linetemp.x1 = tempx;
			linetemp.y1 = tempy;
			linetemp.x2 = tempx;
			linetemp.y2 = tempy;
			stack_log_xy[stack_log_top] = linetemp;

			stack_log[++stack_log_top] = temp->left_link;
			linetemp.x1 = tempx;
			linetemp.y1 = tempy;
			linetemp.x2 = tempx + 1;
			linetemp.y2 = tempy;
			stack_log_xy[stack_log_top] = linetemp;

			_stack[++stack_top] = temp; // 아직 나머지 2방향을 탐색하지 않았으므로 temp를 다시 스택에 넣는다
			stack_x[stack_top] = tempx; // x, y좌표도 함께 넣어준다
			stack_y[stack_top] = tempy;

			_stack[++stack_top] = temp->right_link;
			stack_x[stack_top] = tempx + 1;
			stack_y[stack_top] = tempy;
			testcnt++;
			continue;
		}
		if (temp->up_link && !(temp->up_link->if_visited)) {
			temp->up_link->if_visited = 1;
			temp->up_link->path = temp; // 직전 경로를 저장, 이렇게 하면 끝 점에서부터 역 탐색으로 정확한 경로를 찾을 수 있다.
			//temp->left_link->beforex = temp->x; temp->left_link->beforey = temp->y;

			// 아직 아랫 방향이 남았으니 다시 넣어준다.
			stack_log[++stack_log_top] = temp;
			linetemp.x1 = tempx;
			linetemp.y1 = tempy;
			linetemp.x2 = tempx;
			linetemp.y2 = tempy;
			stack_log_xy[stack_log_top] = linetemp;

			stack_log[++stack_log_top] = temp->up_link;
			linetemp.x1 = tempx;
			linetemp.y1 = tempy;
			linetemp.x2 = tempx;
			linetemp.y2 = tempy - 1;
			stack_log_xy[stack_log_top] = linetemp;


			_stack[++stack_top] = temp; // 아직 아래의 방향을 탐색하지 않았으므로 temp를 다시 스택에 넣는다
			stack_x[stack_top] = tempx; // x, y좌표도 함께 넣어준다
			stack_y[stack_top] = tempy;

			_stack[++stack_top] = temp->up_link;
			stack_x[stack_top] = tempx;
			stack_y[stack_top] = tempy - 1;
			testcnt++;
			continue;
		}
		if (temp->down_link && !(temp->down_link->if_visited)) {
			temp->down_link->if_visited = 1;
			temp->down_link->path = temp; // 직전 경로를 저장, 이렇게 하면 끝 점에서부터 역 탐색으로 정확한 경로를 찾을 수 있다.
			//temp->left_link->beforex = temp->x; temp->left_link->beforey = temp->y;
			stack_log[++stack_log_top] = temp->down_link;
			linetemp.x1 = tempx;
			linetemp.y1 = tempy;
			linetemp.x2 = tempx;
			linetemp.y2 = tempy + 1;
			stack_log_xy[stack_log_top] = linetemp;

			_stack[++stack_top] = temp->down_link;
			stack_x[stack_top] = tempx;
			stack_y[stack_top] = tempy + 1;
			testcnt++;
		}

	}
	if (flag) isDFS = 1;
	//cout << isDFS << endl;
	//cout << "stack_top  " << stack_top << endl;
	//cout << "stack_log_top " << stack_log_top << endl;
	//cout << testcnt << endl;
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			maze[i][j]->if_visited = 0;
		}
	}
	delete[] _stack;
	delete[] stack_x;
	delete[] stack_y;
	return flag ? true : false;
}
void ofApp::dfsdraw()
{
	//TO DO 
	//DFS를 수행한 결과를 그린다. (3주차 내용)
	
	ofColor correct = ofColor::aquamarine;
	ofColor incorrect = ofColor::red;

	ofSetColor(incorrect);
	float half_d = d / 2;
	Node* temp;
	/*
	stack = new Node * [HEIGHT * WIDTH];
	stack_top = -1;
	
	stack[++stack_top] = maze[0][0];
	maze[0][0]->if_visited = 1;
	Node* temp;

	while (stack_top >= 0) {
		temp = stack[stack_top--];
		//cout << temp->x << " " << temp->y << endl;
		if (temp->x == HEIGHT - 1 && temp->y == WIDTH - 1) break;

		if (temp->left_link && !(temp->left_link->if_visited)) {
			temp->left_link->if_visited = 1;
			temp->left_link->path = temp; // 직전 경로를 저장, 이렇게 하면 끝 점에서부터 역 탐색으로 정확한 경로를 찾을 수 있다.
			//temp->left_link->beforex = temp->x; temp->left_link->beforey = temp->y; // 정답 경로 저장을 위한 이전 좌표 저장
			ofDrawLine(d * (temp->y + 1) + half_d, d * (temp->x + 1) + half_d, d * (temp->left_link->y + 1) + half_d, d * (temp->left_link->x + 1) + half_d);
			stack[++stack_top] = temp->left_link;
		}

		if (temp->right_link && !(temp->right_link->if_visited)) {
			temp->right_link->if_visited = 1;
			temp->right_link->path = temp; // 직전 경로를 저장, 이렇게 하면 끝 점에서부터 역 탐색으로 정확한 경로를 찾을 수 있다.
			//temp->left_link->beforex = temp->x; temp->left_link->beforey = temp->y;
			ofDrawLine(d * (temp->y + 1) + half_d, d * (temp->x + 1) + half_d, d * (temp->right_link->y + 1) + half_d, d * (temp->right_link->x + 1) + half_d);
			stack[++stack_top] = temp->right_link;
		}

		if (temp->up_link && !(temp->up_link->if_visited)) {
			temp->up_link->if_visited = 1;
			temp->up_link->path = temp; // 직전 경로를 저장, 이렇게 하면 끝 점에서부터 역 탐색으로 정확한 경로를 찾을 수 있다.
			//temp->left_link->beforex = temp->x; temp->left_link->beforey = temp->y;
			ofDrawLine(d * (temp->y + 1) + half_d, d * (temp->x + 1) + half_d, d * (temp->up_link->y + 1) + half_d, d * (temp->up_link->x + 1) + half_d);
			stack[++stack_top] = temp->up_link;
		}

		if (temp->down_link && !(temp->down_link->if_visited)) {
			temp->down_link->if_visited = 1;
			temp->down_link->path = temp; // 직전 경로를 저장, 이렇게 하면 끝 점에서부터 역 탐색으로 정확한 경로를 찾을 수 있다.
			//temp->left_link->beforex = temp->x; temp->left_link->beforey = temp->y;
			ofDrawLine(d * (temp->y + 1) + half_d, d * (temp->x + 1) + half_d, d * (temp->down_link->y + 1) + half_d, d * (temp->down_link->x + 1) + half_d);
			stack[++stack_top] = temp->down_link;
		}
	}


	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			maze[i][j]->if_visited = 0;
		}
	}
	delete[] stack;
	*/
	//printf("일반 전\n");
	//cout << "---------------------" << endl;
	for (int i = 0; i < stack_log_top; i++) {
		
		//cout << stack_log_xy[i].x1 << " / " << stack_log_xy[i].y1 << " / " << stack_log_xy[i].x2 << " / " << stack_log_xy[i].y2 << endl;
		ofDrawLine(d * (stack_log_xy[i].x1 + 1) + half_d, d * (stack_log_xy[i].y1 + 1) + half_d, d * (stack_log_xy[i].x2 + 1) + half_d, d * (stack_log_xy[i].y2 + 1) + half_d);
	}
	//printf("정답 전\n");

	ofSetColor(correct);
	int curx = HEIGHT - 1;
	int cury = WIDTH - 1;
	int tempx, tempy;
	temp = maze[curx][cury];
	while (!(curx == 0 && cury == 0)) {
		if (temp->path == temp->left_link) {
			tempx = curx;
			tempy = cury - 1;
		}
		else if (temp->path == temp->right_link) {
			tempx = curx;
			tempy = cury + 1;
		}
		else if (temp->path == temp->up_link) {
			tempx = curx - 1 ;
			tempy = cury;
		}
		else {
			tempx = curx + 1;
			tempy = cury;
		}
		temp = temp->path;
		
		ofDrawLine(d * (tempy + 1) + half_d, d * (tempx + 1) + half_d, d * (cury + 1) + half_d, d * (curx + 1) + half_d);
		// cout << tempx << " " << tempy << "/" << curx << " " << cury << endl;
		curx = tempx;
		cury = tempy;
	}
	
	return;
}
/*
bool ofApp::BFS() {
	queue = new Node * [HEIGHT * WIDTH];
	queue_front = -1;
	queue_rear = -1;

	queue_log = new Node * [HEIGHT * WIDTH];
	queue_log_top = -1;

	queue[++queue_rear] = maze[0][0];
	maze[0][0]->if_visited = 1;
	int flag = 0;
	Node* temp;

	while (queue_front != queue_rear) {
		temp = queue[++queue_front];
		//stack_log[++stack_log_top] = temp;
		//cout << temp->x << " " << temp->y << endl;
		if (temp->x == HEIGHT - 1 && temp->y == WIDTH - 1){
			flag = 1;
			break;
		}


		if (temp->left_link && !(temp->left_link->if_visited)) {
			temp->left_link->if_visited = 1;
			temp->left_link->path = temp; // 직전 경로를 저장, 이렇게 하면 끝 점에서부터 역 탐색으로 정확한 경로를 찾을 수 있다.
			//temp->left_link->beforex = temp->x; temp->left_link->beforey = temp->y; // 정답 경로 저장을 위한 이전 좌표 저장
			queue_log[++queue_log_top] = temp->left_link;
			queue[++queue_rear] = temp->left_link;
		}

		if (temp->right_link && !(temp->right_link->if_visited)) {
			temp->right_link->if_visited = 1;
			temp->right_link->path = temp; // 직전 경로를 저장, 이렇게 하면 끝 점에서부터 역 탐색으로 정확한 경로를 찾을 수 있다.
			//temp->left_link->beforex = temp->x; temp->left_link->beforey = temp->y;
			queue_log[++queue_log_top] = temp->right_link;
			queue[++queue_rear] = temp->right_link;
		}
		if (temp->up_link && !(temp->up_link->if_visited)) {
			temp->up_link->if_visited = 1;
			temp->up_link->path = temp; // 직전 경로를 저장, 이렇게 하면 끝 점에서부터 역 탐색으로 정확한 경로를 찾을 수 있다.
			//temp->left_link->beforex = temp->x; temp->left_link->beforey = temp->y;
			queue_log[++queue_log_top] = temp->up_link;
			queue[++queue_rear] = temp->up_link;
		}
		if (temp->down_link && !(temp->down_link->if_visited)) {
			temp->down_link->if_visited = 1;
			temp->down_link->path = temp; // 직전 경로를 저장, 이렇게 하면 끝 점에서부터 역 탐색으로 정확한 경로를 찾을 수 있다.
			//temp->left_link->beforex = temp->x; temp->left_link->beforey = temp->y;
			queue_log[++queue_log_top] = temp->down_link;
			queue[++queue_rear] = temp->down_link;
		}

	}
	if (flag) isBFS = 1;

	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			maze[i][j]->if_visited = 0;
		}
	}
	delete[] queue;
	return flag ? true : false;
}

void ofApp::bfsdraw() {
	ofColor correct = ofColor::lightGreen;
	ofColor incorrect = ofColor::magenta;

	ofSetColor(incorrect);
	float half_d = d / 2;
	Node* temp;

	for (int i = 1; i <= queue_log_top; i++) {
		ofDrawLine(d * (queue_log[i]->y + 1) + half_d, d * (queue_log[i]->x + 1) + half_d, d * (queue_log[i]->path->y + 1) + half_d, d * (queue_log[i]->path->x + 1) + half_d);
	}


	ofSetColor(correct);
	int curx = HEIGHT - 1;
	int cury = WIDTH - 1;
	temp = maze[curx][cury];
	while (!(curx == 0 && cury == 0)) {
		temp = temp->path;
		ofDrawLine(d * (temp->y + 1) + half_d, d * (temp->x + 1) + half_d, d * (cury + 1) + half_d, d * (curx + 1) + half_d);
		curx = temp->x;
		cury = temp->y;
	}

	return;
}
*/



