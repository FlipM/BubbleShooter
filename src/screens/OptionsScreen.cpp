// screens/OptionsScreen.cpp
#include "OptionsScreen.hpp"
#include "core/Renderer.hpp"

namespace screens {

OptionsScreen::OptionsScreen(core::GameSettings &settings, Callback onBack,
                             SDL_Rect viewport)
    : m_settings(settings), m_onBack(std::move(onBack)), m_viewport(viewport) {
  const int cx = viewport.x + viewport.w / 2;

  m_backBtn = core::UI::Button(cx - 100, viewport.y + viewport.h - 80, 200, 45, "BACK");
  m_soundBtn = core::UI::Button(cx - 100, viewport.y + 180, 200, 45, "SOUND");
}

void OptionsScreen::handleEvent(const SDL_Event &event,
                                const core::InputHandler &input) {
  if (event.type == SDL_MOUSEMOTION) {
    m_backBtn.hovered = m_backBtn.contains(event.motion.x, event.motion.y);
    m_soundBtn.hovered = m_soundBtn.contains(event.motion.x, event.motion.y);
  }

  if (event.type == SDL_MOUSEBUTTONUP &&
      event.button.button == SDL_BUTTON_LEFT) {
    if (m_backBtn.contains(event.button.x, event.button.y) && m_onBack)
      m_onBack();
    if (m_soundBtn.contains(event.button.x, event.button.y))
      m_settings.soundEnabled = !m_settings.soundEnabled;
    // TODO: handle brightness slider drag, volume slider, fullscreen toggle.
  }
  (void)input;
}

void OptionsScreen::update(float /*deltaSeconds*/) {
  // TODO: animate slider thumb movement.
}

void OptionsScreen::render(core::Renderer &renderer) {
  // Background.
  renderer.drawRect(m_viewport.x, m_viewport.y, m_viewport.w, m_viewport.h,
                   core::UI::Color(15, 15, 35, 255));

  // Sound toggle row.
  drawRow(renderer, "SOUND", m_soundBtn.x, m_soundBtn.y, m_soundBtn.width, m_soundBtn.height, m_settings.soundEnabled);

  // TODO: brightness row, volume row, fullscreen checkbox.

  // Back button.
  renderer.drawButton(m_backBtn,
                     core::UI::Color(60, 100, 200, 255),  // fill
                     core::UI::Color(200, 230, 255, 255), // outline
                     core::UI::Color(120, 180, 255, 255)); // hover
}

void OptionsScreen::drawRow(core::Renderer &renderer, const char * /*label*/,
                            int x, int y, int w, int h, bool value) const {
  // Indicator box.
  core::UI::Color indicator =
      value ? core::UI::Color(80, 220, 80, 255) : core::UI::Color(80, 80, 80, 255);
  renderer.drawRect(x, y, w, h, indicator);
  renderer.drawRectOutline(x, y, w, h, core::UI::Color(200, 200, 200, 255));
  // TODO: draw label text.
}

} // namespace screens
