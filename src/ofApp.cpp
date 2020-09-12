#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    box2d.init();
    box2d.setFPS(60.0);
    box2d.setGravity(0, 10);
    
    // アウトライン
    mikuSvg.load("miku.svg");
    mikuPng.load("miku.png");
    outline = true;
    
    for (int i = 0; i < mikuSvg.getNumPath(); i++) {
        ofPath shape = mikuSvg.getPathAt(i);
        for (int j = 0; j < shape.getOutline().size(); j++) {
            ofPolyline line = shape.getOutline().at(j);
            if(line.isClosed()){
                line.addVertex(line.getVertices()[0]);
            }
            shared_ptr<ofxBox2dEdge> edge = shared_ptr<ofxBox2dEdge>(new ofxBox2dEdge);
            edge.get()->addVertexes(line);
            edge.get()->setPhysics(0.0, 0.1, 0.7);
            edge->create(box2d.getWorld());
            edges.push_back(edge);
        }
    }
    
    //svgをロード
    dir.listDir("items/");
    dir.allowExt("svg");
    dir.sort();
    if (dir.size()) {
        itemSvg.assign(dir.size(), ofxSVG());
    }
    for (int i = 0; i < (int)dir.size(); i++) {
        itemSvg[i].load(dir.getPath(i));
    }
    //svgの頂点を配列に保存
    for (int i = 0; i < itemSvg.size(); i++) {
    for (int j = 0; j < itemSvg[i].getNumPath(); j++) {
        ofPath p = itemSvg[i].getPathAt(j);
        p.setPolyWindingMode(OF_POLY_WINDING_ODD);
        vector<ofPolyline> vpl= p.getOutline();
        vp.push_back(vector<glm::vec3>());
        for (int k = 0; k < vpl.size(); k++) {
            ofPolyline pl = vpl[k];
            vp[i] = pl.getVertices();
        }
    }
    }
    
    //png画像をロード
    dir.listDir("png/");
    dir.allowExt("png");
    dir.sort();
    if (dir.size()) {
        itemPng.assign(dir.size(), ofImage());
    }
    for (int i = 0; i < (int)dir.size(); i++) {
        itemPng[i].load(dir.getPath(i));
    }
    
    count = 0;
    count2 = 4;

}

//--------------------------------------------------------------
void ofApp::update(){
    box2d.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(214, 241, 218);
    
    // polygon
    for (int i = 0; i < polyShapes.size(); i++){
        ofPoint pos = polyShapes[i].get()->getPosition();
        float rot = polyShapes[i].get()->getRotation();
        
        ofPushMatrix();
        ofTranslate(pos.x, pos.y);
        ofRotateDeg(rot);
        ofScale(0.9);
        ofSetColor(255, 255, 255);
        itemPng[pcount[i]].setAnchorPercent(0.5, 0.5);
        itemPng[pcount[i]].draw(0, 0);
        ofPopMatrix();
    }
    
    //アウトライン（png画像）
    ofSetColor(255);
    if(outline){
    mikuPng.draw(0, 0);
    }
    
    myImage.grabScreen(0, 0, ofGetWidth(),  ofGetHeight());

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == ' ') { // create a new polygon
        if (count == itemSvg.size()) {
            count = 0;
        }
        auto poly = std::make_shared<ofxBox2dPolygon>();
        for (int i = 0; i < vp[count].size(); i++){
            poly->addVertex(mouseX + vp[count][i].x - itemSvg[count].getWidth()/2, mouseY + vp[count][i].y - itemSvg[count].getHeight()/2);
        }
        poly->setPhysics(1.0, 0.5, 0.1);
        poly->simplifyToMaxVerts();
        poly->create(box2d.getWorld());
        polyShapes.push_back(poly);
        pcount.push_back(count);
        count += 1;
    }
    
    if (key == 't' || key == 'T') { // create a new triangle
           auto poly = std::make_shared<ofxBox2dPolygon>();
           for (int i = 0; i < vp[count2].size(); i++){
               poly->addVertex(mouseX + vp[count2][i].x - itemSvg[count2].getWidth()/2, mouseY + vp[count2][i].y - itemSvg[count2].getHeight()/2);
           }
           poly->setPhysics(1.0, 0.5, 0.1);
           poly->simplifyToMaxVerts();
           poly->create(box2d.getWorld());
           polyShapes.push_back(poly);
           pcount.push_back(count2);
        if (count2 == 4) {
            count2 = 7;
        } else if (count2 == 7){
            count2 = 9;
        } else if (count2 == 9){
            count2 = 4;
        }
       }
       
    
    if (key == 'o' || key == 'O') {
    
            outline=!outline; // draw shape outline
    }
    
    if (key == 'c' || key == 'C') {
    
        polyShapes.clear();
        pcount.clear();
    }
    
    if (key == 'S' || key == 's') {
        myImage.save(ofGetTimestampString("%Y%m%d%H%M%S")+"##.png", OF_IMAGE_QUALITY_HIGH);
    }
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
void ofApp::mousePressed(int x, int y, int button){

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
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
