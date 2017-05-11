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

//-------------------------------------------------------------------------------------
AwesomeParticles::AwesomeParticles():
    mCookTorrenCB(false),
    mTorrenNayarCB(false),
    mCookTorren(false),
    mTorrenNayar(false)
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


    //// Water
    //ps = mSceneMgr->createParticleSystem("Water", "Elements/Water");
    //mSceneMgr->getRootSceneNode()->attachObject(ps);

    /*
        // Air
        ps = mSceneMgr->createParticleSystem("Air", "Elements/Air");
        mSceneMgr->getRootSceneNode()->attachObject(ps);


        // Earth
        ps = mSceneMgr->createParticleSystem("Earth", "Elements/Earth");
        mSceneMgr->getRootSceneNode()->attachObject(ps);
    	*/
}
bool AwesomeParticles::mouseMoved(const OIS::MouseEvent &evt)
{
    // relay input events to character controller
    if (!mTrayMgr->isDialogVisible()) {
        mChara->injectMouseMove(evt);
    }
    return BaseApplication::mouseMoved(evt);
}

bool AwesomeParticles::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    // relay input events to character controller
    if (!mTrayMgr->isDialogVisible()) {
        mChara->injectMouseDown(evt, id);
    }
    return BaseApplication::mousePressed(evt, id);
}


bool AwesomeParticles::keyPressed(const OIS::KeyEvent &evt)
{
    // relay input events to character controller
    if (!mTrayMgr->isDialogVisible()) {
        mChara->injectKeyDown(evt);
    }
    return BaseApplication::keyPressed(evt);
}

bool AwesomeParticles::keyReleased(const OIS::KeyEvent &evt)
{
    // relay input events to character controller
    if (!mTrayMgr->isDialogVisible()) {
        mChara->injectKeyUp(evt);
    }
    return BaseApplication::keyReleased(evt);
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------


bool AwesomeParticles::setup(void)
{

    if (!BaseApplication::setup()) {
        return false;
    }

    //GUI
    mTrayMgr->showCursor();

    // Load fonts for tray captions
    Ogre::FontManager::getSingleton().getByName("SdkTrays/Caption")->load();
    setupToggles();
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::createScene()
{
    // setup some basic lighting for our scene
    mSceneMgr->setAmbientLight(ColourValue(0.3, 0.3, 0.3));
    mSceneMgr->createLight()->setPosition(20, 80, 50);
    // disable default camera control so the character can do its own
    mCameraMan->setStyle(CS_MANUAL);
    mChara = new SinbadCharacterController(mCamera);
    // create a floor mesh resource
    MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                            Plane(Vector3::UNIT_Y, -30), 1000, 1000, 10, 10, true, 1, 8, 8, Vector3::UNIT_Z);

    // create a floor entity, give it a material, and place it at the origin
    Entity *floor = mSceneMgr->createEntity("Floor", "floor");
    floor->setMaterialName("Examples/BumpyMetal");
    mSceneMgr->getRootSceneNode()->attachObject(floor);
    mSceneMgr->setSkyDome(true, "Examples/CloudySky", 10, 8);
	setupParticles();
}

//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
void AwesomeParticles::destroyScene()
{
    if (mChara) {
        delete mChara;
        mChara = 0;
    }
    MeshManager::getSingleton().remove("floor");
}

bool AwesomeParticles::frameRenderingQueued(const Ogre::FrameEvent &fe)
{
    mChara->addTime(fe.timeSinceLastFrame);
    return BaseApplication::frameRenderingQueued(fe);
}

//-------------------------------------------------------------------------------------
void AwesomeParticles::setupToggles()
{
    // create check boxes to toggle the visibility of our particle systems
    const int WIDTH_UI = 140;

    mTrayMgr->createLabel(OgreBites::TL_TOPLEFT, "Label1", "Lighting Model", WIDTH_UI);
    mCookTorrenCB = mTrayMgr->createCheckBox(OgreBites::TL_TOPLEFT, "CookTorren", "Cook Torren", WIDTH_UI);
    mCookTorrenCB->setChecked(true);
    mTorrenNayarCB = mTrayMgr->createCheckBox(OgreBites::TL_TOPLEFT, "TorrenNayar", "Torren Nayar", WIDTH_UI);

    const char *vecInit[] = {"Fire", "Earth", "Water", "Air"};
    Ogre::StringVector vecElements(vecInit, vecInit + 4);
    mTrayMgr->createLabel(OgreBites::TL_TOPLEFT, "Label3", "Elements", WIDTH_UI);
    mElementMenu = mTrayMgr->createThickSelectMenu(OgreBites::TL_TOPLEFT, "ElementMenu", "Select Element", WIDTH_UI, 4, vecElements);

}

void AwesomeParticles::checkBoxToggled(OgreBites::CheckBox *box)
{
    if (box == mCookTorrenCB) {
        mCookTorren = mCookTorrenCB->isChecked();
    } else if (box == mTorrenNayarCB) {
        mTorrenNayar = mTorrenNayarCB->isChecked();
    }
}

void AwesomeParticles::itemSelected(OgreBites::SelectMenu *menu)
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