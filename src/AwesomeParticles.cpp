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
    mInfo["About"] = "Awesome Particles @2017.\n"
                     "Created for 3D Game Programming at Computer Scicence Yuan Ze University\n"
                     "Developer :\n"
                     "Lovisa Hassler\n"
                     "Rosdyana Kusuma\n"
                     "Project available on : https://github.com/liuloppan/OGREParticlesSystems";
}
//-------------------------------------------------------------------------------------
AwesomeParticles::~AwesomeParticles()
{
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::setupParticles()
{
    ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);
    mElementNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("elementNode", Vector3::UNIT_Y * CHAR_HEIGHT);

    // Fire
    mParticleSys = mSceneMgr->createParticleSystem("Fire", "Elements/Fire");
    mSceneMgr->getSceneNode("elementNode")->attachObject(mParticleSys);
    mParticleSys->setVisible(false);

    // Water
    mParticleSys = mSceneMgr->createParticleSystem("Water", "Elements/Water");
    mSceneMgr->getSceneNode("elementNode")->attachObject(mParticleSys);
    mParticleSys->setVisible(false);

    // Air
    mParticleSys = mSceneMgr->createParticleSystem("Air", "Elements/Air");
    mSceneMgr->getSceneNode("elementNode")->attachObject(mParticleSys);
    mParticleSys->setVisible(false);


    // Earth
    Ogre::SceneNode *earthNode = mSceneMgr->getSceneNode("elementNode")->createChildSceneNode("earthNode", Vector3::UNIT_Y * CHAR_HEIGHT);
    Ogre::Entity *earthEntity = mSceneMgr->createEntity("Earth", "stone.mesh");
    earthNode->setScale(.2, .2, .2);
    earthNode->setPosition(0, -10, 0);
    earthNode->attachObject(earthEntity);
    earthEntity->setVisible(false);
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
        if (evt.key == OIS::KC_ESCAPE) {
            if (!isUIvisible) {
                mTrayMgr->showAll();
                setMenuVisible("MainMenu");
                setMenuVisible("Option", false);
                setMenuVisible("OrenNayar", false);
                setMenuVisible("CookTorrance", false);
                mTrayMgr->showCursor();
                isUIvisible = true;
            } else {
                setMenuVisible("Option", false);
                setMenuVisible("MainMenu", false);
                mTrayMgr->hideCursor();
                isUIvisible = false;
            }
        }
        if (!isUIvisible) {
            mChara->injectKeyDown(evt);
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
    } else if (b->getName() == "mCreditButton") {
        mTrayMgr->showOkDialog("About", mInfo["About"]);
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
                                            Plane(Vector3::UNIT_Y, -18), 10000, 10000, 20, 20, true, 1, 128, 128, Vector3::UNIT_Z);


    // create a floor entity, give it a material, and place it at the origin
    floor = mSceneMgr->createEntity("Floor", "floor");
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

            mTrayMgr->moveWidgetToTray("mlightingModelLabel", TL_TOPLEFT);
            mTrayMgr->moveWidgetToTray("mCookTorranCB", TL_TOPLEFT);
            mTrayMgr->moveWidgetToTray("mOrrenNayarCB", TL_TOPLEFT);
            mTrayMgr->moveWidgetToTray("mElementLabel", TL_TOPLEFT);
            mTrayMgr->moveWidgetToTray("mELemenSelect", TL_TOPLEFT);
            mTrayMgr->getWidget("mlightingModelLabel")->show();
            mTrayMgr->getWidget("mCookTorranCB")->show();
            mTrayMgr->getWidget("mOrrenNayarCB")->show();
            mTrayMgr->getWidget("mElementLabel")->show();
            mTrayMgr->getWidget("mELemenSelect")->show();
        } else {
            mTrayMgr->removeWidgetFromTray("mlightingModelLabel");
            mTrayMgr->removeWidgetFromTray("mCookTorranCB");
            mTrayMgr->removeWidgetFromTray("mOrrenNayarCB");
            mTrayMgr->removeWidgetFromTray("mElementLabel");
            mTrayMgr->removeWidgetFromTray("mELemenSelect");
            mTrayMgr->getWidget("mlightingModelLabel")->hide();
            mTrayMgr->getWidget("mCookTorranCB")->hide();
            mTrayMgr->getWidget("mOrrenNayarCB")->hide();
            mTrayMgr->getWidget("mElementLabel")->hide();
            mTrayMgr->getWidget("mELemenSelect")->hide();
        }
    } else if (name == "OrenNayar") {
        if (visible) {
            mTrayMgr->moveWidgetToTray("mONSliderLabel", TL_TOPRIGHT);
            mTrayMgr->moveWidgetToTray("mONAlbedo", TL_TOPRIGHT);
            mTrayMgr->moveWidgetToTray("mONRoughness", TL_TOPRIGHT);
            mTrayMgr->getWidget("mONSliderLabel")->show();
            mTrayMgr->getWidget("mONAlbedo")->show();
            mTrayMgr->getWidget("mONRoughness")->show();
        } else {
            mTrayMgr->removeWidgetFromTray("mONSliderLabel");
            mTrayMgr->removeWidgetFromTray("mONAlbedo");
            mTrayMgr->removeWidgetFromTray("mONRoughness");
            mTrayMgr->getWidget("mONSliderLabel")->hide();
            mTrayMgr->getWidget("mONAlbedo")->hide();
            mTrayMgr->getWidget("mONRoughness")->hide();
        }
    } else if (name == "CookTorrance") {
        if (visible) {
            mTrayMgr->moveWidgetToTray("mCTSliderLabel", TL_TOPRIGHT);
            mTrayMgr->moveWidgetToTray("mCTFresnel", TL_TOPRIGHT);
            mTrayMgr->moveWidgetToTray("mCTRoughness", TL_TOPRIGHT);
            mTrayMgr->getWidget("mCTSliderLabel")->show();
            mTrayMgr->getWidget("mCTFresnel")->show();
            mTrayMgr->getWidget("mCTRoughness")->show();
        } else {
            mTrayMgr->removeWidgetFromTray("mCTSliderLabel");
            mTrayMgr->removeWidgetFromTray("mCTFresnel");
            mTrayMgr->removeWidgetFromTray("mCTRoughness");
            mTrayMgr->getWidget("mCTSliderLabel")->hide();
            mTrayMgr->getWidget("mCTFresnel")->hide();
            mTrayMgr->getWidget("mCTRoughness")->hide();
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

    mSceneMgr->clearScene(); // removes all nodes, billboards, lights etc.
    mSceneMgr->destroyAllCameras();
}

//-------------------------------------------------------------------------------------
void AwesomeParticles::setUniform(Ogre::String material, Ogre::String uniform, float value)
{
    (static_cast<MaterialPtr>(MaterialManager::getSingleton().getByName(material)))->getTechnique(0)->
    getPass(0)->getFragmentProgramParameters()->setNamedConstant(uniform, value);
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::sliderMoved(Slider *slider)
{
    // Oren Nayar
    if (slider->getName() == "mONAlbedo") {
        setUniform("Examples/BeachStones/OrenNayar", "albedo", slider->getValue());
    }
    if (slider->getName() == "mONRoughness") {
        setUniform("Examples/BeachStones/OrenNayar", "roughness", slider->getValue());
    }

    //Cook Torrance
    if (slider->getName() == "mCTFresnel") {
        setUniform("Examples/CloudySky/CookTorrance", "fresnel", slider->getValue());
    }
    if (slider->getName() == "mCTRoughness") {
        setUniform("Examples/CloudySky/CookTorrance", "roughness", slider->getValue());
    }
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
    const int WIDTH_UI = 160;

    mTrayMgr->createLabel(TL_NONE, "mlightingModelLabel", "Lighting Model", WIDTH_UI);
    mTrayMgr->createCheckBox(TL_NONE, "mCookTorranCB", "Cook Torrance", WIDTH_UI);
    mTrayMgr->createCheckBox(TL_NONE, "mOrrenNayarCB", "Orren Nayar", WIDTH_UI);

    const char *vecInit[] = {"Fire", "Water", "Air", "Earth"};
    StringVector vecElements(vecInit, vecInit + 4);
    mTrayMgr->createLabel(TL_NONE, "mElementLabel", "Elements", WIDTH_UI);
    mTrayMgr->createThickSelectMenu(TL_NONE, "mELemenSelect", "Select Element", WIDTH_UI, 4, vecElements);

    // main menu
    mTrayMgr->createLabel(TL_NONE, "mMainMenuLabel", "Main Menu", WIDTH_UI);
    mTrayMgr->createButton(TL_NONE, "mOptionButton", "Option");
    mTrayMgr->createButton(TL_NONE, "mCreditButton", "About");
    mTrayMgr->createButton(TL_NONE, "mQuitButton", "Quit");

    // advance lighting slider
    mTrayMgr->createLabel(TL_NONE, "mCTSliderLabel", "Cook Torrance Slider", 256);
    mTrayMgr->createThickSlider(TL_NONE, "mCTFresnel", "Fresnel Value", 256, 80, 0, 1, 100);
    mTrayMgr->createThickSlider(TL_NONE, "mCTRoughness", "Roughness Value", 256, 80, 0, 1, 100);

    mTrayMgr->createLabel(TL_NONE, "mONSliderLabel", "Oren Nayar Slider", 256);
    mTrayMgr->createThickSlider(TL_NONE, "mONAlbedo", "Albedo Value", 256, 80, 0, 1, 100);
    mTrayMgr->createThickSlider(TL_NONE, "mONRoughness", "Roughness Value", 256, 80, 0, 1, 100);
    mTrayMgr->hideAll();

}
//-------------------------------------------------------------------------------------
void AwesomeParticles::checkBoxToggled(CheckBox *box)
{
    if (box->getName() == "mCookTorranCB") {
        mCookTorran = box->isChecked();
        if (mCookTorran) {
            floor->setMaterialName("Examples/BeachStones/OrenNayar");
            setMenuVisible("CookTorrance");
        } else {
            floor->setMaterialName("Examples/BeachStones");
            setMenuVisible("CookTorrance", false);
        }
    } else if (box->getName() == "mOrrenNayarCB") {
        mOrrenNayar = box->isChecked();
        if (mOrrenNayar) {
            mSceneMgr->setSkyDome(true, "Examples/CloudySky/CookTorrance", 10, 8);
            setMenuVisible("OrenNayar");
        } else {
            mSceneMgr->setSkyDome(true, "Examples/CloudySky", 10, 8);
            setMenuVisible("OrenNayar", false);
        }
    }
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::itemSelected(SelectMenu *menu)
{
    Ogre::String currentElement = menu->getSelectedItem();
    mChara->setCurrentElement(currentElement);

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