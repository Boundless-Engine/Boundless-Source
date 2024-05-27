#include "IApplication.h"

#include <iostream>

#include <fstream>

#include <filesystem>

namespace Boundless {
	namespace I {

		#define DEFAULT_PREFS ApplicationPreferences{ {0, 0, 1920, 1080, true } }

		BReturn IApplication::LoadPrefs() {
			
			//auto read_mode = std::ios_base::binary;
			// determine if a .prefs file exists or not.
			std::ifstream file;
			file.open(prefs_filepath);

			if (!file.is_open()) {
				auto prefs = DEFAULT_PREFS;

				if (SavePrefs(&prefs) == FAILURE)
					return FAILURE;

				file.close();
				file.open(prefs_filepath);
			
				if (file.is_open()) {
					std::cout << "Something went wrong... " << std::endl;
					return FAILURE;
				}
			}

			std::cout << "Loading Application Prefrences: path " << prefs_filepath << std::endl;

			auto temp = new ApplicationPreferences();
			file.read((char*)temp, sizeof(ApplicationPreferences));
			
			memcpy(&prefs, temp, sizeof(ApplicationPreferences));

			delete temp;
			file.close();

			return SUCCESS;
		}

		BReturn IApplication::SavePrefs(ApplicationPreferences* pPrefs)
		{
			//auto read_mode = std::ios_base::binary;
			
			auto dir = prefs_filepath.substr(0, prefs_filepath.find_last_of('/') + 1);

			if (!std::filesystem::exists(dir)) {
				std::cout << "application preferences dont exist.. creating.." << std::endl;
				std::error_code err;
				std::filesystem::create_directories(dir, err);	
			}

			std::ofstream new_file(prefs_filepath);
			
			if (!new_file.is_open())
			{
				return FAILURE;
			}

			std::cout << "Saving Application Prefrences: path " << prefs_filepath << std::endl;

			if (pPrefs != nullptr)
				prefs = *pPrefs;

			new_file.write((char*)&prefs, sizeof(ApplicationPreferences));
			new_file.close();

			return SUCCESS;
		}

	}
}