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
AwesomeParticles::AwesomeParticles():
	mTerrainGroup(0),
	mTerrainGlobals(0),
	mInfoLabel(0)
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
	mCamera->setNearClipDistance(.1);

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
void AwesomeParticles::createScene()
{
	// set ambient light : red-green-blue
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1, 1, 0.5));

	// create an Entity
	Ogre::Entity* ninjaEntity = mSceneMgr->createEntity("ninja.mesh");
	ninjaEntity->setCastShadows(true);

	mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(ninjaEntity);

	// light 
	Ogre::Light* spotLight = mSceneMgr->createLight("SpotLight");
	spotLight->setDiffuseColour(0, 0, 1.0);
	spotLight->setSpecularColour(0, 0, 1.0);
	spotLight->setType(Ogre::Light::LT_SPOTLIGHT);
	spotLight->setDirection(-1, -1, 0);
	spotLight->setPosition(Ogre::Vector3(200, 200, 0));
	spotLight->setSpotlightRange(Ogre::Degree(35), Ogre::Degree(50));

	Ogre::Light* directionalLight = mSceneMgr->createLight("DirectionalLight");
	directionalLight->setType(Ogre::Light::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(Ogre::ColourValue(1, 1, 1));
	directionalLight->setSpecularColour(Ogre::ColourValue(.5, 0, 0));
	directionalLight->setDirection(Ogre::Vector3(0, -1, 1));

	// set far plane
	bool infiniteClip =
	mRoot->getRenderSystem()->getCapabilities()->hasCapability(
	Ogre::RSC_INFINITE_FAR_PLANE);
 
	if (infiniteClip)
		mCamera->setFarClipDistance(0);
	else
		mCamera->setFarClipDistance(50000);

	// Fog
	//Ogre::ColourValue fadeColour(.9, .9, .9);
	//mWindow->getViewport(0)->setBackgroundColour(fadeColour);
 
	//mSceneMgr->setFog(Ogre::FOG_EXP2, fadeColour, 0.002);

	// Terrain
	mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
 
	mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(
	mSceneMgr,
	Ogre::Terrain::ALIGN_X_Z,
	513, 12000.0);
	mTerrainGroup->setFilenameConvention(Ogre::String("terrain"), Ogre::String("dat"));
	mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
 
	configureTerrainDefaults(directionalLight);
 
	for (long x = 0; x <= 0; ++x)
		for (long y = 0; y <= 0; ++y)
			defineTerrain(x, y);
 
	mTerrainGroup->loadAllTerrains(true);
 
	if (mTerrainsImported)
	{
		Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
 
		while (ti.hasMoreElements())
		{
			Ogre::Terrain* t = ti.getNext()->instance;
			initBlendMaps(t);
		}
	}

	mTerrainGroup->freeTemporaryResources();

	mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

	//setupParticles();
}

//-------------------------------------------------------------------------------------
void AwesomeParticles::createFrameListener()
{
  BaseApplication::createFrameListener();
 
  mInfoLabel = mTrayMgr->createLabel(OgreBites::TL_TOP, "TerrainInfo", "", 350);
}
 
//-------------------------------------------------------------------------------------
void AwesomeParticles::destroyScene()
{
  OGRE_DELETE mTerrainGroup;
  OGRE_DELETE mTerrainGlobals;
}

bool AwesomeParticles::frameRenderingQueued(const Ogre::FrameEvent& fe)
{
  bool ret = BaseApplication::frameRenderingQueued(fe);
 
  if (mTerrainGroup->isDerivedDataUpdateInProgress())
  {
    mTrayMgr->moveWidgetToTray(mInfoLabel, OgreBites::TL_TOP, 0);
    mInfoLabel->show();
 
    if (mTerrainsImported)
      mInfoLabel->setCaption("Building terrain...");
    else
      mInfoLabel->setCaption("Updating textures...");
  }
  else
  {
    mTrayMgr->removeWidgetFromTray(mInfoLabel);
    mInfoLabel->hide();
 
    if (mTerrainsImported)
    {
      mTerrainGroup->saveAllTerrains(true);
      mTerrainsImported = false;
    }
  }
 
  return ret;
}
 
