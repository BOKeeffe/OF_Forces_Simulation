#include "ofApp.h"

/**
	@file 		ofApp.cpp
	@author		Willaim O'Keeffe
	*/


using namespace YAMPE; using namespace P;

//--------------------------------------------------------------
void ofApp::setup() {
    
    particles.clear() ;
    
    ofSetVerticalSync(true);
    
    // repatable randomness
    ofSeedRandom(10);
    
    // create a minimal gui
    gui.setup("3D Application");
    
    cameraHeightRatio.addListener(this, &ofApp::cameraHeightRatioChanged);
    guiParameters.setName("GUI");
    guiParameters.add(isGuiVisible.set("GUI visible", true));
    guiParameters.add(isDebugVisible.set("Debug visible", true));
    guiParameters.add(isAxisVisible.set("Unit vectors", true));
    guiParameters.add(isXGridVisible.set("Grid (X-axis)", false));
    guiParameters.add(isYGridVisible.set("Grid (Y-axis)", true));
    guiParameters.add(isZGridVisible.set("Grid (Z-axis)", false));
    guiParameters.add(isGroundVisible.set("Ground", true));
    guiParameters.add(cameraHeightRatio.set("Camera (height)", 0.1f, 0., 0.999f));
    
    gui.ofxGuiGroup::add(guiParameters);
    

    // simulation specific stuff goes here
    
    resetButton.addListener(this, &ofApp::reset);
    gui.add(resetButton.setup("Reset"));
    
    quitButton.addListener(this, &ofApp::quitButtonClicked);
    gui.add(quitButton.setup("Quit"));
    
    addBtn.addListener(this, &ofApp::addPend);
    gui.add(addBtn.setup("AddPendulem"));
    
    subBtn.addListener(this, &ofApp::subPend);
    gui.add(subBtn.setup("SubtractPendulem"));
    
    // load gui parameters from file
    gui.loadFromFile("settings.xml");
    
    // instantiate the ground
    ground.set(RANGE, RANGE);
    ground.rotate(90, 1,0,0);
    
    // lift camera to 'eye' level
    easyCam.setDistance(RANGE);
    float d = easyCam.getDistance();
    easyCam.setPosition(0, cameraHeightRatio*d, d*sqrt(1.0f-cameraHeightRatio*cameraHeightRatio));
    easyCam.setTarget(ofVec3f::zero());
    
    font.load(OF_TTF_SANS, 9, true, true);
    
    // simulation specific stuff goes here
    
    isForceVisible = false;
    
    Particle::Ref p(new Particle());
    
    ForceGenerator::Ref fg;
    
    ForceGenerator::Ref gravity(new GravityForceGenerator(ofVec3f(0, -1, 0)));
    
    // bouncing ball due to gravity and (manual) ground collision
    p->setLabel("Bouncing ball");
    p->setBodyColor(ofColor::green);
    p->setWireColor(ofColor::darkGreen);
    //forceGenerators.add(p, gravity);
    particles.push_back(p);
    bouncingParticle = p;
    
    
    // ball on a spring
    p = Particle::Ref(new Particle());
    (*p).setLabel("Spring bob")//
    .setPosition(ofVec3f(2, 10, 0))
    .setBodyColor(ofColor::red)
    .setWireColor(ofColor::darkRed);
    springAnchor = p->position + 8*ofVec3f(0,0,0);
    fg = AnchoredSpringForceGenerator::Ref(new AnchoredSpringForceGenerator(springAnchor, 1, 5));
    forceGenerators.add(p, fg);
    forceGenerators.add(p, gravity);
    particles.push_back(p);
    springParticle = p;
    
    
    // ball on a bunge string
    p = Particle::Ref(new Particle());
    (*p).setLabel("Bungee bob")
    .setPosition(ofVec3f(4, 10, 0))	// start at ground (bungee cord stretched)
    .setBodyColor(ofColor::blue)
    .setWireColor(ofColor::darkBlue);
    bungeeAnchor = p->position + 8*ofVec3f(0,0,0);
    fg = AnchoredBungeeForceGenerator::Ref(new AnchoredBungeeForceGenerator(bungeeAnchor, 10, 5));
    forceGenerators.add(p, fg);
    forceGenerators.add(p, gravity);
    particles.push_back(p);
    bungeeParticle = p;
    
    
    // p1
    p = Particle::Ref(new Particle());
    (*p).setLabel("Pendulum bob")
    .setPosition(ofVec3f(-4, 5.1, 0))
    .setBodyColor(ofColor::blue)
    .setWireColor(ofColor::blue)
    .setVelocity(ofVec3f(0,0,0));
    
    pendulumParticle = p;
    pendulumAnchor = pendulumParticle->position + 5*ofVec3f(0,1,0);
    fg = AnchoredSpringForceGenerator::Ref(new AnchoredSpringForceGenerator(pendulumAnchor, 100,5));
    forceGenerators.add(pendulumParticle, fg);
    forceGenerators.add(pendulumParticle, gravity);
    particles.push_back(pendulumParticle);
    pendulumParticle = p;
    
    
    //  p2
    p = Particle::Ref(new Particle());
    (*p).setLabel("Pendulum bob")
    .setPosition(ofVec3f(-4,-1.1, 0))
    .setBodyColor(ofColor::blue)
    .setWireColor(ofColor::blue)
    .setVelocity(ofVec3f(2,0,0));
    
    pendulumParticle2 = p;
    pendulumAnchor2 = pendulumParticle2->position + 5*ofVec3f(0,1,0);
    fg = SpringForceGenerator::Ref(new SpringForceGenerator(pendulumParticle,1,5));
    forceGenerators.add(pendulumParticle2, fg);
    forceGenerators.add(pendulumParticle2, gravity);
    particles.push_back(pendulumParticle2);
    pendulumParticle2 = p;
    
    
    // p1 red
    p = Particle::Ref(new Particle());
    (*p).setLabel("Pendulum bob")
    .setPosition(ofVec3f(-3, 5.1, 0))
    .setBodyColor(ofColor::blue)
    .setWireColor(ofColor::red)
    .setVelocity(ofVec3f(0,0,0));
    
    pendulumParticleRed = p;
    pendulumAnchorRed = pendulumParticleRed->position + 5*ofVec3f(0,1,0);
    fg = AnchoredSpringForceGenerator::Ref(new AnchoredSpringForceGenerator(pendulumAnchorRed, 100,5));
    forceGenerators.add(pendulumParticleRed, fg);
    forceGenerators.add(pendulumParticleRed, gravity);
    particles.push_back(pendulumParticleRed);
    pendulumParticleRed = p;
    
    
    //  p2 red
    p = Particle::Ref(new Particle());
    (*p).setLabel("Pendulum bob")
    .setPosition(ofVec3f(-3,-1.1, 0))
    .setBodyColor(ofColor::blue)
    .setWireColor(ofColor::red)
    .setVelocity(ofVec3f(2,0,0));
    
    pendulumParticleRed2 = p;
    pendulumAnchor2 = pendulumParticleRed2->position + 5*ofVec3f(0,1,0);
    fg = SpringForceGenerator::Ref(new SpringForceGenerator(pendulumParticleRed,1,5));
    forceGenerators.add(pendulumParticleRed2, fg);
    forceGenerators.add(pendulumParticleRed2, gravity);
    particles.push_back(pendulumParticleRed2);
    pendulumParticleRed2 = p;

    
    //anchor left
    p = Particle::Ref(new Particle());
    (*p).setLabel("anchor left")
    .setPosition(ofVec3f(-5, 5.1, 0))
    .setBodyColor(ofColor::green)
    .setWireColor(ofColor::blue)
    .setVelocity(ofVec3f(0,0,0));
    
    pendulumPartLeft = p;
    pendulumAnchorLeft= pendulumPartLeft->position + 5*ofVec3f(0,1,0);
    fg = AnchoredSpringForceGenerator::Ref(new AnchoredSpringForceGenerator(pendulumAnchorLeft, 1,5));
    forceGenerators.add(pendulumPartLeft, fg);
    particles.push_back(pendulumPartLeft);
    fg = SpringForceGenerator::Ref(new SpringForceGenerator(pendulumPartLeft,0,5));
    forceGenerators.add(pendulumParticle, fg);
    particles.push_back(pendulumParticle);
    pendulumPartLeft = p;
    
    //anchor Right
    p = Particle::Ref(new Particle());
    (*p).setLabel("anchor Right")
    .setPosition(ofVec3f(-2, 5.1, 0))
    .setBodyColor(ofColor::green)
    .setWireColor(ofColor::blue)
    .setVelocity(ofVec3f(0,0,0));
    
    pendulumPartRight = p;
    pendulumAnchorRight= pendulumPartRight->position + 5*ofVec3f(0,1,0);
    fg = AnchoredSpringForceGenerator::Ref(new AnchoredSpringForceGenerator(pendulumAnchorRight, 1,5));
    forceGenerators.add(pendulumPartRight, fg);
    particles.push_back(pendulumPartRight);
    fg = SpringForceGenerator::Ref(new SpringForceGenerator(pendulumPartRight,0,5));
    forceGenerators.add(pendulumParticleRed, fg);
    particles.push_back(pendulumParticleRed);
    pendulumPartRight = p;

    //reset();
    
    // debug GUI
    
    // build text debug page
    debugTextPage.setup("Text");
    debugString = "";
    debugTextPage.add(&debugStringLabel);
    
    // build text debug page (using a panel to allow for multiple graphs)
    debugGraphicsPage.setup("Graphics");
    
    debugGraphicsPanel.setup("TODO");
    debugGraphicsPage.add(&debugGraphicsPanel);
    
    // finally build tabbed pages
    debugPages.setup("Debug", "", debugTextPage.getShape().getRight()+10);
    debugPages.setSize(500, 300);
    debugPages.add(&debugTextPage);
    debugPages.add(&debugGraphicsPage);
    
    // set position of debug GUI to bottom left corner
    debugPages.setPosition(0, ofGetHeight()-debugPages.getHeight());
    
}


