#pragma once

namespace utils
{
	inline std::vector<std::string> split(const std::string& text, char sep)
	{
		std::vector<std::string> tokens;
		std::size_t start = 0, end = 0;
		while ((end = text.find(sep, start)) != std::string::npos)
		{
			tokens.push_back(text.substr(start, end - start));
			start = end + 1;
		}
		tokens.push_back(text.substr(start));
		return tokens;
	}

	inline std::string replace_all(std::string str, const std::string& from, const std::string& to)
	{
		size_t start_pos = 0;
		while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length();
		}
		return str;
	}

	// Generates a random string of a given length
	inline std::string random_string(std::string::size_type length)
	{
		static auto& chrs = "abcdefghijklmnopqrstuvwxyz";

		std::mt19937 rg{ std::random_device{}() };
		std::uniform_int_distribution<std::string::size_type> pick(0, sizeof(chrs) - 2);

		std::string s;
		s.reserve(length);

		while (length--)
			s += chrs[pick(rg)];

		return s;
	}
}