#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	runScene = false;

	ofSetVerticalSync(true);

	BSplineButton.addListener(this,&ofApp::LaunchBSplineScene);
	ExtrusionSimpleButton.addListener(this,&ofApp::LaunchExtrusionSimpleScene);
	ExtrusionRevolutionButton.addListener(this,&ofApp::LaunchExtrusionRevolutionScene);
	ExtrusionGeneraliseeButton.addListener(this,&ofApp::LaunchExtrusionGeneraliseeScene);
	SurfaceBezierButton.addListener(this,&ofApp::LaunchSurfaceBezierScene);
	backButton.addListener(this,&ofApp::BackToMainMenu);

	mainGui.setup();
	mainGui.add(BSplineButton.setup("B-Spline"));
	mainGui.add(ExtrusionSimpleButton.setup("Extrusion simple"));
	mainGui.add(ExtrusionRevolutionButton.setup("Extrusion revolution"));
	mainGui.add(ExtrusionGeneraliseeButton.setup("Extrusion géneralisée"));
	mainGui.add(SurfaceBezierButton.setup("Surface de bezier"));

	backGui.setup();
	backGui.add(backButton.setup("Menu"));
}

//--------------------------------------------------------------
void ofApp::update()
{
	if (runScene)
		currentScene->Update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackgroundGradient(ofColor::white, ofColor::gray);
	
	if (runScene) {
		currentScene->Draw();
		backGui.draw();
	}
	else
		mainGui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (runScene)
		currentScene->keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	if (runScene)
		currentScene->keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
	if (runScene)
		currentScene->mouseMoved(x, y);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
	if (runScene)
		currentScene->mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
	if (runScene)
		currentScene->mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
	if (runScene)
		currentScene->mouseReleased(x, y, button);
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{ 
}

//--------------------------------------------------------------
void ofApp::LaunchBSplineScene()
{
	runScene = true;
	currentScene = new SceneBSpline;
	currentScene->Init();
}

//--------------------------------------------------------------
void ofApp::LaunchExtrusionSimpleScene()
{
	runScene = true;
	currentScene = new SceneExtrusionSimple;
	currentScene->Init();
}

//--------------------------------------------------------------
void ofApp::LaunchExtrusionRevolutionScene()
{
	runScene = true;
	currentScene = new SceneExtrusionRevolution;
	currentScene->Init();
}

//--------------------------------------------------------------
void ofApp::LaunchExtrusionGeneraliseeScene()
{
	runScene = true;
	currentScene = new SceneExtrusionGeneralisee;
	currentScene->Init();
}

//--------------------------------------------------------------
void ofApp::LaunchSurfaceBezierScene()
{
	runScene = true;
	currentScene = new SceneSurfaceBezier;
	currentScene->Init();
}

//--------------------------------------------------------------
void ofApp::BackToMainMenu()
{
	runScene = false;
	//delete(currentScene);
}