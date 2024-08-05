#include "multiplecolliderexception.hpp"

MultipleColliderException::MultipleColliderException()
  : std::runtime_error("Only one collider per object can exist!") {}

MultipleColliderException::MultipleColliderException(unsigned int objectIdentifier)
  : std::runtime_error("Only one collider per object can exist! ObjectId:" + objectIdentifier) {}
