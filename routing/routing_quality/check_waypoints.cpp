#include "routing/routing_quality/check_waypoints.hpp"
#include "routing/routing_quality/utils.hpp"

#include "geometry/point2d.hpp"

#include "base/assert.hpp"
#include "base/logging.hpp"

#include <algorithm>

using namespace std;

namespace routing_quality
{
namespace metrics
{
struct NumberOfMatchedWaypoints
{
  Similarity operator()(RoutePoints && route, vector<Waypoints> && candidates) const
  {
    auto constexpr kEpsilon = 0.001;

    Similarity bestResult = 0.0;
    for (auto k = 0UL; k < candidates.size(); ++k)
    {
      auto const & candidate = candidates[k];
      auto j = 0UL;
      auto const points = FromLatLon(candidate.m_coords);
      auto const size = points.size();
      auto numberOfErrors = 0UL;
      for (auto i = 0UL; i < size; ++i)
      {
        auto const & p1 = points[i];
        bool pointFound = false;
        for (; j < route.size(); ++j)
        {
          auto const & p2 = route[j];
          if (AlmostEqualAbs(p1, p2, kEpsilon))
          {
            pointFound = true;
            break;
          }
        }

        if (!pointFound)
        {
          LOG(LINFO, ("Can't find point", candidate.m_coords[i], "with index", i));
          ++numberOfErrors;
        }
      }

      CHECK_LESS_OR_EQUAL(numberOfErrors, size, ());
      auto const result = ((size - numberOfErrors) / static_cast<double>(size)) * candidate.m_factor;
      LOG(LINFO, ("Matching result", result, "for route with index", k));
      bestResult = max(bestResult, result);
    }

    return bestResult;
  }
};
}  // namespace metrics

Similarity CheckWaypoints(RouteParams && params, vector<Waypoints> && candidates)
{
  auto points = GetRoutePoints(move(params));
  return metrics::NumberOfMatchedWaypoints()(move(points), move(candidates));
}

}  // namespace routing_quality
