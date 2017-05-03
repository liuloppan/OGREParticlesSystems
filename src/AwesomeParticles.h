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

	virtual bool setup();
	void setupToggles();

	void setupParticles();
};

#endif // #ifndef __AwesomeParticles_h_