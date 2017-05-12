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
bool isUIvisible = false;

//-------------------------------------------------------------------------------------
AwesomeParticles::AwesomeParticles()
{
}
//-------------------------------------------------------------------------------------
AwesomeParticles::~AwesomeParticles()
{
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::setupParticles()
{
    ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);
    ParticleSystem *ps;

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
//-------------------------------------------------------------------------------------
bool AwesomeParticles::mouseMoved(const OIS::MouseEvent &evt)
{
    // relay input events to character controller
    if (!isUIvisible) {
        mChara->injectMouseMove(evt);
    }
    return BaseApplication::mouseMoved(evt);
}
//-------------------------------------------------------------------------------------
bool AwesomeParticles::mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id)
{
    // relay input events to character controller
    if (!isUIvisible) {
        mChara->injectMouseDown(evt, id);
    }
    return BaseApplication::mousePressed(evt, id);
}
//-------------------------------------------------------------------------------------
bool AwesomeParticles::keyPressed(const OIS::KeyEvent &evt)
{
    // relay input events to character controller
    if (!mTrayMgr->isDialogVisible()) {
        mChara->injectKeyDown(evt);
        if (evt.key == OIS::KC_ESCAPE) {
            if (!isUIvisible) {
                mTrayMgr->showAll();
                setMenuVisible("MainMenu");
                setMenuVisible("Option", false);
                mTrayMgr->showCursor();
                isUIvisible = true;
            } else {
                mTrayMgr->hideAll();
                mTrayMgr->hideCursor();
                isUIvisible = false;
            }
        }
    }
    return true;
}
//-------------------------------------------------------------------------------------
bool AwesomeParticles::keyReleased(const OIS::KeyEvent &evt)
{
    // relay input events to character controller
    if (!isUIvisible) {
        mChara->injectKeyUp(evt);
    }
    return BaseApplication::keyReleased(evt);
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::buttonHit(Button *b)
{
    if (b->getName() == "mQuitButton") {
        mRoot->queueEndRendering();
    } else if (b->getName() == "mOptionButton") {
        setMenuVisible("Option");
        setMenuVisible("MainMenu", false);
    }
}
//-------------------------------------------------------------------------------------
bool AwesomeParticles::setup(void)
{

    if (!BaseApplication::setup()) {
        return false;
    }

    // Load fonts for tray captions
    FontManager::getSingleton().getByName("SdkTrays/Caption")->load();
    setupWidgets();
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
                                            Plane(Vector3::UNIT_Y, -30), 10000, 10000, 10, 10, true, 1, 8, 8, Vector3::UNIT_Z);

    // create a floor entity, give it a material, and place it at the origin
    Entity *floor = mSceneMgr->createEntity("Floor", "floor");
    floor->setMaterialName("Examples/BeachStones");
    mSceneMgr->getRootSceneNode()->attachObject(floor);
    mSceneMgr->setSkyDome(true, "Examples/CloudySky", 10, 8);
    setupParticles();
}

