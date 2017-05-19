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
#include "SinbadCharacterController.h"
#include "sdkTrays.h"

using namespace Ogre;
using namespace OgreBites;

class AwesomeParticles : public BaseApplication
{
public:
    AwesomeParticles();
    virtual ~AwesomeParticles();

protected:
    virtual void createScene();
    virtual void destroyScene();
    virtual bool frameRenderingQueued(const FrameEvent &fe);
    virtual bool keyPressed(const OIS::KeyEvent &evt);
    virtual bool keyReleased(const OIS::KeyEvent &evt);
    virtual bool mouseMoved(const OIS::MouseEvent &evt);
    virtual bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    virtual void buttonHit(Button *b);

    // setting up the scene
    virtual void checkBoxToggled(CheckBox *box);
    virtual void itemSelected(SelectMenu *menu);
    virtual bool setup();
    virtual void setupWidgets();
private:
    void setupToggles();
    void setupParticles();
    void setMenuVisible(const String &name, bool visible = true);

    bool mMenuName, mCookTorran, mOrrenNayar;
    Entity *floor;
    SinbadCharacterController *mChara;
    ParticleSystem *mParticleSys;
    SceneNode *mElementNode;
    Ogre::NameValuePairList mInfo;
};

#endif // #ifndef __AwesomeParticles_h_