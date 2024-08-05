#pragma once

#include <memory>
#include <functional>
#include <unordered_map>

#include "font.hpp"
#include "../lightweightutils/idprovider.hpp"

class UI {
  public:
    enum UIAction {
      font_add,
      font_remove
    };
    using UIListener = std::function<void(UI::UIAction, std::shared_ptr<Font>)>;
  private:
    IDProvider idProvider;
    std::unordered_map<unsigned int, std::shared_ptr<Font>> fontMap; 
    std::unordered_map<unsigned int, UI::UIListener> listenerMap;
    // Add a map of UI elements.
  public:
    std::shared_ptr<Font> addFont(Font fontToAdd);
    std::shared_ptr<Font> addFont(std::shared_ptr<Font> fontToAdd);
    std::unordered_map<unsigned int, std::shared_ptr<Font>> getFonts();
    std::unordered_map<unsigned int, std::shared_ptr<Font>>& peekFonts();
    void removeFont(std::shared_ptr<Font> fontToRemove);
    unsigned int registerUiListener(UI::UIListener listener);
    void unregisterUiListener(unsigned int listenerId);
};
