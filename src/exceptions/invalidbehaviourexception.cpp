#include "invalidbehaviourexception.hpp"

InvalidBehaviourException::InvalidBehaviourException()
  : std::runtime_error("An error occourred while accessing a non existing object Id!") {}

InvalidBehaviourException::InvalidBehaviourException(unsigned int objectIdentifier)
  : std::runtime_error("An error occourred while accessing object Id: " + objectIdentifier) {}
