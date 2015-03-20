/******************************************************************************/
/*!
\file   TemplateSystem.h
\author DarthVader
\par    Course: GAM200
\par    All content 2014 DigiPen (USA) Corporation, all rights reserved.
\brief  
*/
/******************************************************************************/

#ifndef _TEMPLATE_SYS_H
#define _TEMPLATE_SYS_H

#include "BaseSystem.h"

namespace Framework
{

  class TemplateSystem : public BaseSystem
  {
  public:
  TemplateSystem ();
  ~TemplateSystem ();
  
  virtual bool Initialize ();
  virtual void Update(const double dt);
  virtual const std::string GetName() { return "TemplateSystem"; }
  
  private:

  };  
}

#endif