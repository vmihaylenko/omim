#include "routing/routing_quality/waypoints.hpp"
#include "routing/routing_quality/utils.hpp"

#include "geometry/point2d.hpp"

#include "base/assert.hpp"
#include "base/logging.hpp"

#include <algorithm>
#include <utility>

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
    for (size_t k = 0; k < candidates.size(); ++k)
    {
      auto const & candidate = candidates[k];
      size_t j = 0;
      auto const points = FromLatLon(candidate.m_coords);
      auto const size = points.size();
      CHECK_GREATER(size, 0, ());
      size_t numberOfErrors = 0;
      for (size_t i = 0; i < size; ++i)
      {
        auto const & p1 = points[i];
        bool pointFound = false;
        for (; j < route.size(); ++j)
        {
          auto const & p2 = route[j];
          if (AlmostEqualAbs(p1, p2, kEpsilon))
          {
            pointFound = true;
            ++j;
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

    LOG(LINFO, ("Best result", bestResult));
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