void ofApp::reset() {
    
    // simulation specific stuff goes here
    
    Particle::Ref p;
    
    // bouncing ball due to gravity and (manual) ground collision
    p = particles[0];
    p->position = ofVec3f(0, 4.1, 0);
    p->velocity = ofVec3f::zero();
    
    // ball on a spring - start at ground (spring stretched)
    p = particles[1];
    p->position = ofVec3f(2, 0.1, 0);
    p->velocity = ofVec3f::zero();
    
    // ball on a bunge string
    p =particles[2];
    p->position = ofVec3f(4, 0.1, 0);
    p->velocity = ofVec3f::zero();
    
    // pendulum ball
    p =particles[3];
    p->position = ofVec3f(-7.5, 1.1, 0);
    p->velocity = ofVec3f(2.5,0,0);
    
    // pendulum ball 2
    p =particles[4];
    p->position = ofVec3f(-7.5, 1.1, 0);
    p->velocity = ofVec3f(2.5,0,0);
 
}

void ofApp::update() {
    
    float dt = ofClamp(ofGetLastFrameTime(), 0.01, 0.02);
    
    // simulation specific stuff goes here
    forceGenerators.applyForce(dt);
    
    // update all particles
    
    foreach (p, particles) (**p).integrate(dt);
    
    
   	// deal with collisions -- special code
    if (particles[0]->position.y<particles[0]->radius) {
        particles[0]->position.y=particles[0]->radius;
        particles[0]->velocity = -particles[0]->velocity;
    }
}

