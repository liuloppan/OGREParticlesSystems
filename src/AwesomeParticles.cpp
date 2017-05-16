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
AwesomeParticles::AwesomeParticles():
    waterMesh(0)
{
}
//-------------------------------------------------------------------------------------
AwesomeParticles::~AwesomeParticles()
{
}

void AwesomeParticles::processParticles()
{
    static int pindex = 0 ;
    ParticleIterator pit = ps->_getIterator() ;
    while (!pit.end()) {
        Particle *particle = pit.getNext();
        Vector3 ppos = particle->position;
        if (ppos.y <= 0 && particle->timeToLive > 0) { // hits the water!
            // delete particle
            particle->timeToLive = 0.0f;
            // push the water
            float x = ppos.x / PLANE_SIZE * COMPLEXITY ;
            float y = ppos.z / PLANE_SIZE * COMPLEXITY ;
            float h = rand() % RAIN_HEIGHT_RANDOM + RAIN_HEIGHT_CONSTANT ;
            if (x < 1) {
                x = 1 ;
            }
            if (x > COMPLEXITY - 1) {
                x = COMPLEXITY - 1;
            }
            if (y < 1) {
                y = 1 ;
            }
            if (y > COMPLEXITY - 1) {
                y = COMPLEXITY - 1;
            }
            waterMesh->push(x, y, -h) ;
            WaterCircle *circle = new WaterCircle(mSceneMgr,
                                                  "Circle#" + StringConverter::toString(pindex++),
                                                  x, y);
            circles.push_back(circle);
        }
    }
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::setupParticles()
{
    ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);
    //ParticleSystem *ps;

    // Fire
    ps = mSceneMgr->createParticleSystem("Fire", "Elements/Fire");
    mSceneMgr->getRootSceneNode()->attachObject(ps);
    ps->setVisible(true);

    //// Water
    ps = mSceneMgr->createParticleSystem("Water", "Elements/Water");
    mSceneMgr->getRootSceneNode()->attachObject(ps);
    ps->setVisible(false);

    // Air
    ps = mSceneMgr->createParticleSystem("Air", "Elements/Air");
    mSceneMgr->getRootSceneNode()->attachObject(ps);
    ps->setVisible(false);


    // Earth
    ps = mSceneMgr->createParticleSystem("Earth", "Elements/Earth");
    mSceneMgr->getRootSceneNode()->attachObject(ps);
    ps->setVisible(false);

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
    Light *l = mSceneMgr->createLight("MainLight");
    l->setPosition(20, 80, 50);
    // disable default camera control so the character can do its own
    mCameraMan->setStyle(CS_MANUAL);
	 // create a floor mesh resource
    MeshManager::getSingleton().createPlane("floor", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                            Plane(Vector3::UNIT_Y, -30), 3000, 3000, 10, 10, true, 1, 8, 8, Vector3::UNIT_Z);

    // create a floor entity, give it a material, and place it at the origin
    Entity *floor = mSceneMgr->createEntity("Floor", "floor");
    floor->setMaterialName("Environments/BeachStones");
    mSceneMgr->getRootSceneNode()->attachObject(floor);
    mChara = new SinbadCharacterController(mCamera);
    mSceneMgr->setSkyDome(true, "Environments/CloudySky", 10, 8);
    setupParticles();
    // Create water mesh and entity
    waterMesh = new WaterMesh(MESH_NAME, PLANE_SIZE, COMPLEXITY);
    waterEntity = mSceneMgr->createEntity(ENTITY_NAME,
                                          MESH_NAME);
    waterEntity->setMaterialName(MATERIAL_NAME);
    SceneNode *waterNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    waterNode->attachObject(waterEntity);
    // set up spline animation of light node
    Animation *anim = mSceneMgr->createAnimation("WaterLight", 20);
    NodeAnimationTrack *track ;
    TransformKeyFrame *key ;
    // Create light node
    SceneNode *lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    lightNode->attachObject(l);
    // create a random spline for light
    track = anim->createNodeTrack(0, lightNode);
    key = track->createNodeKeyFrame(0);
    for (int ff = 1; ff <= 19; ff++) {
        key = track->createNodeKeyFrame(ff);
        Vector3 lpos(
            rand() % (int)PLANE_SIZE, //- PLANE_SIZE/2,
            rand() % 300 + 100,
            rand() % (int)PLANE_SIZE //- PLANE_SIZE/2
        );
        key->setTranslate(lpos);
    }
    key = track->createNodeKeyFrame(20);

    // Create a new animation state to track this
    mAnimState = mSceneMgr->createAnimationState("WaterLight");
    mAnimState->setEnabled(true);

    // Put in a bit of fog for the hell of it
    //mSceneMgr->setFog(FOG_EXP, ColourValue::White, 0.0002);

    // Let there be rain
    ps = mSceneMgr->createParticleSystem("rain",
                                         "Elements/Water/Rain");
    particleEmitter = ps->getEmitter(0);
    SceneNode *rNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    rNode->translate(PLANE_SIZE / 2.0f, 30000, PLANE_SIZE / 2.0f);
    rNode->attachObject(ps);
    // Fast-forward the rain so it looks more natural
    ps->fastForward(20);
    // It can't be set in .particle file, and we need it ;)
    static_cast<BillboardParticleRenderer *>(ps->getRenderer())->setBillboardOrigin(BBO_BOTTOM_CENTER);

    prepareCircleMaterial();
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
    delete waterMesh;
    waterMesh = 0;
}
//-------------------------------------------------------------------------------------
bool AwesomeParticles::frameRenderingQueued(const FrameEvent &fe)
{
    mChara->addTime(fe.timeSinceLastFrame);
    mTrayMgr->frameRenderingQueued(fe);
    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
    if (mKeyboard->isKeyDown(OIS::KC_SPACE)) {
        particleEmitter->setEmissionRate(20.0f);
    } else {
        particleEmitter->setEmissionRate(0.0f);
    }
    processParticles();
    timeoutDelay -= fe.timeSinceLastFrame ;
    if (timeoutDelay <= 0) {
        timeoutDelay = 0;
    }

    waterMesh->updateMesh(fe.timeSinceLastFrame);
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
    if (box->getName() == "mCookTorranCB") {
        mCookTorran = box->isChecked();
    } else if (box->getName() == "mOrrenNayarCB") {
        mOrrenNayar = box->isChecked();
    }
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::itemSelected(SelectMenu *menu)
{

    //Ogre::StringVector myItems = menu->getItems();
    Ogre::String currentElement = menu->getSelectedItem();

    for (int i = 0 ; i <= 3; i++) {
        if (currentElement == menu->getItems()[i]) {
            mSceneMgr->getParticleSystem(menu->getItems()[i])->setVisible(true);
        } else {
            mSceneMgr->getParticleSystem(menu->getItems()[i])->setVisible(false);
        }
    }
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::prepareCircleMaterial()
{
    char *bmap = new char[256 * 256 * 4] ;
    memset(bmap, 127, 256 * 256 * 4);
    for (int b = 0; b < 16; b++) {
        int x0 = b % 4 ;
        int y0 = b >> 2 ;
        Real radius = 4.0f + 1.4 * (float) b ;
        for (int x = 0; x < 64; x++) {
            for (int y = 0; y < 64; y++) {
                Real dist = Math::Sqrt((x - 32) * (x - 32) + (y - 32) * (y - 32)); // 0..ca.45
                dist = fabs(dist - radius - 2) / 2.0f ;
                dist = dist * 255.0f;
                if (dist > 255) {
                    dist = 255 ;
                }
                int colour = 255 - (int)dist ;
                colour = (int)(((Real)(15 - b)) / 15.0f * (Real) colour);

                bmap[4 * (256 * (y + 64 * y0) + x + 64 * x0) + 0] = colour ;
                bmap[4 * (256 * (y + 64 * y0) + x + 64 * x0) + 1] = colour ;
                bmap[4 * (256 * (y + 64 * y0) + x + 64 * x0) + 2] = colour ;
                bmap[4 * (256 * (y + 64 * y0) + x + 64 * x0) + 3] = colour ;
            }
        }
    }

    DataStreamPtr imgstream(new MemoryDataStream(bmap, 256 * 256 * 4));
    //~ Image img;
    //~ img.loadRawData( imgstream, 256, 256, PF_A8R8G8B8 );
    //~ TextureManager::getSingleton().loadImage( CIRCLES_MATERIAL , img );
    TextureManager::getSingleton().loadRawData(CIRCLES_MATERIAL,
            ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
            imgstream, 256, 256, PF_A8R8G8B8);
    MaterialPtr material =
        MaterialManager::getSingleton().create(CIRCLES_MATERIAL,
                ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    TextureUnitState *texLayer = material->getTechnique(0)->getPass(0)->createTextureUnitState(CIRCLES_MATERIAL);
    texLayer->setTextureAddressingMode(TextureUnitState::TAM_CLAMP);
    material->setSceneBlending(SBT_ADD);
    material->setDepthWriteEnabled(false) ;
    material->load();
    // finished with bmap so release the memory
    delete [] bmap;
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