#pragma once

#include <string>
#include <imgui.h>

#include "InputCommon/ImageOperations.h"
#include "Core/IOS/USB/Emulated/Skylander.h"

using Element = IOS::HLE::USB::Element;

class SkylanderPortal
{
  public:
    struct Skylander
    {
      std::string name;
      int element;
    };

    static SkylanderPortal& GetInstance()
    {
      static SkylanderPortal instance;
      return instance;
    }

    void actived() { active = true; }
    void deactived() { active = false; }
    bool IsActive() { return active; }

    void set(std::list<Skylander>);
    std::string GetCurrent();

    void show();

  private:
    bool active = false;

    std::list<Skylander> skylanders;
    std::list<Skylander>::iterator sky_it;

    void drawWindow(std::string win_name, std::list<SkylanderPortal::Skylander>::iterator sky,
                    int x_off = 0, float f_scale = 6,
                    float alpha = 1);
    ImVec4 getColorFromElement(int, float = 1);
    bool IsPressed(int, bool = false);
};
