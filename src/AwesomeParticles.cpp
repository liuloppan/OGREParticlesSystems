/*
-----------------------------------------------------------------------------
Filename:    AwesomeParticles.cpp
-----------------------------------------------------------------------------
This source file is part of the
   _____
  /  _  \__  _  __ ____   __________   _____   ____
 /  /_\  \ \/ \/ // __ \ /  ___/  _ \ /     \_/ __ \
/    |    \     /\  ___/ \___ (  <_> )  Y Y  \  ___/
\____|__  /\/\_/  \___  >____  >____/|__|_|  /\___  >
        \/            \/     \/            \/     \/
__________                __  .__       .__
\______   \_____ ________/  |_|__| ____ |  |   ____   ______
 |     ___/\__  \\_  __ \   __\  |/ ___\|  | _/ __ \ /  ___/
 |    |     / __ \|  | \/|  | |  \  \___|  |_\  ___/ \___ \
 |____|    (____  /__|   |__| |__|\___  >____/\___  >____  >
                \/                    \/          \/     \/
      Particle System
      https://github.com/liuloppan/OGREParticlesSystems
-----------------------------------------------------------------------------
*/
#include "AwesomeParticles.h"

using namespace Ogre;
using namespace OgreBites;
//-------------------------------------------------------------------------------------
AwesomeParticles::AwesomeParticles():
    mInfoLabel(0),
    mSinbadCB(false),
    sinbadEntity(NULL),
    mSinbad(false),
    mNinjaCB(false),
    ninjaEntity(NULL),
    mNinja(false),
    mCookTorrenCB(false),
    mTorrenNayarCB(false)
{
}
//-------------------------------------------------------------------------------------
AwesomeParticles::~AwesomeParticles()
{
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::setupParticles()
{
    Ogre::ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);
    Ogre::ParticleSystem *ps;

    // Fire
    ps = mSceneMgr->createParticleSystem("Fire", "Elements/Fire");
    mSceneMgr->getRootSceneNode()->attachObject(ps);

    // Water
    ps = mSceneMgr->createParticleSystem("Water", "Elements/Water");
    mSceneMgr->getRootSceneNode()->attachObject(ps);

    // Air
    ps = mSceneMgr->createParticleSystem("Air", "Elements/Air");
    mSceneMgr->getRootSceneNode()->attachObject(ps);

    // Earth
    ps = mSceneMgr->createParticleSystem("Earth", "Elements/Earth");
    mSceneMgr->getRootSceneNode()->attachObject(ps);

}
//-------------------------------------------------------------------------------------

void AwesomeParticles::createCamera()
{
    // override the camera :D
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // set camera position
    mCamera->setPosition(Ogre::Vector3(0, 300, 500));

    // focus the camera to what we want to see :v
    mCamera->lookAt(Ogre::Vector3(0, 0, 0));

    // set near cliping - it means camera will not render any mesh at those distance
    mCamera->setNearClipDistance(.1);

    // create new sdk camera man
    mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::createViewports()
{
    // add new viewport
    Ogre::Viewport *vp = mWindow->addViewport(mCamera);

    // set the background colour
    vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

    // set aspect ratio
    mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) /
        Ogre::Real(vp->getActualHeight()));
}
bool AwesomeParticles::setup(void)
{

    if (!BaseApplication::setup()) {
        return false;
    }

    //GUI
    mTrayMgr->showCursor();
    setupToggles();
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::createScene()
{
    // set ambient light : red-green-blue
    mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 0.5));

    // set 3d objects
    ninjaEntity = mSceneMgr->createEntity("ninja.mesh");
    ninjaEntity->setCastShadows(true);
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ninjaEntity);
    ninjaEntity->setVisible(false);

    sinbadEntity = mSceneMgr->createEntity("Sinbad.mesh");
    sinbadEntity->setCastShadows(true);
    mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(sinbadEntity);
    sinbadEntity->setVisible(false);

    // light
    Ogre::Light *spotLight = mSceneMgr->createLight("SpotLight");
    spotLight->setDiffuseColour(0, 0, 1.0);
    spotLight->setSpecularColour(0, 0, 1.0);
    spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
    spotLight->setDirection(-1, -1, 0);
    spotLight->setPosition(Ogre::Vector3(200, 200, 0));
    spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));

    Ogre::Light *directionalLight = mSceneMgr->createLight("DirectionalLight");
    directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
    directionalLight->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
    directionalLight->setSpecularColour(Ogre::ColourValue(.5, 0, 0));
    directionalLight->setDirection(Ogre::Vector3(0, -1, 1));


    mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
    setupParticles();
}

//-------------------------------------------------------------------------------------
void AwesomeParticles::createFrameListener()
{
    BaseApplication::createFrameListener();

    mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "Awesome Particles", "", 350);
}

//-------------------------------------------------------------------------------------
void AwesomeParticles::destroyScene()
{

}

bool AwesomeParticles::frameRenderingQueued(const Ogre::FrameEvent &fe)
{
    bool ret = BaseApplication::frameRenderingQueued(fe);
    mTrayMgr->removeWidgetFromTray(mInfoLabel);
    mInfoLabel->hide();

    if (ninjaEntity != NULL) {
        ninjaEntity->setVisible(mNinja);
    }

    if (sinbadEntity != NULL) {
        sinbadEntity->setVisible(mSinbad);
    }

    return ret;
}

//-------------------------------------------------------------------------------------
void AwesomeParticles::setupToggles()
{
    // create check boxes to toggle the visibility of our particle systems
    const int WIDTH_UI = 140;

    mTrayMgr->createLabel(TL_TOPLEFT, "Label1", "Lighting Model", WIDTH_UI);
    mCookTorrenCB = mTrayMgr->createCheckBox(TL_TOPLEFT, "CookTorren", "Cook Torren", WIDTH_UI);
    mTorrenNayarCB = mTrayMgr->createCheckBox(TL_TOPLEFT, "TorrenNayar", "Torren Nayar", WIDTH_UI);

    mTrayMgr->createLabel(TL_TOPLEFT, "Label2", "Select Hero", WIDTH_UI);
    mNinjaCB = mTrayMgr->createCheckBox(TL_TOPLEFT, "Ninja", "Ninja", WIDTH_UI);
    mSinbadCB = mTrayMgr->createCheckBox(TL_TOPLEFT, "Sinbad", "Sinbad", WIDTH_UI);

    const char *vecInit[] = {"Fire", "Earth", "Water", "Air"};
    Ogre::StringVector vecElements(vecInit, vecInit + 4);
    mTrayMgr->createLabel(TL_TOPLEFT, "Label3", "Elements", WIDTH_UI);
    mLightingMenu = mTrayMgr->createThickSelectMenu(TL_TOPLEFT, "ElementMenu", "Select Element", WIDTH_UI, 4, vecElements);

}

void AwesomeParticles::checkBoxToggled(CheckBox *box)
{
    if (box == mNinjaCB) {
        mNinja = mNinjaCB->isChecked();
    } else if (box == mSinbadCB) {
        mSinbad = mSinbadCB->isChecked();
    }
}

void AwesomeParticles::itemSelected(SelectMenu *menu)
{
    // WIP
    if (menu->getSelectedItem() == "Fire") {
        mMenuName ? true : false;
    }
    mSceneMgr->getParticleSystem(menu->getName())->setVisible(mMenuName);
}


#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char *argv[])
#endif
{
    // Create application object
    AwesomeParticles app;

    try {
        app.go();
    } catch (Ogre::Exception &e) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox(NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " <<
                  e.getFullDescription().c_str() << std::endl;
#endif
    }

    return 0;
}

#ifdef __cplusplus
}
#endif