﻿#pragma once

#include "FileModes.h"
#include "hzpch.h"

struct FileInfo
{
    std::string m_Name;

    size_t m_Size = 0;

    bool m_ReadOnly = false;

    time_t m_LastEditTime = 0;

    time_t m_CreateTime = 0;
};

class DirectorySystem
{
public:
    // File Pos
    static const char *PrintError(int errorCode);
    static bool SeekFile(HANDLE file, int64 offset, FilePosMode mode);
    static bool GetFilePos(HANDLE file, int64 &pos);
    static bool SetFilePos(HANDLE file, const int64 pos);

    // Read, Save, Open, Close File
    static HANDLE CreateFiile(const char *dest);
    static HANDLE OpenFile(const char *path,
                           FileAccessMode accessFlag,
                           FileOpenMode modeFlag);
    static bool CloseFile(HANDLE handle);
    static int64 ReadFromFile(HANDLE file, void *buffer, uint64 nbytes);
    static int64 WriteToFile(HANDLE file, const void *buffer, uint64 nbytes);
    static bool FlushFile(HANDLE file);
    static bool IsFileEOF(HANDLE file);


    // Combine Path
    static std::string CombinePath(const char *a, const char *b);

    // 순소 폴더 경로 리턴
    static std::string GetPathDirectory(const char *path);

    // 확장자 리턴
    static std::string GetExtenstion(const char *path);

    // win system 에 맞게 파일 경로를 변경해준다.
    static std::string ToWinSystemPath(const std::string &str);

    static void ToWinSystemPath(char *after, const char *before);

    /**
	 * system path에서 char(utf8 nfc) path로 변환
	 * 
	 * 혹은, WBC  를 MBC 로 변환
	 */
    static void FromSystemToUTF8Path(char *aft_dest_utf8,
                                     const void *bef_src_sys);

    /**
	 * char(utf8) path에서 os system path로 변환. 즉 MBC 을 WBC 로 변환하는 것이다.
	 * @param need_safe_conversion utf-8이 아닌 ANSI type까지 처리 (only for window)
	 */
    static void FromUTF8ToSystemPath(void *dest_sys,
                                     const char *src_utf8,
                                     bool need_safe_conversion = false);
};