#pragma once

#include <deque>
#include <map>
#include <string>
#include <vector>

#include "src/stirling/cassandra/cass_types.h"
#include "src/stirling/common/parse_state.h"

namespace pl {
namespace stirling {
namespace cass {

/**
 * ProcessFrames is the entry point of the Cassandra Stitcher. It loops through the resp_packets,
 * matches them with the corresponding req_packets, and optionally produces an entry to emit.
 *
 * @param req_packets: deque of all request frames.
 * @param resp_packets: deque of all response frames.
 * @return A vector of entries to be appended to table store.
 */
std::vector<Record> ProcessFrames(std::deque<Frame>* req_packets, std::deque<Frame>* resp_packets);

}  // namespace cass
}  // namespace stirling
}  // namespace pl
