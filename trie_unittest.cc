#include "trie.h"

#define CATCH_CONFIG_MAIN
#include "third_party/catch.h"

// NOTE: don't use c-strings, includes '\0'
//       which is confusing.

TEST_CASE("count duplicates", "[trie]") {
    tools::trie<char, int> duplicates;
    auto empty1 = duplicates.insert(std::string());
    REQUIRE(empty1.second);

    auto empty2 = duplicates.insert(std::string());
    REQUIRE(!empty2.second);

    auto abcd = duplicates.insert(std::string("abcd"));
    REQUIRE(abcd.second);

    auto abc = duplicates.insert(std::string("abc"));
    REQUIRE(abc.second);  // Not what you need, ha?
}