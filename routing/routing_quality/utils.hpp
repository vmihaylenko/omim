#pragma once

#include "routing/vehicle_mask.hpp"

#include "geometry/latlon.hpp"
#include "geometry/point2d.hpp"

#include <vector>

namespace routing_quality
{
struct RouteParams
{
  std::vector<ms::LatLon> m_coords;
  routing::VehicleType m_type = routing::VehicleType::Car;
};

using RoutePoints = std::vector<m2::PointD>;

RoutePoints GetRoutePoints(RouteParams && params);
RoutePoints FromLatLon(std::vector<ms::LatLon> const & coords);
}  // namespace routing_quality
