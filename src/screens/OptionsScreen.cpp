// screens/OptionsScreen.cpp
#include "OptionsScreen.hpp"

namespace screens {

OptionsScreen::OptionsScreen(core::GameSettings &settings, Callback onBack,
                             SDL_Rect viewport)
    : m_settings(settings), m_onBack(std::move(onBack)), m_viewport(viewport) {
  const int cx = viewport.x + viewport.w / 2;

  m_backBtn = {{cx - 100, viewport.y + viewport.h - 80, 200, 45}, "BACK"};
  m_soundBtn = {{cx - 100, viewport.y + 180, 200, 45}, "SOUND"};
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

void OptionsScreen::render(SDL_Renderer *renderer) {
  // Background.
  SDL_SetRenderDrawColor(renderer, 15, 15, 35, 255);
  SDL_RenderFillRect(renderer, &m_viewport);

  // Sound toggle row.
  drawRow(renderer, "SOUND", m_soundBtn.rect, m_settings.soundEnabled);

  // TODO: brightness row, volume row, fullscreen checkbox.

  // Back button.
  SDL_Color fill = m_backBtn.hovered ? SDL_Color{120, 180, 255, 255}
                                     : SDL_Color{60, 100, 200, 255};
  SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, fill.a);
  SDL_RenderFillRect(renderer, &m_backBtn.rect);
  SDL_SetRenderDrawColor(renderer, 200, 230, 255, 255);
  SDL_RenderDrawRect(renderer, &m_backBtn.rect);
  // TODO: "BACK" label via SDL2_ttf.
}

void OptionsScreen::drawRow(SDL_Renderer *renderer, const char * /*label*/,
                            const SDL_Rect &rowRect, bool value) const {
  // Indicator box.
  SDL_Color indicator =
      value ? SDL_Color{80, 220, 80, 255} : SDL_Color{80, 80, 80, 255};
  SDL_SetRenderDrawColor(renderer, indicator.r, indicator.g, indicator.b, 255);
  SDL_RenderFillRect(renderer, &rowRect);
  SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
  SDL_RenderDrawRect(renderer, &rowRect);
  // TODO: draw label text.
}

} // namespace screens
