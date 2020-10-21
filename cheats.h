#include "utils.hpp"

inline BOOL is_no_skill_cd = false;
inline BOOL is_box_esp = false;
inline BOOL is_stamina = false;
inline BOOL is_rapid_atk = false;

class cheats
{
public:
	
	static inline const char* no_skill_cd_1 = "0F 10 7B 20 F6 81 0A 01 00 00 01 74 0E 83 B9 BC ?? ?? ?? ?? 75 05 E8 01";
	static inline const char* no_skill_cd_2 = "0F 11 43 20 0F 28 74 24 50 48 8B 7C 24 78 0F 28 7C 24 40 48 83 C4 60 5B";
	static inline const char* no_skill_cd_3 = "0F 10 70 30 F6 81 0A 01 00 00 01 74 0E 83 B9 BC ?? ?? ?? ?? 75 05 E8 C4 AE 9A 02";
	static inline const byte no_skill_cd_1_r[4] = { 0x0F, 0x10, 0x7B, 0x20 };
	static inline const byte no_skill_cd_2_r[4] = { 0x0F, 0x11, 0x43, 0x20 };
	static inline const byte no_skill_cd_3_r[4] = { 0x0F, 0x10, 0x70, 0x30 };
	
	static inline const char* box_esp = "74 1C 48 85 C0 0F 84 CF";
	static inline const char* box_esp_dist = "74 E2 33 D2 48 8B CF";
	static inline const byte box_esp_bytes[2] = { 0x75, 0x1C };
	static inline const byte box_esp_bytes_r[2] = { 0x74, 0x1C };
	static inline const byte box_esp_dist_bytes[2] = { 0x75, 0xE2 };
	static inline const byte box_esp_dist_bytes_r[2] = { 0x74, 0xE2 };

	static inline const char* inf_stamina = "E8 A7 A1 3E 00";
	static inline const byte inf_stamina_bytes[5] = { 0xE8, 0xA7, 0xA1, 0x3E, 0x00 };

	static inline const char* rapid_atk = "41 0F 11 07 44 88 67 7C";
	static inline const byte rapid_atk_bytes[4] = { 0x41, 0x0F, 0x11, 0x07 };
};