void getTerrainImage(bool flipX, bool flipY, Ogre::Image& img)
{
  img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
 
  if (flipX)
    img.flipAroundY();
  if (flipY)
    img.flipAroundX();
 
}
 
void AwesomeParticles::defineTerrain(long x, long y)
{
  Ogre::String filename = mTerrainGroup->generateFilename(x, y);
 
  bool exists =
    Ogre::ResourceGroupManager::getSingleton().resourceExists(
      mTerrainGroup->getResourceGroup(),
      filename);
 
  if (exists)
    mTerrainGroup->defineTerrain(x, y);
  else
  {
    Ogre::Image img;
    getTerrainImage(x % 2 != 0, y % 2 != 0, img);
    mTerrainGroup->defineTerrain(x, y, &img);
 
    mTerrainsImported = true;
  }
}
 
void AwesomeParticles::initBlendMaps(Ogre::Terrain* terrain)
{
  Ogre::Real minHeight0 = 70;
  Ogre::Real fadeDist0 = 40;
  Ogre::Real minHeight1 = 70;
  Ogre::Real fadeDist1 = 15;
 
  Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
  Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
 
  float* pBlend0 = blendMap0->getBlendPointer();
  float* pBlend1 = blendMap1->getBlendPointer();
 
  for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
  {
    for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
    {
      Ogre::Real tx, ty;
 
      blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
      Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
      Ogre::Real val = (height - minHeight0) / fadeDist0;
      val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
      *pBlend0++ = val;
 
      val = (height - minHeight1) / fadeDist1;
      val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
      *pBlend1++ = val;
    }
  }
 
  blendMap0->dirty();
  blendMap1->dirty();
  blendMap0->update();
  blendMap1->update();
 
}
 
void AwesomeParticles::configureTerrainDefaults(Ogre::Light* light)
{
  mTerrainGlobals->setMaxPixelError(8);
  mTerrainGlobals->setCompositeMapDistance(3000);
 
  mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
  mTerrainGlobals->setCompositeMapAmbient(mSceneMgr->getAmbientLight());
  mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
 
  Ogre::Terrain::ImportData& importData = mTerrainGroup->getDefaultImportSettings();
  importData.terrainSize = 513;
  importData.worldSize = 12000.0;
  importData.inputScale = 600;
  importData.minBatchSize = 33;
  importData.maxBatchSize = 65;
 
  importData.layerList.resize(3);
  importData.layerList[0].worldSize = 100;
  importData.layerList[0].textureNames.push_back(
    "dirt_grayrocky_diffusespecular.dds");
  importData.layerList[0].textureNames.push_back(
    "dirt_grayrocky_normalheight.dds");
  importData.layerList[1].worldSize = 30;
  importData.layerList[1].textureNames.push_back(
    "grass_green-01_diffusespecular.dds");
  importData.layerList[1].textureNames.push_back(
    "grass_green-01_normalheight.dds");
  importData.layerList[2].worldSize = 200;
  importData.layerList[2].textureNames.push_back(
    "growth_weirdfungus-03_diffusespecular.dds");
  importData.layerList[2].textureNames.push_back(
    "growth_weirdfungus-03_normalheight.dds");
 
}

//-------------------------------------------------------------------------------------
void AwesomeParticles::setupToggles()
{
	// create check boxes to toggle the visibility of our particle systems
	const int WIDTH_UI = 130;
	const char *vecInit[] = {"Fire", "Earth", "Water", "Air"};
	Ogre::StringVector vecElements(vecInit,vecInit+4);

	mTrayMgr->createLabel(TL_TOPLEFT, "VisLabel", "Lighting Model");
	mTrayMgr->createCheckBox(TL_TOPLEFT, "Fireworks", "Fireworks", WIDTH_UI)->setChecked(true);
	mTrayMgr->createCheckBox(TL_TOPLEFT, "Rain", "Rain", WIDTH_UI)->setChecked(false);

	mTrayMgr->createLabel(TL_TOPLEFT, "ElemLabel", "Elements");
	mTrayMgr->createThickSelectMenu(TL_TOPLEFT, "ElementMenu", "Select Element", WIDTH_UI, 4,vecElements);

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