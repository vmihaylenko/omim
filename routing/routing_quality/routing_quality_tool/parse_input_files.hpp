#pragma once

#include "routing/routing_quality/waypoints.hpp"
#include "routing/routing_quality/utils.hpp"

#include "coding/file_reader.hpp"

#include "geometry/mercator.hpp"

#include "base/string_utils.hpp"

#include <iterator>
#include <string>
#include <utility>
#include <vector>

namespace routing_quality
{
template <typename Iter, typename Container>
void SerializeLatLon(Iter begin, Iter end, Container & c)
{
  c.reserve(std::distance(begin, end));
  for (auto it = begin; it != end; ++it)
  {
    auto const & str = *it;
    auto const coords = strings::Tokenize(str, ",");
    CHECK_EQUAL(coords.size(), 2, ("Incorrect string", str));
    double lat, lon;
    CHECK(strings::to_double(coords[0], lat), ("Incorrect string", coords[0]));
    CHECK(strings::to_double(coords[1], lon), ("Incorrect string", coords[1]));
    CHECK(MercatorBounds::ValidLat(lat), ("Incorrect lat", lat));
    CHECK(MercatorBounds::ValidLon(lon), ("Incorrect lon", lon));
    c.emplace_back(lat, lon);
  }
}

RouteParams SerializeRouteParams(std::string const & routeParamsFilePath)
{
  FileReader r(routeParamsFilePath);
  std::string data;
  r.ReadAsString(data);
  auto const tokenized = strings::Tokenize(data, ";");
  auto const size = tokenized.size();
  CHECK_GREATER_OR_EQUAL(size, 3, ("Incorrect string", data));
  RouteParams params;
  SerializeLatLon(tokenized.begin(), tokenized.end() - 1, params.m_coords);
  int type;
  CHECK(strings::to_int(tokenized[size - 1], type), ("Incorrect string", tokenized[size - 1]));
  CHECK_LESS(type, static_cast<int>(routing::VehicleType::Count), ("Incorrect vehicle type", type));
  params.m_type = static_cast<routing::VehicleType>(type);
  return params;
}

std::vector<Waypoints> SerializeWaypoints(string const & waypointsFilePath)
{
  FileReader r(waypointsFilePath);
  std::string data;
  r.ReadAsString(data);
  auto const tokenized = strings::Tokenize(data, "\n");
  CHECK_GREATER_OR_EQUAL(tokenized.size(), 1, ("Incorrect string", data));
  std::vector<Waypoints> candidates;
  for (auto const & line : tokenized)
  {
    auto const candidate = strings::Tokenize(line, ";");
    auto const size = candidate.size();
    CHECK_GREATER_OR_EQUAL(size, 3, ("Incorrect string", line));
    Waypoints c;
    SerializeLatLon(candidate.begin(), candidate.end() - 1, c.m_coords);
    double factor;
    CHECK(strings::to_double(candidate[size - 1], factor), ("Incorrect string", candidate[size - 1]));
    c.m_factor = factor;
    candidates.emplace_back(std::move(c));
  }

  return candidates;
}
}  // namespace routing_quality