void ofApp::addPend(){
    
    //particles.clear();
    
    Particle::Ref p(new Particle());
    ForceGenerator::Ref fg;
    ForceGenerator::Ref gravity(new GravityForceGenerator(ofVec3f(0, -1, 0)));
    
    float startPos = 0 - (int)amtPend/2;
    
    for (int i=0; i < (int)amtPend; i++){
        
        if(i < (int)amtPend){
            Particle::Ref p(new Particle());
            
            ForceGenerator::Ref fg;
            
            ForceGenerator::Ref gravity(new GravityForceGenerator(ofVec3f(0, -1, 0)));
            
            
            // p1
            p = Particle::Ref(new Particle());
            (*p).setLabel("Pendulum bob")
            .setPosition(ofVec3f(4, 5.1, 0))
            .setBodyColor(ofColor::blue)
            .setWireColor(ofColor::blue)
            .setVelocity(ofVec3f(0,0,0));
            
            pendulumParticle = p;
            pendulumAnchor = pendulumParticle->position + 5*ofVec3f(0,1,0);
            fg = AnchoredSpringForceGenerator::Ref(new AnchoredSpringForceGenerator(pendulumAnchor, 100,5));
            forceGenerators.add(pendulumParticle, fg);
            forceGenerators.add(pendulumParticle, gravity);
            particles.push_back(pendulumParticle);
            pendulumParticle = p;
            
            
            //  p2
            p = Particle::Ref(new Particle());
            (*p).setLabel("Pendulum bob")
            .setPosition(ofVec3f(4,-1.1, 0))
            .setBodyColor(ofColor::blue)
            .setWireColor(ofColor::blue)
            .setVelocity(ofVec3f(2,0,0));
            
            pendulumParticle2 = p;
            pendulumAnchor2 = pendulumParticle2->position + 5*ofVec3f(0,1,0);
            fg = SpringForceGenerator::Ref(new SpringForceGenerator(pendulumParticle,1,5));
            forceGenerators.add(pendulumParticle2, fg);
            forceGenerators.add(pendulumParticle2, gravity);
            particles.push_back(pendulumParticle2);
            pendulumParticle2 = p;
            
            
            // p1 red
            p = Particle::Ref(new Particle());
            (*p).setLabel("Pendulum bob")
            .setPosition(ofVec3f(3, 5.1, 0))
            .setBodyColor(ofColor::blue)
            .setWireColor(ofColor::red)
            .setVelocity(ofVec3f(0,0,0));
            
            pendulumParticleRed = p;
            pendulumAnchorRed = pendulumParticleRed->position + 5*ofVec3f(0,1,0);
            fg = AnchoredSpringForceGenerator::Ref(new AnchoredSpringForceGenerator(pendulumAnchorRed, 100,5));
            forceGenerators.add(pendulumParticleRed, fg);
            forceGenerators.add(pendulumParticleRed, gravity);
            particles.push_back(pendulumParticleRed);
            pendulumParticleRed = p;
            
            
            //  p2 red
            p = Particle::Ref(new Particle());
            (*p).setLabel("Pendulum bob")
            .setPosition(ofVec3f(3,-1.1, 0))
            .setBodyColor(ofColor::blue)
            .setWireColor(ofColor::red)
            .setVelocity(ofVec3f(2,0,0));
            
            pendulumParticleRed2 = p;
            pendulumAnchor2 = pendulumParticleRed2->position + 5*ofVec3f(0,1,0);
            fg = SpringForceGenerator::Ref(new SpringForceGenerator(pendulumParticleRed,1,5));
            forceGenerators.add(pendulumParticleRed2, fg);
            forceGenerators.add(pendulumParticleRed2, gravity);
            particles.push_back(pendulumParticleRed2);
            pendulumParticleRed2 = p;
            
            
            //anchor left
            p = Particle::Ref(new Particle());
            (*p).setLabel("anchor left")
            .setPosition(ofVec3f(5, 5.1, 0))
            .setBodyColor(ofColor::green)
            .setWireColor(ofColor::blue)
            .setVelocity(ofVec3f(0,0,0));
            
            pendulumPartLeft = p;
            pendulumAnchorLeft= pendulumPartLeft->position + 5*ofVec3f(0,1,0);
            fg = AnchoredSpringForceGenerator::Ref(new AnchoredSpringForceGenerator(pendulumAnchorLeft, 1,5));
            forceGenerators.add(pendulumPartLeft, fg);
            particles.push_back(pendulumPartLeft);
            pendulumPartLeft = p;
            
            //anchor Right
            p = Particle::Ref(new Particle());
            (*p).setLabel("anchor Right")
            .setPosition(ofVec3f(2, 5.1, 0))
            .setBodyColor(ofColor::green)
            .setWireColor(ofColor::blue)
            .setVelocity(ofVec3f(0,0,0));
            
            pendulumPartRight = p;
            pendulumAnchorRight= pendulumPartRight->position + 5*ofVec3f(0,1,0);
            fg = AnchoredSpringForceGenerator::Ref(new AnchoredSpringForceGenerator(pendulumAnchorRight, 1,5));
            forceGenerators.add(pendulumPartRight, fg);
            //forceGenerators.add(pendulumPartLeft, gravity);
            particles.push_back(pendulumPartRight);
            pendulumPartRight = p;
            
            // draw non-physics things (lines, springs)
            ofPolyline pendulum;
            pendulum.addVertex(pendulumAnchor);
            pendulum.addVertex(pendulumParticle->position);
            pendulum.draw();
            
            //Line 2
            ofPolyline pendulum2;
            pendulum2.addVertex(pendulumAnchor);
            pendulum2.addVertex(pendulumParticle->position);
            pendulum2.addVertex(pendulumParticle2->position);
            pendulum2.draw();
            
            //Line red
            ofPolyline pendulumRed;
            pendulumRed.addVertex(pendulumAnchorRed);
            pendulumRed.addVertex(pendulumParticleRed->position);
            pendulumRed.addVertex(pendulumParticleRed2->position);
            pendulumRed.draw();
            
            ofPolyline leftAnchorLine;
            leftAnchorLine.addVertex(pendulumParticle->position);
            leftAnchorLine.addVertex(pendulumPartLeft->position);
            leftAnchorLine.draw();
            
            ofPolyline RightAnchorLine;
            RightAnchorLine.addVertex(pendulumParticleRed->position);
            RightAnchorLine.addVertex(pendulumPartRight->position);
            RightAnchorLine.draw();


        }
        
    }
    amtPend++;
    
}

