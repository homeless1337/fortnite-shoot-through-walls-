Vector3 CalcAim()
{
    Vector3 RetVector = { 0,0,0 };

    if (!entityx) return { 0,0,0 };

    uint64_t currentactormesh = read<uint64_t>(entityx + Offsets::Pawn::Mesh);
    if (!currentactormesh) return { 0,0,0 };

    Vector3 rootHead = GetBoneLocByIdx(currentactormesh, 66);
    if (rootHead.x == 0 && rootHead.y == 0) return Vector3(0, 0, 0);

    Vector3 VectorPos = rootHead - CamLoc;

    float distance = VectorPos.Length();
    RetVector.x = -(((float)acos(VectorPos.z / distance) * (float)(180.0f / M_PI)) - 90.f);
    RetVector.y = (float)atan2(VectorPos.y, VectorPos.x) * (float)(180.0f / M_PI);

    return RetVector;
}

INT GetViewPointHook(PVOID player, FMinimalViewInfo* viewInfo, BYTE stereoPass) {
    const float upperFOV = 50.534008f;
    const float lowerFOV = 40.0f;

    auto ret = GetViewPoint(player, viewInfo, stereoPass);

    auto fov = viewInfo->FOV;
    auto desired = (((180.0f - upperFOV) / (180.0f - 80.0f)) * (Settings.FOV - 80.0f)) + upperFOV;

    if (fov > upperFOV) {
        fov = desired;
    }
    else if (fov > lowerFOV) {
        fov = (((fov - lowerFOV) / (upperFOV - lowerFOV)) * (desired - lowerFOV)) + lowerFOV;
    }

    if (GetAsyncKeyState(VK_RBUTTON)) {
        viewInfo->Rotation = CalcAim();
    }

    // Do whatever you want with viewInfo to change the camera location 
    viewInfo->FOV = fov;

    return ret;
}
