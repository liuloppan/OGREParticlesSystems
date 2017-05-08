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
    virtual bool frameStarted(const Ogre::FrameEvent &evt);

    // setting up the scene
    virtual void createCamera();
    virtual void createViewports();
    virtual void checkBoxToggled(OgreBites::CheckBox *box);
    virtual void itemSelected(OgreBites::SelectMenu *menu);
    virtual bool setup();
private:
    void setupToggles();
    void setupParticles();
    void setupMainChar();

    OgreBites::Label *mInfoLabel;;
    bool mMenuName, mCookTorren, mTorrenNayar;
    OgreBites::CheckBox *mCookTorrenCB;
    OgreBites::CheckBox *mTorrenNayarCB;
    OgreBites::SelectMenu *mElementMenu;
    Ogre::Entity *mHeroEntity;
    Ogre::SceneNode *mHeroNode;
    Ogre::AnimationState *mIdleBase;
    Ogre::AnimationState *mIdleTop;
    Ogre::AnimationState *mRunBaseState;
    Ogre::AnimationState *mRunTopState;
    Ogre::AnimationState *mAttackState;
};

#endif // #ifndef __AwesomeParticles_h_