#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    
    ofSetBackgroundAuto(false);
    
    //gui
    gui.setup();
    gui.add(lineNum.setup("line num", 200, 1, 400));
    gui.add(scale.setup("scale", 20, 5, 60));
    gui.add(parameter.setup(" ", 17, 1, 39));
    gui.add(lineWidth.setup("line width", 2, 2, 40));
    gui.add(reset.setup("reset draw"));
    bHide = false;
    
    //color
    bgColor = ofColor::fromHex(0xe7e7e7);
    colors[0] = ofColor::fromHex(0x96DFD8);
    colors[1] = ofColor::fromHex(0x85D4BE);
    colors[2] = ofColor::fromHex(0xAEE6CB);
    colors[3] = ofColor::fromHex(0xCDEEF3);
    colors[4] = ofColor::fromHex(0xDAF1DE);
    colors[5] = ofColor::fromHex(0xD6E9AA);
    
    //fbo
    fbo.allocate(ofGetWidth(), ofGetHeight());
    
    fbo.begin();
    ofBackground(bgColor);
    fbo.end();
    
    a = 0;
    b = 0;
    pos = glm::vec2(ofGetWidth()/2, ofGetHeight()/2);
    colorStep = 0;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    fbo.begin();
    
    //guiのresetを押したとき、fboの描画をリセットする
    if(reset){
        ofSetColor(bgColor);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        a = 0;
        b = 0;
        pos = glm::vec2(ofGetWidth()/2, ofGetHeight()/2);
        colorStep = 0;
    }
    
    for (int i = 0; i < lineNum; i++) {
        draw1();
    }
    
    fbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255, 255, 255);
    fbo.draw(0, 0);
    
    if(!bHide){
        gui.draw();
    }

}

//--------------------------------------------------------------
void ofApp::draw1(){
    a += b * DEG_TO_RAD;
   
    b = b + parameter;
    
    lastPos = pos;
    
    if (colorStep > 0) {
        _lastPos = lastPos;
    }
    
    glm::vec2 d = glm::vec2(cos(a), sin(a));
    float len = scale;
    pos += d * len;
    
    if (colorStep % 100 == 0) {
        int r = (int)ofRandom(NUM);
        color = colors[r];
    }
    
    
    ofSetLineWidth(lineWidth);
    ofSetColor(color);
    roundLine(lastPos, pos);
    
    colorStep ++;
    
    
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'h' || key == 'H'){
        bHide = !bHide;
    }else if (key == 'S' || key == 's') {
            ofImage myImage;
            myImage.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
            myImage.save(ofGetTimestampString("%Y%m%d%H%M%S")+"##.png");
        }

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    lastPos = pos;
    pos = glm::vec2(x, y);
    colorStep = 0;
}

//--------------------------------------------------------------

//roundLine

//--------------------------------------------------------------

void ofApp::roundLine(glm::vec2 p0, glm::vec2 p1){
    drawCap_round_first(p0, p1);
    drawLine(p0, p1);
    drawCap_round_last(p0, p1);
}

//--------------------------------------------------------------
void ofApp::drawLine(glm::vec2 p0, glm::vec2 p1){
    glm::vec2 base;
    glm::vec2 T[4];
    float half_lineWidth = lineWidth/2;
    //最初のライン
    base = p0-p1;
    base = glm::normalize(base);
    base *= half_lineWidth;
    T[0] = p0 + perpen(base);
    T[1] = p0 + inverse(perpen(base));
    T[2] = p1+ perpen(base);
    T[3] = p1 + inverse(perpen(base));
        
    ofMesh mesh1;
    mesh1.addVertex(ofPoint(T[0].x, T[0].y, 0));
    mesh1.addVertex(ofPoint(T[1].x, T[1].y, 0));
    mesh1.addVertex(ofPoint(T[2].x, T[2].y, 0));
    mesh1.addVertex(ofPoint(T[3].x, T[3].y, 0));
    mesh1.addIndex(0);
    mesh1.addIndex(1);
    mesh1.addIndex(2);
    mesh1.addIndex(1);
    mesh1.addIndex(2);
    mesh1.addIndex(3);
    
    ofFill();
    mesh1.draw();
}

//--------------------------------------------------------------
glm::vec2 ofApp::perpen(glm::vec2 P){
    //垂直、反時計回りに９０度
    return glm::vec2(P.y, -P.x);
}
//--------------------------------------------------------------
glm::vec2 ofApp::inverse(glm::vec2 P){
    //１８０度反転
    return glm::vec2(-P.x, -P.y);
}
//--------------------------------------------------------------
void ofApp::drawCap_round_first(glm::vec2 p0, glm::vec2 p1){
    //最初のキャップ
    
    vector<glm::vec2> A;
    float half_lineWidth = lineWidth/2;
    
    float angle1 = atan2(p1.y-p0.y, p1.x-p0.x);
    if (angle1 < 0) {angle1 = TWO_PI + angle1;}
    angle1 = angle1+HALF_PI;
    for (float ang = angle1; ang <= angle1 + PI; ang += PI/18) {
        float x = p0.x + (half_lineWidth * cos(ang));
        float y = p0.y + (half_lineWidth * sin(ang));
        A.push_back(glm::vec2(x, y));
    }
    
    ofMesh mesh;
    mesh.addVertex(ofPoint(p0.x, p0.y));
    for (int i = 0; i < A.size(); i++) {
        mesh.addVertex(ofPoint(A[i].x, A[i].y, 0));
    }
    
    for (int i = 1; i < A.size(); i++) {
        mesh.addIndex(0);
        mesh.addIndex(i);
        mesh.addIndex(i+1);
    }
    
    ofFill();
    mesh.draw();
}
//--------------------------------------------------------------
void ofApp::drawCap_round_last(glm::vec2 p0, glm::vec2 p1){
    //最後のキャップ
    vector<glm::vec2> A;
    float half_lineWidth = lineWidth/2;
    
    float angle2 = atan2(p0.y-p1.y, p0.x-p1.x);
    if (angle2 < 0) {angle2 = TWO_PI + angle2;}
    angle2 = angle2 + HALF_PI;
    for (float ang = angle2; ang <= angle2+PI; ang += PI/18) {
        float x = p1.x + (half_lineWidth * cos(ang));
        float y = p1.y + (half_lineWidth * sin(ang));
        A.push_back(glm::vec2(x, y));
    }
    ofMesh mesh;
    mesh.addVertex(ofPoint(p1.x, p1.y));
    for (int i = 0; i < A.size(); i++) {
        mesh.addVertex(ofPoint(A[i].x, A[i].y, 0));
    }
    
    for (int i = 1; i < A.size(); i++) {
        mesh.addIndex(0);
        mesh.addIndex(i);
        mesh.addIndex(i+1);
    }
    
    //ofSetColor(col);
    ofFill();
    mesh.draw();
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}
//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
