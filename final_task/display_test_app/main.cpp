#include <memory>
#include <chrono>
#include <thread>
#include <string>
#include <filesystem>
#include <cstddef>
#include <vector>

#include <fcntl.h>
#include <unistd.h>

inline constexpr std::uint16_t COLOR_BLACK = 0x0000;
inline constexpr std::uint16_t COLOR_BLUE = 0x001F;
inline constexpr std::uint16_t COLOR_RED = 0xF800;
inline constexpr std::uint16_t COLOR_GREEN = 0x07E0;
inline constexpr std::uint16_t COLOR_CYAN = 0x07FF;
inline constexpr std::uint16_t COLOR_MAGENTA = 0xF81F;
inline constexpr std::uint16_t COLOR_YELLOW = 0xFFE0;
inline constexpr std::uint16_t COLOR_WHITE = 0xFFFF;

constexpr std::string_view kFrameBufferPath = "/dev/gc9_framebuffer_0";

struct FileRaiiGuard {
	FileRaiiGuard(std::string_view filePath)
	{
		if (!std::filesystem::exists(filePath)) {
			throw std::runtime_error(
				"Bad file path for the FileRaiiGuard");
		}
		m_fileHandle = open(filePath.data(), O_WRONLY);

		if (m_fileHandle == kInvalidFileHandle) {
			throw std::runtime_error(
				"Failed to open the given file");
		}
	}
	FileRaiiGuard &operator=(const FileRaiiGuard &) = delete;
	FileRaiiGuard(const FileRaiiGuard &) = delete;

	~FileRaiiGuard()
	{
		if (m_fileHandle != kInvalidFileHandle) {
			close(m_fileHandle);
		}
	}

    public:
	void writeToFile(const std::uint8_t *dataSequence,
			 std::size_t dataSequenceSize)
	{
		write(m_fileHandle, dataSequence, dataSequenceSize);
	}

    private:
	static constexpr inline std::int32_t kInvalidFileHandle = -1;

    private:
	int m_fileHandle;
};

template <std::size_t DisplayWidth, std::size_t DisplayHeight>
class FramebufferHolder {
    public:
	FramebufferHolder() : m_framebuffer{}
	{
		m_framebuffer.resize(kDisplayFramebufferSize);
	}

	void fillWithColor(std::uint16_t color)
	{
		std::fill(m_framebuffer.begin(), m_framebuffer.end(), color);
	}

	const std::uint8_t *getRaw()
	{
		return reinterpret_cast<const std::uint8_t *>(m_framebuffer.data());
	}

    private:
	static constexpr inline std::size_t kDisplayWidth = DisplayWidth;
	static constexpr inline std::size_t kDisplayHeight = DisplayHeight;
	static constexpr inline std::size_t kDisplayFramebufferSize =
		kDisplayWidth * kDisplayHeight;

    private:
	std::vector<std::uint16_t> m_framebuffer;
};

int main()
{
	FileRaiiGuard fileWrapper{ kFrameBufferPath };
	constexpr std::size_t kDisplayWidth = 320;
	constexpr std::size_t kDisplayHeight = 240;

	FramebufferHolder<kDisplayWidth, kDisplayHeight> frameBuffer;
	constexpr std::size_t kDisplayFramebufferSizeRaw =
		kDisplayWidth * kDisplayHeight * sizeof(std::uint16_t);

	frameBuffer.fillWithColor(COLOR_BLUE);
	fileWrapper.writeToFile(frameBuffer.getRaw(),
				kDisplayFramebufferSizeRaw);

	return 0;
}