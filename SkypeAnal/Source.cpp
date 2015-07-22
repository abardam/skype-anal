#include <iostream>
#include <fstream>
#include <regex>
#include <map>

struct Stats {
	std::string name;
	int num_lines = 0;
};

typedef std::map<std::string, Stats> StatsMap;
typedef std::pair<std::string, Stats> StatsPair;

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Enter file name!\n";
		return 0;
	}

	std::string filename(argv[1]);

	std::ifstream in;
	in.open(filename.c_str());

	if (!in.is_open()) {
		std::cout << "File " << filename << " not found!\n";
		return 0;
	}

	//use regex ^\[([0-9/]* )?[0-9:]* [AP]M\] [a-z ]*:
	//or ^\[[0-9/:]* [AP]M\] [a-z ]*:
	std::regex date_name_regex("^\\[[0-9/: ]*[AP]M\\] [A-Za-z ]*:");

	StatsMap stats_map;

	char line[2048];

	while (in.getline(line, 2048)) {
		std::string sline(line);

		std::smatch sm;
		std::regex_search(sline, sm, date_name_regex);

		if (sm.size() > 0) {
			std::string date_name = sm[0];
			size_t name_idx = date_name.find_last_of(']') + 2;
			size_t name_len = date_name.length() - name_idx - 1;

			std::string name = date_name.substr(name_idx, name_len);

			if (stats_map.find(name) == stats_map.end()) {
				Stats nstats;
				nstats.name = name;
				stats_map.insert(StatsPair(name, nstats));
			}

			stats_map[name].num_lines++;
		}
		
	}

	for (auto it = stats_map.begin(); it != stats_map.end(); ++it) {
		Stats& stats = it->second;

		std::cout << stats.name << ":\n" << "num lines: " << stats.num_lines 
			<< std::endl;
	}
}