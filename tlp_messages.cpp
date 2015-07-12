/*
 * tlp_messages.cpp
 *
 *  Created on: Jul 11, 2015
 *      Author: hooman
 */
#include "tlp_messages.h"

std::ostream& operator<<(std::ostream & Str, tlp_header const & v) {
  Str << "tlp_header";
  return Str;
}
std::ostream& operator<<(std::ostream & Str, npu_header const & v) {
  Str << "npu_header";
  return Str;
}
