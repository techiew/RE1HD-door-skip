#pragma once 

#include <iostream>
#include <vector>
#include <Windows.h>
#include <Psapi.h>
#include <map>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <unordered_map>
#include <filesystem>
#include <Xinput.h>

namespace mod_stuff
{
	inline std::string target_window_name = "NULL";
	inline HWND app_window = NULL;
	inline bool logging_enabled = false;
	inline std::ofstream log_file;

	template<typename... Types> inline void log(Types... args);
	inline void enable_logging(bool enable);
	inline void show_error_popup(std::string error);
	inline void set_target_window_name(std::string target_window_name);
	inline std::string get_current_process_name();
	inline std::string get_current_module_name();
	inline DWORD_PTR get_process_base_address(DWORD process_id);
	inline void mem_copy(uintptr_t destination, uintptr_t source, size_t num_bytes);
	inline void mem_set(uintptr_t address, unsigned char byte, size_t num_bytes);
	inline void toggle_memory_protection(bool enable_protection, uintptr_t address, size_t size);
	inline bool verify_aob(std::string aob);
	inline bool verify_aobs(std::vector<std::string> aobs);
	inline bool is_aob_valid(std::vector<std::string> aob_tokens);
	inline bool do_aobs_match(std::string aob1, std::string aob2);
	inline std::vector<std::string> tokenify_aob_string(std::string aob);
	inline std::vector<unsigned char> string_aob_to_raw_aob(std::string aob);
	inline std::string raw_aob_to_string_aob(std::vector<unsigned char> raw_aob);
	inline uintptr_t aob_scan(std::string aob);
	template<typename T> inline std::string number_to_hex_string(T number);
	inline std::string number_to_hex_string(unsigned char number);
	inline bool replace_bytes_at_address(uintptr_t address, std::string expected_bytes, std::string new_bytes);
	inline void hook(uintptr_t address, uintptr_t destination);
	inline void hook32(uintptr_t address, uintptr_t destination);
	inline void hook64(uintptr_t address, uintptr_t destination);
	inline int32_t calculate_displacement_for_relative_jump(uintptr_t relative_jump_address, uintptr_t destination_address);
	inline uintptr_t relative_to_absolute_address(uintptr_t relative_address_location);
	inline bool keys_pressed(std::vector<std::string> key_combination, bool return_true_while_held);
	inline void get_window_handle();
	inline bool get_window_handle_by_window_name();
	inline bool get_window_handle_by_enumeration();
	inline BOOL CALLBACK enum_window_handles(HWND hwnd, LPARAM lParam);

