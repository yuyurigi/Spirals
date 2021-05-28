#pragma once

#include "ofMain.h"
#include "ofxGui.h"

static const int NUM = 6; //色数

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
    
    float a, b;
    glm::vec2 pos, lastPos;
    ofColor colors[NUM], bgColor;
    int colorStep, r;
    ofFbo fbo;
    void draw1();
    
    void roundLine(glm::vec2 p0, glm::vec2 p1);
    void drawLine(glm::vec2 p0, glm::vec2 p1);
    glm::vec2 perpen(glm::vec2 P);
    glm::vec2 inverse(glm::vec2 P);
    void drawCap_round_first(glm::vec2 p0, glm::vec2 p1);
    void drawCap_round_last(glm::vec2 p0, glm::vec2 p1);
    
    // gui
    bool bHide;
    ofxPanel gui;
    ofxIntSlider lineNum;
    ofxIntSlider scale;
    ofxIntSlider parameter;
    ofxIntSlider lineWidth;
    ofxButton reset;
    
};
