#include <benchmark/benchmark.h>

#include <string>
#include <vector>

/* ------------- common ----------------------*/
const std::string id = "5201_1_100";

/* ------------- boost split -----------------*/
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

static void BM_boost(benchmark::State& state) {
  for (auto _ : state) {
    std::vector<std::string> ret_split;
    boost::split(ret_split, id, boost::is_any_of("_"),
                 boost::token_compress_on);
  }
}
BENCHMARK(BM_boost);

/* ------------- absl split ------------------*/
#include <absl/strings/str_split.h>

static void BM_absl(benchmark::State& state) {
  for (auto _ : state) {
    std::vector<std::string> ret_split;
    ret_split = absl::StrSplit(id, "_");
  }
}
BENCHMARK(BM_absl);

#include <absl/strings/string_view.h>
static void BM_absl1(benchmark::State& state) {
  for (auto _ : state) {
    std::vector<absl::string_view> ret_split;
    ret_split = absl::StrSplit(id, "_");
  }
}
BENCHMARK(BM_absl1);

/* ------------- string split ----------------*/
std::vector<std::string> split(const std::string& str,
                               const std::string& delimiter) {
  std::vector<std::string> res;
  size_t pos = 0;
  size_t index = 0;
  std::string token;
  while ((pos = str.find(delimiter, index)) != std::string::npos) {
    token = str.substr(index, pos - index);
    res.emplace_back(token);
    index = pos + delimiter.length();
  }
  token = str.substr(index);
  res.emplace_back(token);
  return res;
}

BENCHMARK_MAIN();
