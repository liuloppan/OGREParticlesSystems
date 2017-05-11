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
    mHeroEntity(NULL),
    mHeroNode(NULL),
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
//-------------------------------------------------------------------------------------
void AwesomeParticles::setupMainChar()
{
    // sinbad character
    mHeroEntity = mSceneMgr->createEntity("Sinbad", "Sinbad.mesh");
    mHeroEntity->setCastShadows(true);
    Ogre::Entity *sinbadAttack = mSceneMgr->createEntity("SinbadAttack", "Sword.mesh");
    mHeroEntity->attachObjectToBone("Handle.R", sinbadAttack);
    mHeroNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HeroNode", Ogre::Vector3(0, 0, 65));
    mHeroNode->attachObject(mHeroEntity);
    mHeroNode->scale(Ogre::Vector3(20, 20, 20));
    mHeroNode->translate(0, 0, -200);
    // Set cumulative blending mode
    mHeroEntity->getSkeleton()->setBlendMode(Ogre::ANIMBLEND_CUMULATIVE);

    // Set default key-frame interpolation mode
    Ogre::Animation::setDefaultInterpolationMode(Ogre::Animation::IM_SPLINE);
    Ogre::Animation::setDefaultRotationInterpolationMode(Ogre::Animation::RIM_SPHERICAL);

    // Set animation state properties ("IdleBase")
    mIdleBase = mHeroEntity->getAnimationState("IdleBase");
    mIdleBase->setLoop(true);
    mIdleBase->setEnabled(false);

    // Set animation state properties ("IdleTop")
    mIdleTop = mHeroEntity->getAnimationState("IdleBase");
    mIdleTop->setLoop(true);
    mIdleTop->setEnabled(false);

    // Set animation state properties ("RunBase")
    mRunBaseState = mHeroEntity->getAnimationState("RunBase");
    mRunBaseState->setLoop(true);
    mRunBaseState->setEnabled(false);

    // Set animation state properties ("RunTop")
    mRunTopState = mHeroEntity->getAnimationState("RunTop");
    mRunTopState->setLoop(true);
    mRunTopState->setEnabled(false);

    // Set animation state properties ("DrawSwords")
    mAttackState = mHeroEntity->getAnimationState("DrawSwords");
    mAttackState->setLoop(false);
    mAttackState->setEnabled(false);
}
//-------------------------------------------------------------------------------------
bool AwesomeParticles::frameStarted(const Ogre::FrameEvent &evt)
{
    // Check keyboard to determine running mode
    bool bRunning = false;
    if (mKeyboard->isKeyDown(OIS::KC_D)) {
        // Turn left and run
        bRunning = true;
        mHeroNode->translate(Ogre::Vector3(-1.0f, 0.0f, 0.0f) * evt.timeSinceLastFrame);
        mHeroNode->resetOrientation();
        mHeroNode->yaw(Ogre::Radian(-Ogre::Math::HALF_PI));
    } else if (mKeyboard->isKeyDown(OIS::KC_A)) {
        // Turn right and run
        bRunning = true;
        mHeroNode->translate(Ogre::Vector3(1.0f, 0.0f, 0.0f) * evt.timeSinceLastFrame);
        mHeroNode->resetOrientation();
        mHeroNode->yaw(Ogre::Radian(Ogre::Math::HALF_PI));

    } else if (mKeyboard->isKeyDown(OIS::KC_S)) {
        // turn back and run
        bRunning = true;
        mHeroNode->resetOrientation();
        mHeroNode->rotate(Ogre::Vector3(0, -1, 0), Ogre::Degree(180));
    } else if (mKeyboard->isKeyDown(OIS::KC_W)) {
        // turn front and run
        bRunning = true;
        mHeroNode->resetOrientation();
    }

    if (bRunning) {
        // Advance the animation
        mRunBaseState->setEnabled(true);
        mRunBaseState->addTime(evt.timeSinceLastFrame);

        mRunTopState->setEnabled(true);
        mRunTopState->addTime(evt.timeSinceLastFrame);
    } else {
        // Reset node orientation and time position
        mHeroNode->resetOrientation();

        mRunBaseState->setEnabled(false);
        mRunBaseState->setTimePosition(0.0f);

        mRunTopState->setEnabled(false);
        mRunTopState->setTimePosition(0.0f);

        // idle state
        mIdleBase->setEnabled(true);
        mIdleBase->addTime(evt.timeSinceLastFrame);
        mIdleTop->setEnabled(true);
        mIdleTop->addTime(evt.timeSinceLastFrame);
    }

    if (mAttackState->getEnabled()) {
        if (mAttackState->hasEnded()) {
            mAttackState->setEnabled(false);
            mAttackState->setTimePosition(0.0f);
        }
        mAttackState->addTime(evt.timeSinceLastFrame);
    } else if (mKeyboard->isKeyDown(OIS::KC_SPACE)) {
        mAttackState->setEnabled(true);
        mAttackState->addTime(evt.timeSinceLastFrame);
    }

    if (mKeyboard->isKeyDown(OIS::KC_ESCAPE)) {
        mTrayMgr->createCheckBox(OgreBites::TL_CENTER, "Manual", "Manual Animation")->setChecked(false);
    }

    return true;
}

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
    // set ambient light : red-green-blue
    mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 0.5));

    setupMainChar();

    // Set camera style
    mCameraMan->setStyle(OgreBites::CS_ORBIT);
    mCameraMan->setYawPitchDist(Ogre::Radian(0), Ogre::Radian(15), 400);

    // light
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
}

//-------------------------------------------------------------------------------------
void AwesomeParticles::destroyScene()
{

}

bool AwesomeParticles::frameRenderingQueued(const Ogre::FrameEvent &fe)
{
    bool ret = BaseApplication::frameRenderingQueued(fe);
    return ret;
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