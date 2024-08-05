#include "ui.hpp"

#include "../exceptions/invalidfontexception.hpp"

std::shared_ptr<Font> UI::addFont(Font fontToAdd) {
  std::shared_ptr<Font> fontPtr = std::make_shared<Font>(std::move(fontToAdd));
  return this->addFont(fontPtr);
}
std::shared_ptr<Font> UI::addFont(std::shared_ptr<Font> fontToAdd) {
  fontToAdd->setIdentifier(this->idProvider.provideId());
  this->fontMap.emplace(fontToAdd->getIdentifier().value(), fontToAdd);
  return fontToAdd;
}

std::unordered_map<unsigned int, std::shared_ptr<Font>> UI::getFonts() {
  return this->fontMap;
}

std::unordered_map<unsigned int, std::shared_ptr<Font>>& UI::peekFonts() {
  return this->fontMap;
}

void UI::removeFont(std::shared_ptr<Font> fontToRemove) {
  if (!fontToRemove->getIdentifier().has_value()) {
    throw InvalidFontException();
  }
  this->fontMap.erase(fontToRemove->getIdentifier().value());
}

unsigned int UI::registerUiListener(UI::UIListener listener) {
  unsigned int providedId = this->idProvider.provideId();
  this->listenerMap.emplace(providedId, listener);
  return providedId;
}

void UI::unregisterUiListener(unsigned int listenerId) {
  this->idProvider.freeId(listenerId);
  this->listenerMap.erase(listenerId);
}
