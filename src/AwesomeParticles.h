/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.h
-----------------------------------------------------------------------------
This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#ifndef __TutorialApplication_h_
#define __TutorialApplication_h_

#include "BaseApplication.h"
#include "SdkTrays.h"

class AwesomeParticles : public BaseApplication
{
public:
    AwesomeParticles(void);
    virtual ~AwesomeParticles(void);

protected:
    virtual void createScene(void);

	// setting up the scene
	virtual void createCamera();
	virtual void createViewports();
	void setupParticles();
};

#endif // #ifndef __TutorialApplication_h_