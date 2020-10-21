#include "utils.hpp"

//resources: https://github.com/HeathHowren/Pattern-Scanning
//https://fearlessrevolution.com/viewtopic.php?t=13926


int main() {
	
    std::cout << "By kemo#1337." << std::endl;

    while (PID == 0)
    {
        PID = GetProcId("GenshinImpact.exe");
        Sleep(10);
    }
    std::cout << "ProcessID: " << PID << std::endl;

    MODULEENTRY32 userdll = { 0 };
    while (modBase == 0)
    {

        userdll = GetModule(PID, "UserAssembly.dll");
        modBase = (ULONG)userdll.modBaseAddr;
        Sleep(10);
    }

    process_handle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, false, PID);

    DWORD* noskillcd_code[3] = { nullptr, nullptr, nullptr };

    noskillcd_code[0] = (DWORD*)find_pattern(userdll, cheats::no_skill_cd_1);
    noskillcd_code[1] = (DWORD*)find_pattern(userdll, cheats::no_skill_cd_2);
    noskillcd_code[2] = (DWORD*)find_pattern(userdll, cheats::no_skill_cd_3);

    DWORD* boxesp = (DWORD*)find_pattern(userdll, cheats::box_esp);
    DWORD* boxespdist = (DWORD*)find_pattern(userdll, cheats::box_esp_dist);
    DWORD* stamina = (DWORD*)find_pattern(userdll, cheats::inf_stamina);
    DWORD* rapid_atk = (DWORD*)find_pattern(userdll, cheats::rapid_atk);


    std::cout << "Got all patterns" << std::endl;

    while (true)
    {
        if (GetAsyncKeyState(VK_F6) < 0)
        {
        	if(!is_no_skill_cd)
        	{
                for (size_t i = 0; i < 3; i++)
                {
                    NopEx(noskillcd_code[i], 4);
                }
                is_no_skill_cd = !is_no_skill_cd;
        	}
            else
            {
                WriteProcessMemory(process_handle, noskillcd_code[0], cheats::no_skill_cd_1_r, 4, NULL);
                WriteProcessMemory(process_handle, noskillcd_code[1], cheats::no_skill_cd_2_r, 4, NULL);
                WriteProcessMemory(process_handle, noskillcd_code[2], cheats::no_skill_cd_3_r, 4, NULL);
                is_no_skill_cd = !is_no_skill_cd;
            }
            Sleep(200);
        }
    	else if (GetAsyncKeyState(VK_F7) < 0)
        {
	        if(!is_box_esp)
	        {
                WriteProcessMemory(process_handle, boxesp, cheats::box_esp_bytes, 2, NULL);
                WriteProcessMemory(process_handle, boxespdist, cheats::box_esp_dist_bytes, 2, NULL);
                is_box_esp = !is_box_esp;
	        }
            else
            {
                WriteProcessMemory(process_handle, boxesp, cheats::box_esp_bytes_r, 2, NULL);
                WriteProcessMemory(process_handle, boxespdist, cheats::box_esp_dist_bytes_r, 2, NULL);
                is_box_esp = !is_box_esp;
            }
            Sleep(200);
        }
        else if (GetAsyncKeyState(VK_F8) < 0)
        {
            if (!is_stamina)
            {
                NopEx(stamina, 5);
            	is_stamina = !is_stamina;
            }
            else
            {
                WriteProcessMemory(process_handle, stamina, cheats::inf_stamina_bytes, 5, NULL);
                is_stamina = !is_stamina;
            }
            Sleep(200);
        }
        else if (GetAsyncKeyState(VK_F9) < 0)
        {
            if (!is_rapid_atk)
            {
                NopEx(rapid_atk, 4);
                is_rapid_atk = !is_rapid_atk;
            }
            else
            {
                WriteProcessMemory(process_handle, rapid_atk, cheats::rapid_atk_bytes, 4, NULL);
                is_rapid_atk = !is_rapid_atk;
            }
            Sleep(200);
        }
        Sleep(10);
    }
    return 0;
}