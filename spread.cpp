PVOID(*CalculateSpread)(PVOID, float*, float*) = nullptr;
PVOID calculateSpreadCaller = nullptr;
float originalReloadTime = 0.0f;
PVOID(*GetWeaponStats)(PVOID) = nullptr;

PVOID CalculateSpreadHook(PVOID arg0, float* arg1, float* arg2) {
	if (originalReloadTime != 0.0f) {
		auto localPlayerWeapon = mem->read<uintptr_t>(LocalPawn, Offsets::CurrentWeapon);
		if (localPlayerWeapon) {
			auto stats = (GetWeaponStats, (PVOID)localPlayerWeapon);
			if (stats) {
				*reinterpret_cast<float*>(reinterpret_cast<PBYTE>(stats) + Offsets::ReloadTime) = originalReloadTime;
				originalReloadTime = 0.0f;
			}
		}
	}

	if (bNoSpread && _ReturnAddress() == calculateSpreadCaller) {
		return 0;
	}

	return (CalculateSpread, arg0, arg1, arg2);
}

void hookthisshit()
{
  addr = Scanners::PatternScan(xorthis("48 83 EC 58 48 8B 91 ? ? ? ? 48 85 D2 0F 84 ? ? ? ? F6 81 ? ? ? ? ? 74 10 48 8B 81 ? ? ? ? 48 85 C0 0F 85 ? ? ? ? 48 8B 8A ? ? ? ? 48 89 5C 24 ? 48 8D 9A ? ? ? ? 48 85 C9"));
	GetWeaponStats = reinterpret_cast<decltype(GetWeaponStats)>(addr);

	addr = Scanners::PatternScan(xorthis("83 79 78 ? 4C 8B C9 75 0F 0F 57 C0 C7 02 ? ? ? ? F3 41 0F 11 ? C3 48 8B 41 70 8B 48 04 89 0A 49 63 41 78 48 6B C8 1C 49 8B 41 70 F3 0F 10 44 01 ? F3 41 0F 11 ? C3"));
	SpoofCall(DiscordHelper::InsertHook, addr, (uintptr_t)CalculateSpreadHook, (uintptr_t)&CalculateSpread);

	addr = Scanners::PatternScan(xorthis("0F 57 D2 48 8D 4C 24 ? 41 0F 28 CD E8 ? ? ? ? 48 8B 4D B0 0F 28 F0"));
	calculateSpreadCaller = (PVOID)addr;
}
