#include <iostream>
#include <queue>
#include <WASAPI/AudioCapture.hpp>
#include <ryulib/AudioIO.hpp>

const int CHANNEL = 1;
const int SAMPLE_RATE = 48000;
const int SAMPLE_SIZE = 4;
const int FRAMES = 5760;

struct AudioBuffer {
    void* data;
    int size;
    AudioBuffer(const void* src, int sz) {
        data = malloc(sz);
        memcpy(data, src, sz);
        size = sz;
    }
    ~AudioBuffer() {
        if (data != nullptr) free(data);
    }
};

int main()
{
    std::cout << "=== 오디오 캡처 프로그램 ===" << std::endl;
    std::cout << "시스템 오디오를 캡처하고 재생합니다." << std::endl << std::endl;

    AudioCaptureOption option;
    option.mic_device_id = -1;
    option.use_system_audio = true;
    option.channels = CHANNEL;
    option.sample_rate = SAMPLE_RATE;
    option.sample_size = SAMPLE_SIZE;
    option.frames = FRAMES;

    std::queue<AudioBuffer*> buffers;
    AudioCapture audioCapture;
    //audioCapture.setMicMute(true);

    AudioOutput audio_output(CHANNEL, SAMPLE_RATE, SAMPLE_SIZE, FRAMES);
    audio_output.open(-1);

    std::cout << "오디오 캡처를 초기화하는 중..." << std::endl;

    int count = 0;
    audioCapture.setOnData([&](const void* sender, const void* data, int size) {
        buffers.push(new AudioBuffer(data, size));
        //std::cout << "\r캡처 중... (버퍼 수: " << buffers.size() << ")     ";
        std::cout.flush();
        });

    audioCapture.start(option);

    std::cout << "\n캡처가 시작되었습니다." << std::endl;
    std::cout << "녹음을 멈추려면 's'를 입력하고 Enter를 누르세요." << std::endl;

    std::string line;
    while (true) {
        std::getline(std::cin, line);
        if (line == "s") break;
        std::cout << "녹음을 멈추려면 's'를 입력하고 Enter를 누르세요." << std::endl;
    }

    std::cout << "\n캡처를 중지하는 중..." << std::endl;
    audioCapture.stop();

    std::cout << "캡처된 오디오를 재생합니다..." << std::endl;
    std::cout << "총 버퍼 수: " << buffers.size() << std::endl;

    while (!buffers.empty()) {
        AudioBuffer* buffer = buffers.front();
        buffers.pop();
        audio_output.play(buffer->data, buffer->size);
        delete buffer;

        while (audio_output.getDelayCount() > 3) Pa_Sleep(10);

        std::cout << "\r재생 중... (남은 버퍼: " << buffers.size() << ")     ";
        std::cout.flush();
    }

    std::cout << "\n\n재생이 완료되었습니다." << std::endl;
    std::cout << "프로그램을 종료합니다." << std::endl;

    return 0;
}