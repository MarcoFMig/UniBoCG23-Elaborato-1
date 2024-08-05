#include "idprovider.hpp"

#include "exceptions/nonexistantidexception.hpp"

IDProvider::IDProvider () {
  this->nextSlot = 0;
}
IDProvider::IDProvider(unsigned int nextSlot, std::list<unsigned int> freeSlots) {
  this->nextSlot = nextSlot;
  this->freeSlots = freeSlots;
}

unsigned int IDProvider::provideId() {
  if (this->freeSlots.size() == 0) {
    return nextSlot++;
  } else {
    unsigned int slotToReturn = this->freeSlots.front();
    this->freeSlots.pop_front();
    return slotToReturn;
  }
}

void IDProvider::freeId(unsigned int identifier) {
  if (identifier > nextSlot) {
    throw NonExistantIDException(identifier, "freeing identifier");
  }  else if (identifier == this->nextSlot - 1) {
    this->nextSlot--;
  } else {
    this->freeSlots.push_back(identifier);
  }
}
