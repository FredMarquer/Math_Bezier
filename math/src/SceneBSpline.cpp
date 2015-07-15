#include "SceneBSpline.h"


SceneBSpline::SceneBSpline(void)
{
	nbPas = 50;
}


SceneBSpline::~SceneBSpline(void)
{
}

//--------------------------------------------------------------
void SceneBSpline::Init()
{
	gui.setup();
	gui.setPosition(10, 100);
	gui.add(nbPasSlider.setup("nb Pas", 50, 10, 200));
}

//--------------------------------------------------------------
void SceneBSpline::Update()
{
	if (nbPasSlider != nbPas)
		UpdateNbPas();
}

//--------------------------------------------------------------
void SceneBSpline::Draw()
{
	bSpline.Draw();
	gui.draw();
}

//--------------------------------------------------------------
void SceneBSpline::keyPressed(int key)
{
}

//--------------------------------------------------------------
void SceneBSpline::keyReleased(int key)
{
}

//--------------------------------------------------------------
void SceneBSpline::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void SceneBSpline::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void SceneBSpline::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void SceneBSpline::mouseReleased(int x, int y, int button)
{
}


void SceneBSpline::UpdateNbPas()
{
	nbPas = nbPasSlider;
	bSpline.nbPas = nbPasSlider;
	bSpline.GenerateBSpline();
}