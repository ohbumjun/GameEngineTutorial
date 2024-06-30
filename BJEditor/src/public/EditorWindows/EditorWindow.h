#pragma once
#include "EditorWindows/GuiWindow.h"
#include "Hazel/Core/Reflection/TypeId.h"
#include "Panel/Panel.h"
#include "Hazel/Core/Reflection/Reflection.h"
#include "Hazel/Core/DataStructure/RingBuffer.h"
#include <queue>

namespace HazelEditor
{
	class EditorWindow : public GuiWindow
	{
    public:
        static EditorWindow *GetEditorWindow();
        /**
	 * @brief ���ο��� �����Ǵ� Panel �ν��Ͻ� ����
	 * @details ��Ƽ �����忡�� ȣ�� �� ���� ������ ó���� �ϱ�� ������, ���� �����尡 ����ִ� ��� ������¿� ����.
	 */
        template <typename T>
        inline T *CreatePanel()
        {
            return (T *)CreatePanel(Hazel::Reflection::GetTypeId<T>());
        }

        /**
	 * @brief ���ο��� �����Ǵ� Panel �ν��Ͻ� ����
	 * @details ��Ƽ �����忡�� ȣ�� �� ���� ������ ó���� �ϱ�� ������, ���� �����尡 ����ִ� ��� ������¿� ����.
	 */
        Panel *CreatePanel(const Hazel::TypeId &type);

        template <typename T>
        inline std::vector<T *> FindPanels()
        {
            std::vector<T *> result;
            std::vector<Panel *> finds = FindPanels(Hazel::Reflection::GetTypeId<T>());
            for (uint64 i = 0, max = finds.Count(); i < max; i++)
            {
                result.Add((T *)finds[i]);
            }
            return result;
        }
        std::vector<Panel *> FindPanels(const Hazel::TypeId &type) const;
        Panel *FindPanel(const char *name) const;

        void OpenMessagePopup(const std::string &title, const std::string &message);

        /**
	 * @brief [Thread-safe] ���� �����Ϳ� ���� ����â�� ǥ��
	 */
        void DisplayProgressBar(float progress,
                                const char *title,
                                const char *message,
                                const std::string &key = "");

        void ClearProgressBar(const std::string &key = "");

	private:
        struct ProgressData
        {
            std::string message = "";
            std::string title = "";
            float progress = 0.f;
            bool show = false;

            ProgressData() = default;
            ProgressData(float progress, const char *title, const char *message)
                : progress(progress), title(title), message(message), show(true)
            {
            }

            bool operator==(const ProgressData &other) const
            {
                return progress == other.progress && title == other.title &&
                       message == other.message && show == show;
            }

            bool operator!=(const ProgressData &other) const
            {
                return !operator==(other);
            }

            static const ProgressData Show(float progress,
                                           const char *title,
                                           const char *message)
            {
                return ProgressData(progress, title, message);
            }

            static const ProgressData &Close()
            {
                const static ProgressData close;
                return close;
            }
        };

        void onInit() override;

        void onUpdate(float deltatime) override;

        void onOpen() override;

        void onClose() override;

        void onPrepareGUI() override;

        void onFinishGUI() override;

        void onGUI() override;

        void onNextFrame() override;

        void updateProgressBar(const ProgressData &data);

        PanelController *_panelController = nullptr;

        Popup::LvProgressPanel *_progress = nullptr;

        Hazel::RingBuffer<ProgressData> _progressQueue;
        std::queue<float> _seconds;
	};
};
