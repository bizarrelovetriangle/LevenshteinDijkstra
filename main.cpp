#define _ITERATOR_DEBUG_LEVEL 0
#include <iostream>
#include <vector>
#include "jsoncpp/include/json/json.h"
#include <optional>

#include <Windows.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <ranges>
#include <set>
#include <future>
#include <ranges>
#include <map>

std::vector<std::vector<int>> dp;
std::vector<std::vector<int>> colors;


const std::string RED = "\033[1;31m";
const std::string GREEN = "\033[1;32m";
const std::string YELLOW = "\033[1;33m";
const std::string CYAN = "\033[1;36m";
const std::string RESET = "\033[0m";

std::string greenStr(const auto& str)
{
    return std::format("{}{}{}", GREEN, str, RESET);
}

std::string cyanStr(const auto& str)
{
    return std::format("{}{}{}", CYAN, str, RESET);
}

std::string yellowStr(const auto& str)
{
    return std::format("{}{}{}", YELLOW, str, RESET);
}

void print(auto& word1, auto& word2)
{
    std::stringstream ss;
    ss << "    ";
    for (int x = 0; x < word1.size(); ++x) {
        ss << yellowStr(std::format("{} ", word1[x]));
    }
    ss << "\r\n";

    for (int y = 0; y < dp.front().size(); ++y) {
        if (y == 0 || y > word2.size()) ss << std::format("  ");
        else ss << yellowStr(std::format("{} ", word2[y - 1]));

        for (int x = 0; x < dp.size(); ++x) {
            if (colors[x][y] == 1) {
                ss << cyanStr(std::format("{} ", dp[x][y]));
            } else if (colors[x][y] == 2) {
                ss << greenStr(std::format("{} ", dp[x][y]));
            } else {
                ss << std::format("{} ", dp[x][y]);
            }
        }
        ss << "\r\n";
    }

    std::cout << "\033[H";
    std::cout << ss.str() << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

int dijkstraDistance(std::string word1, std::string word2) {
    using pair = std::pair<int, int>;
    // priority_queue can be utilized instead of multimap
    std::multimap<int, pair> map;
    std::set<pair> set;

    map.emplace(0, pair(0, 0));

    while (true) {
        auto [diss, p] = *map.begin();
        map.erase(map.begin());

        // Ignore nodes we already processed. They will always have the bigger distances than the previous time.
        if (auto insPair = set.insert(p); !insPair.second) {
            continue;
        }

        // Printing. We don't need dp for the algorithm
        {
            dp[p.first][p.second] = diss;
            print(word1, word2);
        }

        // Emplace new nodes into the map
        bool leftAvailable = p.first < word1.size();
        bool bottomAvailable = p.second < word2.size();

        if (leftAvailable) {
            map.emplace(diss + 1, pair(p.first + 1, p.second));
        }
        if (bottomAvailable) {
            map.emplace(diss + 1, pair(p.first, p.second + 1));
        }
        if (leftAvailable && bottomAvailable) {
            int diff = word1[p.first] == word2[p.second] ? 0 : 1;
            map.emplace(diss + diff, pair(p.first + 1, p.second + 1));
        }

        // We know that we are at the bottom left corner and we get here the fastest possible time.
        if (!leftAvailable && !bottomAvailable) {
            return diss;
        }
    }
}


int dijkstraDistance2(std::string word1, std::string word2) {
    using pair = std::pair<int, int>;
    // priority_queue can be utilized instead of multimap
    std::multimap<int, pair> map;
    std::set<pair> set;


    std::multimap<int, pair> map2;
    std::set<pair> set2;

    map.emplace(0, pair(0, 0));
    map2.emplace(0, pair(word1.length() + 1, word2.length() + 1));

    while (true) {
        if (map.begin()->first < map2.begin()->first) {
            auto [diss, p] = *map.begin();
            map.erase(map.begin());

            // Ignore nodes we already processed. They will always have the bigger distances than the previous time.
            if (auto insPair = set.insert(p); !insPair.second) {
                continue;
            }

            // Printing. We don't need dp for the algorithm
            {
                dp[p.first][p.second] = diss;
                colors[p.first][p.second] = 1;
                print(word1, word2);

                if (int prev = dp[p.first + 1][p.second + 1]; prev != 0) {
                    return prev + diss;
                }
            }

            // Emplace new nodes into the map
            bool leftAvailable = p.first < word1.size();
            bool bottomAvailable = p.second < word2.size();

            if (leftAvailable) {
                map.emplace(diss + 1, pair(p.first + 1, p.second));
            }
            if (bottomAvailable) {
                map.emplace(diss + 1, pair(p.first, p.second + 1));
            }
            if (leftAvailable && bottomAvailable) {
                int diff = word1[p.first] == word2[p.second] ? 0 : 1;
                map.emplace(diss + diff, pair(p.first + 1, p.second + 1));
            }

            // We know that we are at the bottom left corner and we get here the fastest possible time.
            if (!leftAvailable && !bottomAvailable) {
                return diss;
            }
        }
        else {
            auto [diss, p] = *map2.begin();
            map2.erase(map2.begin());

            // Ignore nodes we already processed. They will always have the bigger distances than the previous time.
            if (auto insPair = set2.insert(p); !insPair.second) {
                continue;
            }

            // Printing. We don't need dp for the algorithm
            {
                dp[p.first][p.second] = diss;
                colors[p.first][p.second] = 2;
                print(word1, word2);

                if (int prev = dp[p.first - 1][p.second - 1]; prev != 0) {
                    return prev + diss;
                }
            }

            // Emplace new nodes into the map
            bool leftAvailable = p.first > 1;
            bool topAvailable = p.second > 1;

            if (leftAvailable) {
                map2.emplace(diss + 1, pair(p.first - 1, p.second));
            }
            if (topAvailable) {
                map2.emplace(diss + 1, pair(p.first, p.second - 1));
            }
            if (leftAvailable && topAvailable) {
                int diff = word1[p.first - 2] == word2[p.second - 2] ? 0 : 1;
                map2.emplace(diss + diff, pair(p.first - 1, p.second - 1));
            }

            // We know that we are at the top left corner and we get here the fastest possible time.
            if (!leftAvailable && !topAvailable) {
                return diss;
            }
        }
    }
}

int levenshteinDistance(std::string word1, std::string word2) {
    for (int i = 1; i < word1.length() + 1; ++i)
        dp[i][0] = i;
    for (int i = 1; i < word2.length() + 1; ++i)
        dp[0][i] = i;

    for (int i = 0; i < word1.length(); ++i) {
        for (int j = 0; j < word2.length(); ++j) {
            if (word1[i] == word2[j])
            {
                dp[i + 1][j + 1] = dp[i][j];
            }
            else
            {
                dp[i + 1][j + 1] = 1 + (std::min)({ dp[i][j + 1], dp[i + 1][j], dp[i][j] });
            }

            print(word1, word2);
        }
    }
    return dp.back().back();
}

int main()
{
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_FONT_INFOEX fontex;
        GetCurrentConsoleFontEx(hOut, 0, &fontex);
        fontex.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        fontex.dwFontSize.X = 0;   // Set width to 0 for system to determine automatically
        fontex.dwFontSize.Y = 34;  // Set the height (font size in points)
        SetCurrentConsoleFontEx(hOut, FALSE, &fontex);
    }

    std::string word1 = "elephant";
    std::string word2 = "relevant";
    dp = std::vector(word1.length() + 2, std::vector<int>(word2.length() + 2, 0));
    colors = std::vector(word1.length() + 2, std::vector<int>(word2.length() + 2, 0));

    auto res = dijkstraDistance2(word1, word2);
    std::cout << "result: " << res << std::endl;
    std::cin.get();
}