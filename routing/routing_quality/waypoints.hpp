#pragma once

#include "routing/vehicle_mask.hpp"

#include "geometry/latlon.hpp"

#include <vector>

namespace routing_quality
{
struct RouteParams;

struct Waypoints
{
  std::vector<ms::LatLon> m_coords;
  // Value in range (0.0; 1.0] which indicates how desirable the route is.
  double m_factor = 1.0;
};

using Similarity = double;

Similarity CheckWaypoints(RouteParams && params, std::vector<Waypoints> && candidates);
}  // namespace routing_quality
