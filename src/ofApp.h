#pragma once

/**
	@file 		ofApp.h
	@author		Willaim O'Keeffe
    @brief      ofApp header file
	*/

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxGuiExtended.h"
#include "ofxXmlSettings.h"

#include "ParticleForceGeneratorRegistry.h"

class ofApp : public ofBaseApp {
    
    
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
    void gotMessage(ofMessage msg);
		
    // simple 3D world with ground and axes
    const float RANGE = 16;
    ofEasyCam easyCam;
    ofPlanePrimitive ground;
    
    // ofxGui code
    ofxPanelExtended gui;
    ofParameterGroup guiParameters;
    ofParameter<bool> isGuiVisible;
    ofParameter<bool> isDebugVisible;
    ofParameter<bool> isAxisVisible;
    ofParameter<bool> isXGridVisible;
    ofParameter<bool> isYGridVisible;
    ofParameter<bool> isZGridVisible;
    ofParameter<bool> isGroundVisible;
    ofParameter<bool> isFullScreen;
    ofParameter<std::string> position;
    
    ofParameter<int> nParticle;
    ofParameter<int> nParticlesOffSet;
    
    
    ofParameter<float> cameraHeightRatio;
    void cameraHeightRatioChanged(float & cameraHeightRatio);

    void reset();
    ofxMinimalButton resetButton;

    ofxMinimalButton quitButton;
    void quitButtonClicked();

    ofxButton addBtn;
    void addPend();
    
    ofxButton subBtn;
    void subPend()
    
    float amtPend = 4;
    float offSet = 1;
    float start;
    

    ofTrueTypeFont font;
    
    // simulation specific stuff goes here
    
    // debug code
    ofxTabbedPages debugPages;
    ofxGuiPage debugTextPage, debugGraphicsPage;
    ofxPanelExtended debugGraphicsPanel;

    ofxLabel debugStringLabel;
    string debugString;

    // simulation specific stuff goes here (debug)

private:

    // or here
    
    YAMPE::P::ForceGeneratorRegistry forceGenerators;
    YAMPE::ParticleRegistry particles;
    
    bool isForceVisible;

    // needed for drawing and calcualting energy
    YAMPE::Particle::Ref bouncingParticle;

    YAMPE::Particle::Ref springParticle;
    ofVec3f springAnchor;
    
    YAMPE::Particle::Ref bungeeParticle;
    ofVec3f bungeeAnchor;

    YAMPE::Particle::Ref pendulumParticle;
    ofVec3f pendulumAnchor;
    
    YAMPE::Particle::Ref pendulumParticle2;
    ofVec3f pendulumAnchor2;
    
    YAMPE::Particle::Ref pendulumParticleRed;
    ofVec3f pendulumAnchorRed;
    
    YAMPE::Particle::Ref pendulumParticleRed2;
    ofVec3f pendulumAnchorRed2;
    
    YAMPE::Particle::Ref pendulumPartLeft;
    ofVec3f pendulumAnchorLeft;
    
    YAMPE::Particle::Ref pendulumPartRight;
    ofVec3f pendulumAnchorRight;
};
