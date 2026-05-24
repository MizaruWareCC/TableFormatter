#include <ios>
#include <iostream>
#include <tuple>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

std::string format_table(std::vector<std::vector<std::string>> entries, const int max_width) {
    int field_count = entries[0].size();
    std::vector<int> max_widths(field_count);

    // calculate max size field possible
    for (int i = 0; i < entries.size(); ++i) {
        for (int j = 0; j < field_count; ++j) {
            max_widths[j] = std::max(max_widths[j], std::min(static_cast<int>(entries[i][j].size() + 1), max_width));
        }
    }

    std::ostringstream stream;

    std::ostringstream cut_stream; // in between of entries
    cut_stream << '+';
    for (int i = 0; i < field_count; ++i) {
        cut_stream << std::left << std::string(max_widths[i], '-') << '+';
    }
    cut_stream << '\n';

    std::string cut = cut_stream.str();

    stream << cut;

    auto check = [&max_width](const std::string& str) {
        if (str.size() <= max_width)
            return std::tuple<std::string, std::string>(str, "");

        size_t split = str.rfind(' ', max_width-1);
        if (split == std::string::npos || split == 0)
            split = max_width;

        std::string first = str.substr(0, split);
        std::string rest  = (split < str.size()) ? str.substr(split) : "";

        while (!rest.empty() && rest.front() == ' ')
            rest.erase(rest.begin());

        return std::tuple<std::string, std::string>(first, rest);
    };

    auto is_v_empty = [](std::vector<std::string>& vec) {
    for (const auto& s : vec) if (s.size() != 0) return false;
    return true;
    };

    std::vector<std::string> entry(field_count);

    for (int i = 0; i < entries.size(); ++i) {
        entry = entries[i]; // mutable copy of entry

        while (!is_v_empty(entry)) {
            stream << '|';
            for (int j = 0; j < field_count; ++j) {
                auto c = check(entry[j]);
                entry[j] = std::get<1>(c);
                stream << std::left << std::setw(max_widths[j]) << std::get<0>(c) << '|';
            }
            stream << "\n";
        }
        stream << cut;
    }

    return stream.str();
}

int main() {
    std::vector<std::vector<std::string>> entries = {
    {
        "Name",
        "Email",
        "Adress",
        "About me",
        "Gender"
    }, // fields
    {
        "Jhon",
        "jhon@gmail.com",
        "XXX street NN home N8",
        "I am cook. I like eating delicious food!",
        "Male"
    }, // entry 1
    {
        "Alex",
        "alex@outlook.com",
        "PPP street UU home N3",
        "A programmer. I like pancakes and jems!",
        "Female"
    } // entry 2
    };


    std::cout << format_table(entries, 20);

    return 0;
}