//-------------------------------------------------------------------------------------
void AwesomeParticles::setMenuVisible(const String &name, bool visible)
{
    if (name == "MainMenu") {
        if (visible) {
            mTrayMgr->moveWidgetToTray("mMainMenuLabel", TL_CENTER);
            mTrayMgr->moveWidgetToTray("mOptionButton", TL_CENTER);
            mTrayMgr->moveWidgetToTray("mCreditButton", TL_CENTER);
            mTrayMgr->moveWidgetToTray("mQuitButton", TL_CENTER);
            mTrayMgr->getWidget("mMainMenuLabel")->show();
            mTrayMgr->getWidget("mOptionButton")->show();
            mTrayMgr->getWidget("mCreditButton")->show();
            mTrayMgr->getWidget("mQuitButton")->show();
        } else {
            mTrayMgr->removeWidgetFromTray("mMainMenuLabel");
            mTrayMgr->removeWidgetFromTray("mOptionButton");
            mTrayMgr->removeWidgetFromTray("mCreditButton");
            mTrayMgr->removeWidgetFromTray("mQuitButton");
            mTrayMgr->getWidget("mMainMenuLabel")->hide();
            mTrayMgr->getWidget("mOptionButton")->hide();
            mTrayMgr->getWidget("mCreditButton")->hide();
            mTrayMgr->getWidget("mQuitButton")->hide();
        }
    } else if (name == "Option") {
        if (visible) {
            mTrayMgr->moveWidgetToTray("mlightingModelLabel", TL_CENTER);
            mTrayMgr->moveWidgetToTray("mCookTorrenCB", TL_CENTER);
            mTrayMgr->moveWidgetToTray("mTorrenNayarCB", TL_CENTER);
            mTrayMgr->moveWidgetToTray("mElementLabel", TL_CENTER);
            mTrayMgr->moveWidgetToTray("mELemenSelect", TL_CENTER);
            mTrayMgr->getWidget("mlightingModelLabel")->show();
            mTrayMgr->getWidget("mCookTorrenCB")->show();
            mTrayMgr->getWidget("mTorrenNayarCB")->show();
            mTrayMgr->getWidget("mElementLabel")->show();
            mTrayMgr->getWidget("mELemenSelect")->show();
        } else {
            mTrayMgr->removeWidgetFromTray("mlightingModelLabel");
            mTrayMgr->removeWidgetFromTray("mCookTorrenCB");
            mTrayMgr->removeWidgetFromTray("mTorrenNayarCB");
            mTrayMgr->removeWidgetFromTray("mElementLabel");
            mTrayMgr->removeWidgetFromTray("mELemenSelect");
            mTrayMgr->getWidget("mlightingModelLabel")->hide();
            mTrayMgr->getWidget("mCookTorrenCB")->hide();
            mTrayMgr->getWidget("mTorrenNayarCB")->hide();
            mTrayMgr->getWidget("mElementLabel")->hide();
            mTrayMgr->getWidget("mELemenSelect")->hide();
        }
    }
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::destroyScene()
{
    if (mChara) {
        delete mChara;
        mChara = 0;
    }
    if (mTrayMgr) {
        mTrayMgr->destroyAllWidgets();
    }
    MeshManager::getSingleton().remove("floor");
}
//-------------------------------------------------------------------------------------
bool AwesomeParticles::frameRenderingQueued(const FrameEvent &fe)
{
    mChara->addTime(fe.timeSinceLastFrame);
    mTrayMgr->frameRenderingQueued(fe);
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    return true;
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::setupWidgets()
{
    mTrayMgr->destroyAllWidgets();
    // create check boxes to toggle the visibility of our particle systems
    const int WIDTH_UI = 140;

    mTrayMgr->createLabel(TL_NONE, "mlightingModelLabel", "Lighting Model", WIDTH_UI);
    mTrayMgr->createCheckBox(TL_NONE, "mCookTorrenCB", "Cook Torren", WIDTH_UI);
    mTrayMgr->createCheckBox(TL_NONE, "mTorrenNayarCB", "Torren Nayar", WIDTH_UI);

    const char *vecInit[] = {"Fire", "Earth", "Water", "Air"};
    StringVector vecElements(vecInit, vecInit + 4);
    mTrayMgr->createLabel(TL_NONE, "mElementLabel", "Elements", WIDTH_UI);
    mTrayMgr->createThickSelectMenu(TL_NONE, "mELemenSelect", "Select Element", WIDTH_UI, 4, vecElements);

    // main menu
    mTrayMgr->createLabel(TL_NONE, "mMainMenuLabel", "Main Menu", WIDTH_UI);
    mTrayMgr->createButton(TL_NONE, "mOptionButton", "Option");
    mTrayMgr->createButton(TL_NONE, "mCreditButton", "About");
    mTrayMgr->createButton(TL_NONE, "mQuitButton", "Quit");
    mTrayMgr->hideAll();

}
//-------------------------------------------------------------------------------------
void AwesomeParticles::checkBoxToggled(CheckBox *box)
{
    if (box->getName() == "mCookTorrenCB") {
        mCookTorren = box->isChecked();
    } else if (box->getName() == "mTorrenNayarCB") {
        mTorrenNayar = box->isChecked();
    }
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::itemSelected(SelectMenu *menu)
{

}
//-------------------------------------------------------------------------------------

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
    } catch (Exception &e) {
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