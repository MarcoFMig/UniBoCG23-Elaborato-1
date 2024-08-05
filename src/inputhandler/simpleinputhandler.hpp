#pragma once

#include <map>
#include <functional>
#include <unordered_set>
#include <gl/freeglut.h>

#include "../lightweightutils/lwu.hpp"
#include "../lightweightutils/idprovider.hpp"
#include "../lightweightutils/scheduling/tickable.hpp"

#define __RESERVED_KEYS 5

class SimpleInputHandler : public Tickable {
  private:
    enum Action {
      KEYUP,
      KEYDOWN,
      KEYPRESS
    };
    struct EventData {
      Action type;
      unsigned char key;
      int mouseX;
      int mouseY;
    };
    using ObserverStructure = std::function<void(unsigned char, int, int)>;
    using ObserverMap = std::map<unsigned int, ObserverStructure>;
    using UnboundObserverMap = std::map<Action, ObserverMap>;
    using BoundObserverMap = std::map<unsigned char, UnboundObserverMap>;
    using HeldDownKeyList = std::unordered_set<unsigned char>;
    using InputVariationList = std::list<EventData>;

    int lastMouseX, lastMouseY;
    IDProvider idProvider;
    UnboundObserverMap unboundObservers;
    BoundObserverMap boundObservers;
    HeldDownKeyList heldDownKeyList;
    InputVariationList inputVariationList;
    std::map<unsigned int, std::map<unsigned int, ObserverMap::iterator>> observerReference;

    void propagateUpdate(unsigned char key, int mouseX, int mouseY);
    UnboundObserverMap generateUnboundMap();
    SimpleInputHandler();
    SimpleInputHandler(const SimpleInputHandler&) = delete;
    SimpleInputHandler& operator=(const SimpleInputHandler&) = delete;
  public:
    static SimpleInputHandler& getInstance() {
      static SimpleInputHandler instance;
      return instance;
    }
    void handleKeyDown(unsigned char key, int currentMouseX, int currentMouseY);
    void handleKeyUp(unsigned char key, int currentMouseX, int currentMouseY);
    unsigned int registerKeyUpListener(unsigned char bindToKey, ObserverStructure observer);
    unsigned int registerKeyUpListener(ObserverStructure observer);
    unsigned int registerKeyDownListener(unsigned char bindToKey, ObserverStructure observer);
    unsigned int registerKeyDownListener(ObserverStructure observer);
    unsigned int registerKeyListener(unsigned char bindToKey, ObserverStructure observer);
    unsigned int registerKeyListener(ObserverStructure observer);
    void unregisterListener(unsigned int observerIdentifier);
    void tick() override;
};
