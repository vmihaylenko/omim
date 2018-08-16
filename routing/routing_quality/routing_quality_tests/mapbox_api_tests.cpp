#include "testing/testing.hpp"

#include "routing_quality/mapbox/api.hpp"
#include "routing_quality/mapbox/types.hpp"
#include "routing_quality/utils.hpp"

#include <vector>
#include <utility>

using namespace routing_quality;
using namespace mapbox;
using namespace std;

UNIT_TEST(MapboxApi_DirectionsSmoke)
{
  RouteParams params;
  params.m_coords = {{55.796977, 37.538985}, {55.765583, 37.603400}};
  auto const routes = Api::MakeDirectionsRequest(params).m_routes;
  TEST_EQUAL(routes.size(), 1, ());
  auto const & route = routes[0];
  TEST_GREATER(route.m_duration, 0, ());
  auto const & geometry = route.m_geometry;
  TEST_GREATER(geometry.m_coordinates.size(), 0, ());
  TEST_EQUAL(geometry.m_type, "LineString", ());
}

UNIT_TEST(MapboxApi_StylesSmoke)
{
  RouteParams params;
  params.m_coords = {{55.98347, 37.21146}, {55.41752, 37.89614}};
  auto const routes = Api::MakeDirectionsRequest(params).m_routes;
  auto const mapboxGeometry = routes[0].m_geometry;
  Geometry mapsmeGeometry;
  mapsmeGeometry.Build(GetRoutePoints(move(params)));
  Api::DrawRoutes(mapboxGeometry, mapsmeGeometry);
}