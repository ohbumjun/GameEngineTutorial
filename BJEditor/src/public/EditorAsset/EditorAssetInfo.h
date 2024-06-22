#pragma once

#include "EditorAsset.h"
#include <string>
#include <cstdint>

namespace HazelEditor
{
class EditorAssetInfo
{
public:
    // struct SubEditorAssetInfo : LvArchivable
    struct SubEditorAssetInfo
    {
        // void Serialize(Engine::LvArchive & archive) override;
        // void Deserialize(Engine::LvArchive& archive) override;

        std::string name;
        uint32_t localId = 0;
        uint8_t EditorAssetType = 0;
    };
    friend class EditorAsset;

    EditorAssetInfo() = default;
    EditorAssetInfo(const EditorAssetInfo &rhs) = default;
    EditorAssetInfo &operator=(const EditorAssetInfo &rhs) = default;


    virtual void CopyFrom(const EditorAssetInfo *info)
    {
        // NOTICE 자식 객체를 갖는 부모클래스 포인터로 복사대입연산 수행시(*base = *obj) Base의 operator=이 수행되어 제대로 복사가 안된다.
        // Base* base1 = new Derived();
        // Base* base2 = new Derived();
        // *base1 = *base2;
        // 위와 같은 상황에서 Derived의 operaotr=을 호출하기 위해 2가지 방법이 있다.
        // 1. virtual void CopyFrom(Base* rhs) 정의하여 override
        // 2. virtual Base& operator=(Base& rhs) 정의하여 자식객체는 같은 시그니쳐의 메서드를 override
        // 여기서 1번의 방법을 사용하여 복사한다. (TODO operator=과 하는 일이 같기 때문에 추후에 제거할지 말지 결정필요)

        //LvObject에 관한건 복사하지 않는다. Why? LvObject::Instantiate를 할 때 넣어주기 때문에. 하지만 스택변수로 만들면 위험해진다.
        version = info->version;

        // guid는 복사하지 않는다!
        //guid = info->guid;
    }

    // TODO 비트 연산으로 바꾸자.
    enum class State : uint8_t
    {
        NONE = 0,
        FILE_NOT_EXIST, // .Inf가 존재하지 않음.
        GUID_NOT_MATCH,
        CHECKSUM_NOT_MATCH
    };

    constexpr static const char *FileExtension = ".inf";

    /**
	* EditorAsset info 의 serialize 버전
	*/
    std::string version;

    /**
	* 연결된 EditorAsset의 guid
	*/
    std::string guid;

    /**
	*EditorAssetInfo를 생성합니다.
	* type 생성하려는 EditorAssetInfo Type
	* @param path EditorAsset info의 위치
	*/
    // static EditorAssetInfo* Create(LvTypeId type, const std::string& path);

    /**
	* EditorAsset에 해당하는 .inf를 만듭니다.
	* EditorAsset .inf를 생성할 target EditorAsset
	* @return .Inf 생성 성공여부를 반환합니다.
	*/
    inline static bool Save(const EditorAsset *EditorAsset)
    {
        // if (!(EditorAsset != nullptr && EditorAsset->info != nullptr))
        // {
        // 	return false;
        // }
        // return SaveFile(EditorAsset);
    }
};

} // namespace HazelEditor