#pragma once

#include "storage/diff_scheme/diff_types.hpp"

namespace diff_scheme
{
class Checker final
{
public:
  static NameFileInfoMap Check(LocalMapsInfo const & info);
};
}  // namespace diff_scheme