	const inline std::unordered_map<std::string, unsigned short> string_to_keycode_map = {
		{"lmb", VK_LBUTTON}, {"rmb", VK_RBUTTON}, {"mmb", VK_MBUTTON}, {"mb1", VK_XBUTTON1}, {"mb2", VK_XBUTTON2},
		{"backspace", VK_BACK}, {"tab", VK_TAB}, {"enter", VK_RETURN}, {"lshift", VK_LSHIFT}, {"rshift", VK_RSHIFT},
		{"lctrl", VK_LCONTROL}, {"rctrl", VK_RCONTROL}, {"lalt", VK_LMENU}, {"ralt", VK_RMENU}, {"pause", VK_PAUSE},
		{"capslock", VK_CAPITAL}, {"escape", VK_ESCAPE}, {"spacebar", VK_SPACE}, {"pageup", VK_PRIOR}, {"pagedown", VK_NEXT},
		{"end", VK_END}, {"home", VK_HOME}, {"leftarrow", VK_LEFT}, {"uparrow", VK_UP}, {"rightarrow", VK_RIGHT},
		{"downarrow", VK_DOWN}, {"printscreen", VK_SNAPSHOT}, {"insert", VK_INSERT}, {"delete", VK_DELETE}, {"0", 0x30},
		{"1", 0x31}, {"2", 0x32}, {"3", 0x33}, {"4", 0x34}, {"5", 0x35},
		{"6", 0x36}, {"7", 0x37}, {"8", 0x38}, {"9", 0x39}, {"a", 0x41},
		{"b", 0x42}, {"c", 0x43}, {"d", 0x44}, {"e", 0x45}, {"f", 0x46},
		{"g", 0x47}, {"h", 0x48}, {"i", 0x49}, {"j", 0x4a}, {"k", 0x4b},
		{"l", 0x4c}, {"m", 0x4d}, {"n", 0x4e}, {"o", 0x4f}, {"p", 0x50},
		{"q", 0x51}, {"r", 0x52}, {"s", 0x53}, {"t", 0x54}, {"u", 0x55},
		{"v", 0x56}, {"w", 0x57}, {"x", 0x58}, {"y", 0x59}, {"z", 0x5a},
		{"numpad0", VK_NUMPAD0}, {"numpad1", VK_NUMPAD1}, {"numpad2", VK_NUMPAD2}, {"numpad3", VK_NUMPAD3}, {"numpad4", VK_NUMPAD4},
		{"numpad5", VK_NUMPAD5}, {"numpad6", VK_NUMPAD6}, {"numpad7", VK_NUMPAD7}, {"numpad8", VK_NUMPAD8}, {"numpad9", VK_NUMPAD9},
		{"multiply", VK_MULTIPLY}, {"add", VK_ADD}, {"separator", VK_SEPARATOR}, {"subtract", VK_SUBTRACT}, {"decimal", VK_DECIMAL},
		{"divide", VK_DIVIDE}, {"f1", VK_F1}, {"f2", VK_F2}, {"f3", VK_F3}, {"f4", VK_F4},
		{"f5", VK_F5}, {"f6", VK_F6}, {"f7", VK_F7}, {"f8", VK_F8}, {"f9", VK_F9},
		{"f10", VK_F10}, {"f11", VK_F11}, {"f12", VK_F12}, {"numlock", VK_NUMLOCK}, {"scrolllock", VK_SCROLL}
	};

	const inline std::unordered_map<std::string, unsigned short> string_to_xinput_keycode_map = {
		{"dpadup", 0x0001}, {"dpaddown", 0x0002}, {"dpadleft", 0x0004}, {"dpadright", 0x0008}, {"start", 0x0010},
		{"select", 0x0020}, {"lthumbpress", 0x0040}, {"rthumbpress", 0x0080}, {"lshoulder", 0x0100}, {"rshoulder", 0x0200},
		{"xa", 0x1000}, {"xb", 0x2000}, {"xx", 0x4000}, {"xy", 0x8000}
	};

	class Timer
	{
	public:
		Timer(size_t interval_ms)
		{
			this->interval_ms = interval_ms;
			timestamp = std::chrono::system_clock::now();
		}

		void reset()
		{
			timestamp = std::chrono::system_clock::now();
		}

		bool has_elapsed()
		{
			auto now = std::chrono::system_clock::now();
			auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - timestamp);
			if (diff.count() >= interval_ms)
			{
				return true;
			}
			return false;
		}

