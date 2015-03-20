/******************************************************************************/
/*!
\file   Scene_.h
\author DarthVader
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _SCENE__H
#define _SCENE__H

#include "Scene.h"

namespace Framework
{

  class Scene_ : public Scene
  {
  public:
  Scene_ ();
  ~Scene_ ();

  private:
    virtual void Key_Pressed (int key, int state, int action, int mods) override;
    virtual void Mouse_Button (int button, int state, int mods) override;
    virtual void Mouse_Moved (double xPos, double yPos) override;
    virtual void Update (const double dt) override;
    virtual void Draw () override;
    virtual void Load_Scene (const char* filename) override;
    virtual void Change_Size (int x, int y) override;
  }; 
}

#endif