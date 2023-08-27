#pragma once

namespace Hazel
{
	// Texture 라는 최상위 Class 를 두는 이유
	// - Texture2D, 3D 등 여부가 관계없이 Texture 라는 것 자체를 사용할 때가 있으므로
	class Texture
	{
	public :
		virtual ~Texture() = default;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual uint32_t GetRendererID() const = 0;
		// data : data to be uploaded to gpu
		virtual void SetData(void* data, uint32_t size) = 0;

		virtual bool operator == (const Texture& other) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);


	};
}



