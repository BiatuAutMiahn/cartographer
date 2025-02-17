#pragma once


void InitH2Startup();
void H2DedicatedServerStartup();
void DeinitH2Startup();

// use only after initLocalAppData has been called
// by default useAppDataLocalPath is set to true, if not specified
inline std::wstring prepareLogFileName(std::wstring logFileName, bool useAppDataLocalPath = true);

extern bool H2IsDediServer;
extern DWORD H2BaseAddr;
extern wchar_t* H2ProcessFilePath;
extern wchar_t* H2AppDataLocal;
extern wchar_t* FlagFilePathConfig;