void ofApp::subPend(){

}

void ofApp::draw() {
    
    
    ofEnableDepthTest();
    ofBackgroundGradient(ofColor(128), ofColor(0), OF_GRADIENT_BAR);
    
    ofPushStyle();
    easyCam.begin();
    
    ofDrawGrid(RANGE/(2*8), 8, false, isXGridVisible, isYGridVisible, isZGridVisible);
    
    if (isAxisVisible) ofDrawAxis(1);
    
    if (isGroundVisible) {
        ofPushStyle();
        ofSetHexColor(0xB87333);
        ground.draw();
        ofPopStyle();
    }
    
    // simulation specific stuff goes here
    
    
    // draw non-physics things (lines, springs)
    ofPolyline pendulum;
    pendulum.addVertex(pendulumAnchor);
    pendulum.addVertex(pendulumParticle->position);
    pendulum.draw();
    
    //Line 2
    ofPolyline pendulum2;
    pendulum2.addVertex(pendulumAnchor);
    pendulum2.addVertex(pendulumParticle->position);
    pendulum2.addVertex(pendulumParticle2->position);
    pendulum2.draw();
    
    //Line red
    ofPolyline pendulumRed;
    pendulumRed.addVertex(pendulumAnchorRed);
    pendulumRed.addVertex(pendulumParticleRed->position);
    pendulumRed.addVertex(pendulumParticleRed2->position);
    pendulumRed.draw();
    
    ofPolyline leftAnchorLine;
    leftAnchorLine.addVertex(pendulumParticle->position);
    leftAnchorLine.addVertex(pendulumPartLeft->position);
    leftAnchorLine.draw();
    
    ofPolyline RightAnchorLine;
    RightAnchorLine.addVertex(pendulumParticleRed->position);
    RightAnchorLine.addVertex(pendulumPartRight->position);
    RightAnchorLine.draw();
    
    
    // render all particles
    foreach (p, particles) (**p).draw();
    
    easyCam.end();
    ofPopStyle();
    
    //position.set(ofToString((int)ofGetFrameRate()));
    
    // finally render any GUI/HUD
    ofDisableDepthTest();
    if (isGuiVisible) gui.draw();
    if (isDebugVisible) {
        
        float pendulumKE = 0.5*pendulumParticle->velocity.lengthSquared();
        float pendulumPE = pendulumParticle->position.y;
        float pendulumTE = pendulumKE+pendulumPE;
        
        float bouncingKE = 0.5*bouncingParticle->velocity.lengthSquared();
        float bouncingPE = bouncingParticle->position.y;
        float bouncingTE = bouncingKE+bouncingPE;
        
        float springKE = 0.5*springParticle->velocity.lengthSquared();
        float springPE = abs((springAnchor-springParticle->position).length()-5);
        float springTE = springKE+springPE;
        
        float bungeeKE = 0.5*bungeeParticle->velocity.lengthSquared();
        float bungeePE = (springAnchor-bungeeParticle->position).length()-5;
        if (bungeePE<0) bungeePE = 0.0f;
        float bungeeTE = bungeeKE+bungeePE;
        
        debugString = String("      Pendulum Bouncing Spring Bungee\n")
        + "  KE   " + ofToString(pendulumKE,4)
        + "  " + ofToString(bouncingKE,4)
        + "  " + ofToString(springKE,4)
        + "  " + ofToString(bungeeKE,4)
        + "\n"
        + "  PE   " + ofToString(pendulumPE,4)
        + "  " + ofToString(bouncingPE,4)
        + "  " + ofToString(springPE,4)
        + "  " + ofToString(bungeePE,4)
        + "\n"
        + "Total  " + ofToString(bouncingTE,4)
        + "  " + ofToString(bouncingTE,4)
        + "  " + ofToString(springTE,4)
        + "  " + ofToString(bungeeTE,4)
        ;
        
        debugStringLabel.setup("",
                               "FPS = " + ofToString((int)ofGetFrameRate()) + "\n\n" + debugString);
        
        debugPages.draw();
    }
    
}


