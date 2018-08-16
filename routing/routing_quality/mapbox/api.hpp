#pragma once

#include "routing/routing_quality/mapbox/types.hpp"
#include "routing/routing_quality/utils.hpp"

namespace routing_quality
{
namespace mapbox
{
class Api
{
public:
  static DirectionsResponce MakeDirectionsRequest(RouteParams const & params);
  static void DrawRoutes(Geometry const & mapboxRoute, Geometry const & mapsmeRoute);
};
}  // namespace mapbox
}  // namespace routing_quality