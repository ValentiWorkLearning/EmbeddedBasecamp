#include <array>
#include <charconv>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include <fcntl.h>
#include <fstream>
#include <unistd.h>

struct file_deleter
{
    void operator()(std::FILE* fp)
    {
        printf("Close file descriptor\n");
        std::fclose(fp);
    }
};

using TSysFsFile = std::unique_ptr<std::FILE, file_deleter>;

constexpr inline std::string_view kExportString = "/sys/class/gpio/export";
constexpr inline std::string_view kUnexportString = "/sys/class/gpio/unexport";
constexpr inline std::string_view kConcreteGpioBegin = "/sys/class/gpio/gpio";
constexpr inline int kInvalidFileHandle = -1;
enum class Direction
{
    In,
    Out
};

struct PortWrapper
{

    PortWrapper(int portNumber, Direction direction)
        : m_portNumber(portNumber)
        , m_direction{direction}
        , m_valueFileHandle{kInvalidFileHandle}
        , m_portStr{}
        , m_portString{}
        , m_currentButtonState{}
    {
        {
            TSysFsFile pExportFile{fopen(kExportString.data(), "w")};

            if (!pExportFile)
                throw std::logic_error("Can't open sysfs for gpio exports");

            if (auto [ptr, ec] = std::to_chars(
                    m_portStr.data(), m_portStr.data() + m_portStr.size(), m_portNumber);
                ec == std::errc())
            {
                m_portString = std::string_view(m_portStr.data(), ptr - m_portStr.data());
                fprintf(pExportFile.get(), m_portString.data());
                fflush(pExportFile.get());
            }
        }
        std::string concretePortString{kConcreteGpioBegin.data()};
        concretePortString += m_portString;

        std::string directionString{concretePortString};
        std::cout << "Direction string is" << directionString << std::endl;
        directionString += "/direction";

        {
            TSysFsFile pDirectionFile{fopen(directionString.c_str(), "wr")};

            if (!pDirectionFile)
                throw std::logic_error("Can't open direction file. Probably the export process was "
                                       "failed!");
            if (direction == Direction::In)
            {
                fprintf(pDirectionFile.get(), "r");
            }
            else
                fprintf(pDirectionFile.get(), "w");

            fflush(pDirectionFile.get());
        }
        {

            std::string valueString{concretePortString};
            std::cout << "valueString string is" << valueString << std::endl;
            valueString += "/value";
            m_valueFileHandle = open(valueString.c_str(), O_RDWR);
        }
    }
    void checkState()
    {
        if (m_direction != Direction::In)
            return;

        char value{};

        lseek(m_valueFileHandle, 0, SEEK_SET);
        read(m_valueFileHandle, &value, sizeof(char));

        updateButtonState(value);
    }

    using TClickObserver = std::function<void()>;
    void subsribeOnClick(TClickObserver clickObserver)
    {
        m_clickObservers.push_back(clickObserver);
    }

    ~PortWrapper()
    {
        if (m_valueFileHandle != kInvalidFileHandle)
            close(m_valueFileHandle);

        TSysFsFile pUnexport{fopen(kUnexportString.data(), "w")};

        if (pUnexport)
        {
            fprintf(pUnexport.get(), m_portString.data());
        }
    }

private:
    static constexpr inline std::size_t kPortLength = 2;

    enum class ButtonState
    {
        Pressed,
        Released
    };

private:
    void updateButtonState(char newPortValue)
    {
        if (newPortValue == '1')
        {
            m_currentButtonState = ButtonState::Pressed;
        }
        else if (newPortValue == '0')
        {
            if (m_currentButtonState.has_value())
            {
                if (m_currentButtonState.value() == ButtonState::Pressed)
                    for (const auto& observer : m_clickObservers)
                    {
                        observer();
                    }
            }
            m_currentButtonState = ButtonState::Released;
        }
    }

private:
    int m_portNumber;
    Direction m_direction;
    int m_valueFileHandle;
    std::optional<ButtonState> m_currentButtonState;
    std::array<char, kPortLength> m_portStr;

    std::vector<TClickObserver> m_clickObservers;
    std::string_view m_portString;
};

int main()
{

    std::uint64_t kClicksCounter{};
    PortWrapper buttonPort(17, Direction::In);
    buttonPort.subsribeOnClick([&kClicksCounter] {
        ++kClicksCounter;
        std::cout << "Current clicks value: " << kClicksCounter << std::endl;
    });
    while (true)
    {
        buttonPort.checkState();
    }
    return 0;
}