//--------------------------------------------------------------
// GUI events and listeners
//--------------------------------------------------------------

void ofApp::keyPressed(int key) {
    
    float d = easyCam.getDistance();
    
    switch (key) {
            
        case 'h':                               // toggle GUI/HUD
            isGuiVisible = !isGuiVisible;
            break;
        case 'b':                               // toggle debug
            isDebugVisible = !isDebugVisible;
            break;
        case 'a':                               // toggle axis unit vectors
            isAxisVisible = !isAxisVisible;
            break;
        case '1':                               // toggle grids (X)
            isXGridVisible = !isXGridVisible;
            break;
        case '2':                               // toggle grids (Y)
            isYGridVisible = !isYGridVisible;
            break;
        case '3':                               // toggle grids (Z)
            isZGridVisible = !isZGridVisible;
            break;
        case 'g':                               // toggle ground
            isGroundVisible = !isGroundVisible;
            break;
        case 'u':                               // set the up vecetor to be up (ground to be level)
            easyCam.setTarget(ofVec3f::zero());
            break;
            
        case 'S' :                              // save gui parameters to file
            gui.saveToFile("settings.xml");
            
            break;
        case 'L' :                              // load gui parameters
            gui.loadFromFile("settings.xml");
            break;
            
        case 'z':
            easyCam.setPosition(0, cameraHeightRatio*d,
                                d*sqrt(1.0f-cameraHeightRatio*cameraHeightRatio));
            easyCam.setTarget(ofVec3f::zero());
            break;
        case 'Z':
            easyCam.setPosition(0, 0, d);
            easyCam.setTarget(ofVec3f::zero());
            break;
        case 'x':
            easyCam.setPosition(d*sqrt(1.0f-cameraHeightRatio*cameraHeightRatio), cameraHeightRatio*d, 0);
            easyCam.setTarget(ofVec3f::zero());
            break;
        case 'X':
            easyCam.setPosition(d, 0, 0);
            easyCam.setTarget(ofVec3f::zero());
            break;
        case 'Y':
            easyCam.setPosition(0.001, d, 0);
            easyCam.setTarget(ofVec3f::zero());
            break;
            
        case 'f':                               // toggle fullscreen
            // BUG: window size is not preserved
            isFullScreen = !isFullScreen;
            if (isFullScreen) {
                ofSetFullscreen(false);
            } else {
                ofSetFullscreen(true);
            }
            break;
            
            // simulation specific stuff goes here
            
    }
}

void ofApp::cameraHeightRatioChanged(float & cameraHeightRatio) {
    
    float d = easyCam.getDistance();
    easyCam.setPosition(0, cameraHeightRatio*d, d*sqrt(1.0f-cameraHeightRatio*cameraHeightRatio));
    easyCam.setTarget(ofVec3f::zero());
}


void ofApp::quitButtonClicked() {
    ofExit();
}

//--------------------------------------------------------------
// Unused
//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}
void ofApp::mouseMoved(int x, int y ) {}
void ofApp::mouseDragged(int x, int y, int button) {}
void ofApp::mousePressed(int x, int y, int button) {}
void ofApp::mouseReleased(int x, int y, int button) {}
void ofApp::mouseEntered(int x, int y) {}
void ofApp::mouseExited(int x, int y) {}
void ofApp::windowResized(int w, int h) {}
void ofApp::gotMessage(ofMessage msg) {}
void ofApp::dragEvent(ofDragInfo dragInfo) {}