	private:
		size_t interval_ms = 0;
		std::chrono::system_clock::time_point timestamp;
	};

	template<typename... Types>
	inline void log(Types... args)
	{
		const std::string log_file_name = get_current_module_name() + "_log.txt";
		constexpr size_t failbit = 2;
		std::stringstream stream;

		if (!logging_enabled)
		{
			return;
		}

		stream << get_current_module_name() << " > ";
		(stream << ... << args) << std::endl;
		std::cout << stream.str();

		if (!log_file.fail())
		{
			if (!log_file.is_open())
			{
				std::cout << "Log file opened" << std::endl;
				log_file.open(log_file_name);
			}
			if (log_file.is_open())
			{
				// Stop at a log file size of > 100MB to avoid filling disks
				if (std::filesystem::file_size(log_file_name) > 100000000)
				{
					log_file << "Max log file size reached. Stopping logging.";
					log_file.flush();
					log_file.setstate(failbit);
				}
				else
				{
					log_file << stream.str();
					log_file.flush();
				}
			}
		}
	}

	inline void enable_logging(bool enable = true)
	{
		logging_enabled = enable;
	}

	inline void show_error_popup(std::string error)
	{
		MessageBox(NULL, error.c_str(), get_current_module_name().c_str(), MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
	}

	inline void set_target_window_name(std::string target_window_name)
	{
		mod_stuff::target_window_name = target_window_name;
	}

	inline std::string get_current_process_name()
	{
		static std::string current_process_name = "NULL";
		if(current_process_name != "NULL")
		{
			return current_process_name;
		}

		HMODULE module = NULL;
		char lp_filename[MAX_PATH];
		GetModuleFileNameA(module, lp_filename, sizeof(lp_filename));
		std::string module_name = strrchr(lp_filename, '\\');
		current_process_name = module_name.substr(1, module_name.length());
		return current_process_name;
	}

	inline std::string get_current_module_name()
	{
		static std::string current_module_name = "NULL";
		if (current_module_name != "NULL")
		{
			return current_module_name;
		}

		HMODULE module = NULL;
		static char dummy_static_var_to_get_module_handle = 'x';
		GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, &dummy_static_var_to_get_module_handle, &module);
		char lp_filename[MAX_PATH];
		GetModuleFileNameA(module, lp_filename, sizeof(lp_filename));
		current_module_name = strrchr(lp_filename, '\\');
		current_module_name = current_module_name.substr(1, current_module_name.length());
		current_module_name.erase(current_module_name.find(".dll"), current_module_name.length());

		return current_module_name;
	}

	inline DWORD_PTR get_process_base_address(DWORD process_id)
	{
		DWORD_PTR base_address = 0;
		HANDLE process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
		HMODULE* module_array = nullptr;
		LPBYTE module_array_bytes = 0;
		DWORD bytes_required = 0;

		if (process_handle)
		{
			if (EnumProcessModules(process_handle, NULL, 0, &bytes_required))
			{
				if (bytes_required)
				{
					module_array_bytes = (LPBYTE)LocalAlloc(LPTR, bytes_required);
					if (module_array_bytes)
					{
						unsigned int module_count;
						module_count = bytes_required / sizeof(HMODULE);
						module_array = (HMODULE*)module_array_bytes;
						if (EnumProcessModules(process_handle, module_array, bytes_required, &bytes_required))
						{
							base_address = (DWORD_PTR)module_array[0];
						}
						LocalFree(module_array_bytes);
					}
				}
			}
			CloseHandle(process_handle);
		}
		return base_address;
	}

	inline void mem_copy(uintptr_t destination, uintptr_t source, size_t num_bytes)
	{
		toggle_memory_protection(false, destination, num_bytes);
		toggle_memory_protection(false, source, num_bytes);
		memcpy((void*)destination, (void*)source, num_bytes);
		toggle_memory_protection(true, source, num_bytes);
		toggle_memory_protection(true, destination, num_bytes);
	}

	inline void mem_set(uintptr_t address, unsigned char byte, size_t num_bytes)
	{
		toggle_memory_protection(false, address, num_bytes);
		memset((void*)address, byte, num_bytes);
		toggle_memory_protection(true, address, num_bytes);
	}

	inline void toggle_memory_protection(bool enable_protection, uintptr_t address, size_t size)
	{
		static std::map<uintptr_t, DWORD> protection_history;
		if (enable_protection && protection_history.find(address) != protection_history.end())
		{
			VirtualProtect((void*)address, size, protection_history[address], &protection_history[address]);
			protection_history.erase(address);
		}
		else if (!enable_protection && protection_history.find(address) == protection_history.end())
		{
			DWORD old_protection = 0;
			VirtualProtect((void*)address, size, PAGE_EXECUTE_READWRITE, &old_protection);
			protection_history[address] = old_protection;
		}
	}
 
	inline bool verify_aob(std::string aob)
	{
		std::vector<std::string> aob_tokens = tokenify_aob_string(aob);
		if (!is_aob_valid(aob_tokens))
		{
			log("AOB is invalid! (" + aob + ")");
			return false;
		};
		return true;
	}

	inline bool verify_aobs(std::vector<std::string> aobs)
	{
		for (const auto& aob : aobs)
		{
			if (!verify_aob(aob))
			{
				return false;
			}
		}
		return true;
	}

	inline bool is_aob_valid(std::vector<std::string> aob_tokens)
	{
		for (const auto& byte : aob_tokens)
		{
			if (byte == "?")
			{
				continue;
			}

			if (byte.length() != 2)
			{
				return false;
			}

			std::string whitelist = "0123456789abcdef";
			if (byte.find_first_not_of(whitelist) != std::string::npos)
			{
				return false;
			}
		}
		return true;
	}

	inline bool do_aobs_match(std::string aob1, std::string aob2)
	{
		std::vector<std::string> aob1_tokens = tokenify_aob_string(aob1);
		std::vector<std::string> aob2_tokens = tokenify_aob_string(aob2);

		size_t shortest_aob_length = aob1_tokens.size() < aob2_tokens.size() ? aob1_tokens.size() : aob2_tokens.size();
		for (size_t i = 0; i < shortest_aob_length; i++)
		{
			bool is_token_masked = aob1_tokens[i] == "?" || aob2_tokens[i] == "?";
			if (is_token_masked)
			{
				continue;
			}
			if (aob1_tokens[i] != aob2_tokens[i])
			{
				log("Bytes do not match!");
				return false;
			}
		}
		return true;
	}

	inline std::vector<std::string> tokenify_aob_string(std::string aob)
	{
		std::istringstream iss(aob);
		std::vector<std::string> aob_tokens{
			std::istream_iterator<std::string>{iss},
			std::istream_iterator<std::string>{}
		};
		return aob_tokens;
	}

	inline std::vector<unsigned char> string_aob_to_raw_aob(std::string aob)
	{
		std::vector<unsigned char> raw_aob;
		std::vector<std::string> tokenified_aob = tokenify_aob_string(aob);
		for (size_t i = 0; i < tokenified_aob.size(); i++)
		{
			if (tokenified_aob[i] == "?")
			{
				log("Cannot convert AOB with mask to raw AOB");
				return std::vector<unsigned char>();
			}

			unsigned char byte = (unsigned char)std::stoul(tokenified_aob[i], nullptr, 16);
			raw_aob.push_back(byte);
		}
		return raw_aob;
	}

	inline std::string raw_aob_to_string_aob(std::vector<unsigned char> raw_aob)
	{
		std::string aob;
		for (const auto& byte : raw_aob)
		{
			std::string string = number_to_hex_string(byte);
			aob += string + " ";
		}
		aob.pop_back();
		return aob;
	}

	inline uintptr_t aob_scan(std::string aob)
	{
		if (!verify_aob(aob))
		{
			return 0;
		};

		std::vector<std::string> aob_tokens = tokenify_aob_string(aob);
		DWORD process_id = GetCurrentProcessId();
		uintptr_t region_start = get_process_base_address(process_id);
		size_t num_regions_checked = 0;
		size_t max_regions_to_check = 10000;
		uintptr_t current_address = 0;

		while (num_regions_checked < max_regions_to_check)
		{
			MEMORY_BASIC_INFORMATION memory_info = { 0 };
			if (VirtualQuery((void*)region_start, &memory_info, sizeof(MEMORY_BASIC_INFORMATION)) == 0)
			{
				DWORD error = GetLastError();
				if (error == ERROR_INVALID_PARAMETER)
				{
					log("Reached end of scannable memory.");
				}
				else
				{
					log("VirtualQuery failed, error code: ", error);;
				}
				break;
			}
			region_start = (uintptr_t)memory_info.BaseAddress;
			uintptr_t region_size = (uintptr_t)memory_info.RegionSize;
			uintptr_t region_end = region_start + region_size;
			uintptr_t protection = (uintptr_t)memory_info.Protect;
			uintptr_t state = (uintptr_t)memory_info.State;

			bool memory_is_readable = 
				state == MEM_COMMIT
				&& (protection == PAGE_EXECUTE
				|| protection == PAGE_EXECUTE_READ
				|| protection == PAGE_EXECUTE_READWRITE
				|| protection == PAGE_EXECUTE_WRITECOPY
				|| protection == PAGE_READONLY
				|| protection == PAGE_READWRITE
				|| protection == PAGE_WRITECOPY);
			if (memory_is_readable)
			{
				log("Checking region: ", number_to_hex_string(region_start));
				current_address = region_start;
				while (current_address < region_end - aob_tokens.size())
				{
					for (size_t i = 0; i < aob_tokens.size(); i++)
					{
						if (aob_tokens[i] == "?")
						{
							current_address++;
							continue;
						} 
						else if (*(unsigned char*)current_address != (unsigned char)std::stoul(aob_tokens[i], nullptr, 16))
						{
							current_address++;
							break;
						}
						else if (i == aob_tokens.size() - 1)
						{
							uintptr_t signature = current_address - aob_tokens.size() + 1;
							log("Found signature at ", number_to_hex_string(signature));
							return signature;
						}
						current_address++;
					}
				}
			}
			else
			{
				log("Skipped region: ", number_to_hex_string(region_start));
			}
			num_regions_checked++;
			region_start += memory_info.RegionSize;
		}

		log("Stopped at: ", number_to_hex_string(current_address), ", num regions checked: ", num_regions_checked);
		return 0;
	}

	template<typename T>
	inline std::string number_to_hex_string(T number)
	{
		std::stringstream stream;
		stream
			<< std::setfill('0')
			<< std::setw(sizeof(T) * 2)
			<< std::hex
			<< number;
		return stream.str();
	}

	inline std::string number_to_hex_string(unsigned char number)
	{
		std::stringstream stream;
		stream
			<< std::setw(2)
			<< std::setfill('0')
			<< std::hex
			<< (unsigned int)number; // The << operator overload for unsigned chars screws us over unless this cast is done
		return stream.str();
	}

	inline bool replace_bytes_at_address(uintptr_t address, std::string expected_bytes, std::string new_bytes)
	{
		if (!verify_aobs({ expected_bytes, new_bytes }))
		{
			return false;
		}

		std::vector<std::string> expected_bytes_tokens = tokenify_aob_string(expected_bytes);
		std::vector<unsigned char> existing_bytes_buffer(expected_bytes_tokens.size(), 0);
		mem_copy((uintptr_t)&existing_bytes_buffer[0], address, existing_bytes_buffer.size());
		std::string existing_bytes = raw_aob_to_string_aob(existing_bytes_buffer);

		log("Bytes at address: ", existing_bytes);
		log("Expected bytes: ", expected_bytes);
		log("New bytes: ", new_bytes);

		if (do_aobs_match(existing_bytes, expected_bytes))
		{
			log("Bytes match");
			std::vector<unsigned char> raw_new_bytes = string_aob_to_raw_aob(new_bytes);
			mem_copy(address, (uintptr_t)&raw_new_bytes[0], raw_new_bytes.size());
			log("Patch applied");
			return true;
		}
		return false;
	}

	inline void hook(uintptr_t address, uintptr_t destination)
	{
#ifndef _WIN64
		hook32(address, destination);
#else
		hook64(address, destination);
#endif
		log("Created jump from ", number_to_hex_string(address), " to ", number_to_hex_string(destination));
	}

	inline void hook32(uintptr_t address, uintptr_t destination)
	{
		constexpr unsigned char jump_byte = 0xe9;
		mem_copy(address, (uintptr_t)&jump_byte, 1);
		int32_t relative_displacement = calculate_displacement_for_relative_jump(address, destination);
		mem_copy(address + 1, (uintptr_t)&relative_displacement, sizeof(uintptr_t));
	}

	inline void hook64(uintptr_t address, uintptr_t destination)
	{
		std::vector<unsigned char> jump_bytes = { 0xff, 0x25, 0x00, 0x00, 0x00, 0x00 };
		mem_copy(address, (uintptr_t)&jump_bytes[0], jump_bytes.size());
		mem_copy((address + 6), (uintptr_t)&destination, sizeof(uintptr_t));
	}

	inline int32_t calculate_displacement_for_relative_jump(uintptr_t relative_jump_address, uintptr_t destination_address)
	{
		constexpr size_t size_of_e9_jump = 5;
		return -int32_t(relative_jump_address + size_of_e9_jump - destination_address);
	}

	inline uintptr_t relative_to_absolute_address(uintptr_t relative_address_location)
	{
		uintptr_t absolute_address = 0;
		intptr_t relative_address = 0;
		mem_copy((uintptr_t)&relative_address, relative_address_location, 4);
		absolute_address = relative_address_location + 4 + relative_address;
		return absolute_address;
	}

	inline bool keys_pressed(std::vector<std::string> key_combination, bool return_true_while_held = false)
	{
		static std::vector<std::vector<std::string>> previously_pressed_combinations;
		static bool should_attempt_to_get_window_handle = true;

		if (should_attempt_to_get_window_handle)
		{
			get_window_handle();
			should_attempt_to_get_window_handle = false;
		}

		bool input_is_for_another_application = app_window == NULL || app_window != GetForegroundWindow();
		if (input_is_for_another_application)
		{
			return false;
		}

		size_t pressed_key_count = 0;
		for (const auto& key_string : key_combination)
		{
			bool is_windows_virtual_key_code = string_to_keycode_map.find(key_string) != string_to_keycode_map.end();
			if (is_windows_virtual_key_code)
			{
				unsigned short key_code = string_to_keycode_map.at(key_string);
				if (GetAsyncKeyState(key_code))
				{
					pressed_key_count++;
				}
			}
			else
			{
				bool is_xinput_key_code = string_to_xinput_keycode_map.find(key_string) != string_to_xinput_keycode_map.end();
				if (is_xinput_key_code)
				{
					for (DWORD controller_index = 0; controller_index < XUSER_MAX_COUNT; controller_index++)
					{
						XINPUT_STATE state = { 0 };
						DWORD result = XInputGetState(controller_index, &state);
						if (result == ERROR_SUCCESS) // LOL
						{
							unsigned short key_code = string_to_xinput_keycode_map.at(key_string);
							if ((key_code & state.Gamepad.wButtons) == key_code)
							{
								pressed_key_count++;
							}
						}
					}
				}
				else
				{
					log("Error: Key string does not match a key code");
				}
			}
		}

		auto iterator = std::find(previously_pressed_combinations.begin(), previously_pressed_combinations.end(), key_combination);
		bool combination_was_previously_pressed = iterator != previously_pressed_combinations.end();
		bool all_keys_in_combination_pressed = pressed_key_count == key_combination.size();
		if (all_keys_in_combination_pressed)
		{
			if (combination_was_previously_pressed)
			{
				if (return_true_while_held)
				{
					return true;
				}
			}
			else
			{
				previously_pressed_combinations.push_back(key_combination);
				return true;
			}
		}
		else
		{
			if (combination_was_previously_pressed)
			{
				previously_pressed_combinations.erase(iterator);
			}
		}
		return false;
	}

	inline void get_window_handle()
	{
		if (target_window_name == "NULL")
		{
			log("Target window name not set. Cannot check input.");
			return;
		}

		size_t attempts = 0;
		size_t max_attempts = 1000;
		while (true)
		{
			if (attempts >= max_attempts)
			{
				return;
			}
			if (get_window_handle_by_window_name())
			{
				break;
			}
			else if (get_window_handle_by_enumeration())
			{
				break;
			}
			attempts++;
			Sleep(10);
		}
		char buffer[100];
		GetWindowTextA(app_window, buffer, 100);
		log("Found application window: ", buffer);
	}

	inline bool get_window_handle_by_window_name()
	{
		HWND hwnd = FindWindowExA(NULL, NULL, NULL, target_window_name.c_str());
		DWORD process_id = 0;
		GetWindowThreadProcessId(hwnd, &process_id);
		if (process_id == GetCurrentProcessId())
		{
			app_window = hwnd;
			log("Found window handle by name ", target_window_name);
			return true;
		}
		return false;
	}

	inline bool get_window_handle_by_enumeration()
	{
		EnumWindows(&enum_window_handles, NULL);
		if (app_window != NULL)
		{
			return true;
		}
		return false;
	}

	inline BOOL CALLBACK enum_window_handles(HWND hwnd, LPARAM lParam)
	{
		DWORD process_id = NULL;
		GetWindowThreadProcessId(hwnd, &process_id);
		if (process_id == GetCurrentProcessId())
		{
			char buffer[100];
			GetWindowTextA(hwnd, buffer, 100);
			bool window_name_matches_target_window_name = std::string(buffer).find(target_window_name) != std::string::npos;
			if (window_name_matches_target_window_name)
			{
				log(buffer, " handle selected");
				app_window = hwnd;
				return false;
			}
		}
		return true;
	}
}
