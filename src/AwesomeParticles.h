/*
-----------------------------------------------------------------------------
Filename:    AwesomeParticles.h
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
#ifndef __AwesomeParticles_h_
#define __AwesomeParticles_h_

#include "BaseApplication.h"
#include "sdkTrays.h"

class AwesomeParticles : public BaseApplication
{
public:
    AwesomeParticles();
    virtual ~AwesomeParticles();

protected:
    virtual void createScene();
    virtual void createFrameListener();
    virtual void destroyScene();
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &fe);

    // setting up the scene
    virtual void createCamera();
    virtual void createViewports();
    virtual void checkBoxToggled(OgreBites::CheckBox *box);
    virtual void itemSelected(OgreBites::SelectMenu *menu);
    virtual bool setup();
private:
    void setupToggles();
    void setupParticles();

    OgreBites::Label *mInfoLabel;
    OgreBites::CheckBox *mNinjaCB;
    bool mNinja, mSinbad, mMenuName;
    OgreBites::CheckBox *mSinbadCB;
    Ogre::Entity *ninjaEntity;
    Ogre::Entity *sinbadEntity;
    OgreBites::CheckBox *mCookTorrenCB;
    OgreBites::CheckBox *mTorrenNayarCB;
    OgreBites::SelectMenu *mLightingMenu;
};

#endif // #ifndef __AwesomeParticles_h_