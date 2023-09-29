#include "DolphinQt/SkylanderPortal/SkylanderPortal.h"

#include "Core/HotkeyManager.h"

void SkylanderPortal::set(std::list<Skylander> skys)
{
  skylanders = skys;
  sky_it = skylanders.begin();
}
std::string SkylanderPortal::GetCurrent()
{
  active = false;
  if(sky_it != skylanders.begin()) skylanders.splice(skylanders.begin(), skylanders, sky_it, std::next(sky_it));
  sky_it = skylanders.begin();
  return sky_it->name;
}

void SkylanderPortal::show()
{
  if (!active) return;

  if (IsPressed(HK_SEL_SKYLANDER_R))
  {
    if (std::next(sky_it) != skylanders.end())
      sky_it++;
  }
  else if (IsPressed(HK_SEL_SKYLANDER_L))
  {
    if (sky_it != skylanders.begin())
      sky_it--;
  }

  drawWindow("Skylander", sky_it);

  if (sky_it != skylanders.begin())
    drawWindow("Previous Skylander", std::prev(sky_it), -550, 5, 0.5f);

  auto next = std::next(sky_it);
  if (next != skylanders.end())
    drawWindow("Next Skylander", next, 550, 5, 0.5f);
}

void SkylanderPortal::drawWindow(std::string win_name, std::list<SkylanderPortal::Skylander>::iterator sky, int x_off, float f_scale, float alpha)
{
  const auto imgui_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs |
                           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings |
                           ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoNav |
                           ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing;
  auto ptext = sky->name;
  int width = ptext.length() * f_scale * 7.5f;

  float old_size = ImGui::GetFont()->Scale;
  ImGui::GetFont()->Scale = f_scale;
  ImGui::PushFont(ImGui::GetFont());

  ImGuiIO& io = ImGui::GetIO();
  ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f + x_off, io.DisplaySize.y * 0.5f),
                          ImGuiCond_Always, ImVec2(0.5f, 0.5f));
  ImGui::SetNextWindowSize(ImVec2(width, f_scale * 16));
  ImGui::SetNextWindowBgAlpha(alpha - 0.3f);

  if (ImGui::Begin(win_name.c_str(), nullptr, imgui_flags))
  {
    ImGui::TextColored(getColorFromElement(sky->element, alpha), "%s", ptext.c_str());
    ImGui::End();
  }
  ImGui::GetFont()->Scale = old_size;
  ImGui::PopFont();
}

ImVec4 SkylanderPortal::getColorFromElement(int e, float a)
{
  int r = 255, g = 255, b = 255;
  if (e == (int)Element::Magic)
  {
    r = 168;
    g = 53;
    b = 217;
  }
  else if (e == (int)Element::Tech)
  {
    r = 203;
    g = 132;
    b = 78;
  }
  else if (e == (int)Element::Water)
  {
    r = 84;
    g = 173;
    b = 186;
  }
  else if (e == (int)Element::Fire)
  {
    r = 219;
    g = 67;
    b = 80;
  }
  else if (e == (int)Element::Life)
  {
    r = 88;
    g = 179;
    b = 82;
  }
  else if (e == (int)Element::Undead)
  {
    r = 127;
    g = 109;
    b = 94;
  }
  else if (e == (int)Element::Earth)
  {
    r = 152;
    g = 106;
    b = 65;
  }
  else if (e == (int)Element::Air)
  {
    r = 78;
    g = 149;
    b = 202;
  }

  return ImVec4((float)r / 255, (float)g / 255, (float)b / 255, a);
}

bool SkylanderPortal::IsPressed(int id, bool held)
{
  return HotkeyManagerEmu::IsPressed(id, held);
}
