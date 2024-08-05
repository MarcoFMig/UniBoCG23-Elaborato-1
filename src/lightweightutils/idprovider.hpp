#pragma once

#include <list>

class IDProvider {
  private:
    unsigned int nextSlot;
    std::list<unsigned int> freeSlots;
  public:
    IDProvider();
    IDProvider(unsigned int nextSlot, std::list<unsigned int> freeSlots);
    unsigned int provideId();
    void freeId(unsigned int identifier);
};
