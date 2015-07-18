#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Scene.h"
#include "SceneBSpline.h"
#include "SceneExtrusionSimple.h"
#include "SceneExtrusionRevolution.h"
#include "SceneExtrusionGeneralisee.h"
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
		void LaunchExtrusionSimpleScene();
		void LaunchExtrusionRevolutionScene();
		void LaunchExtrusionGeneraliseeScene();
		void LaunchSurfaceBezierScene();
		void BackToMainMenu();

		ofxPanel mainGui;
		ofxButton BSplineButton;
		ofxButton ExtrusionSimpleButton;
		ofxButton ExtrusionRevolutionButton;
		ofxButton ExtrusionGeneraliseeButton;
		ofxButton SurfaceBezierButton;

		ofxPanel backGui;
		ofxButton backButton;

		bool runScene;
		SceneBase* currentScene;
};
