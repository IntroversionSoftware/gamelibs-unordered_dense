#include <ankerl/unordered_dense.h>
#include <fuzz/Provider.h>

#if defined(FUZZ)
#    define REQUIRE(x) ::fuzz::Provider::require(x)
#else
#    include <doctest.h>
#endif

#include <unordered_map>

namespace fuzz {

void string(uint8_t const* data, size_t size) {
    auto p = fuzz::Provider(data, size);

    auto ank = ankerl::unordered_dense::map<std::string, std::string>();
    auto ref = std::unordered_map<std::string, std::string>();

    while (p.has_remaining_bytes()) {
        auto str = p.string(32);
        REQUIRE(ank.try_emplace(str, "hello!").second == ref.try_emplace(str, "hello!").second);

        str = p.string(32);
        auto it_ank = ank.find(str);
        auto it_ref = ref.find(str);
        REQUIRE((it_ank == ank.end()) == (it_ref == ref.end()));

        if (it_ank != ank.end()) {
            ank.erase(it_ank);
            ref.erase(it_ref);
        }
        REQUIRE(ank.size() == ref.size());

        str = p.string(32);
        REQUIRE(ank.try_emplace(str, "huh").second == ref.try_emplace(str, "huh").second);

        str = p.string(32);
        REQUIRE(ank.erase(str) == ref.erase(str));
    }

    REQUIRE(std::unordered_map(ank.begin(), ank.end()) == ref);
}

} // namespace fuzz

#if defined(FUZZ)
extern "C" auto LLVMFuzzerTestOneInput(uint8_t const* data, size_t size) -> int {
    fuzz::string(data, size);
    return 0;
}
#endif