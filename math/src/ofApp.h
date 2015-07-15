#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Scene.h"
#include "SceneBSpline.h"
#include "ScenePrimitiveExtrusion.h"
#include "SceneSurfaceBezier.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void LaunchBSplineScene();
		void LaunchPrimitiveExtrusionScene();
		void LaunchSurfaceBezierScene();
		void BackToMainMenu();

		ofxPanel mainGui;
		ofxButton BSplineButton;
		ofxButton PrimitiveExtrusionButton;
		ofxButton SurfaceBezierButton;

		ofxPanel backGui;
		ofxButton backButton;

		bool runScene;
		SceneBase* currentScene;
};
