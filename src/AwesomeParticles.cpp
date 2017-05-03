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
#include "sdkTrays.h"


using namespace Ogre;
using namespace OgreBites;
//-------------------------------------------------------------------------------------
AwesomeParticles::AwesomeParticles(void)
{
}
//-------------------------------------------------------------------------------------
AwesomeParticles::~AwesomeParticles(void)
{
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::setupParticles()	
{
	Ogre::ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);
	Ogre::ParticleSystem* ps;

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
	mCamera->setNearClipDistance(5);

	// create new sdk camera man
	mCameraMan = new OgreBites::SdkCameraMan(mCamera);
}
//-------------------------------------------------------------------------------------
void AwesomeParticles::createViewports()
{
	// add new viewport
	Ogre::Viewport* vp = mWindow->addViewport(mCamera);

	// set the background colour
	vp->setBackgroundColour(Ogre::ColourValue(0, 0, 0));

	// set aspect ratio
	mCamera->setAspectRatio(
		Ogre::Real(vp->getActualWidth()) /
		Ogre::Real(vp->getActualHeight()));
}
bool AwesomeParticles::setup(void){

	if(!BaseApplication::setup())
		return false;

		//GUI
	//mTrayMgr = new SdkTrayManager("InterfaceName", mWindow, mMouse, this);
	mTrayMgr->showCursor();
	setupToggles(); 

}
//-------------------------------------------------------------------------------------
void AwesomeParticles::createScene(void)
{
	// set ambient light : red-green-blue
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 0.5));

	// create an Entity
	Ogre::Entity* ninjaEntity = mSceneMgr->createEntity("ninja.mesh");
	ninjaEntity->setCastShadows(true);

	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ninjaEntity);

	// create plane object
	Ogre::Plane plane(Ogre::Vector3::UNIT_Y, 0);
	mSceneMgr->setSkyBox( true, "Examples/SpaceSkyBox" );

	// create mesh into our plane
	Ogre::MeshManager::getSingleton().createPlane(
	"ground",
	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	plane, 
	1500, 1500, 20, 20, 
	true, 
	1, 5, 5, 
	Ogre::Vector3::UNIT_Z);

	Ogre::Entity* groundEntity = mSceneMgr->createEntity("ground");
	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);

	groundEntity->setCastShadows(false);

	groundEntity->setMaterialName("Examples/Rockwall");

	mSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));
	
	mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

	Ogre::Light* spotLight = mSceneMgr->createLight("SpotLight");
	spotLight->setDiffuseColour(0, 0, 1.0);
	spotLight->setSpecularColour(0, 0, 1.0);
	spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
	spotLight->setDirection(-1, -1, 0);
	spotLight->setPosition(Ogre::Vector3(200, 200, 0));
	spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));

	Ogre::Light* directionalLight = mSceneMgr->createLight("DirectionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(Ogre::ColourValue(.4, 0, 0));
	directionalLight->setSpecularColour(Ogre::ColourValue(.4, 0, 0));
	directionalLight->setDirection(Ogre::Vector3(0, -1, 1));

	setupParticles();
}

//-------------------------------------------------------------------------------------
void AwesomeParticles::setupToggles()
{
		// create check boxes to toggle the visibility of our particle systems
		const int WIDTH_UI = 130;
		const char *vecInit[] = {"fire", "earth", "water", "air"};
		Ogre::StringVector vecElements(vecInit,vecInit+4);

		mTrayMgr->createLabel(TL_TOPLEFT, "VisLabel", "Lighting Model");
		mTrayMgr->createCheckBox(TL_TOPLEFT, "Fireworks", "Fireworks", WIDTH_UI)->setChecked(true);
		mTrayMgr->createCheckBox(TL_TOPLEFT, "Rain", "Rain", WIDTH_UI)->setChecked(false);

		mTrayMgr->createLabel(TL_TOPLEFT, "ElemLabel", "Elements");
		mTrayMgr->createThickSelectMenu(TL_TOPLEFT, "ElementMenu", "ElemLabel", WIDTH_UI, 4,vecElements);

}



#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        AwesomeParticles app;

        try {
            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
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