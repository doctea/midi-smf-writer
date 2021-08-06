#define BOOST_TEST_MODULE BasicArduinoTests
#include <boost/test/unit_test.hpp>
#include "default_test_fixture.h"
#include "smfwriter.h"
#include "midireader.h"
BOOST_AUTO_TEST_SUITE(basic_midi_read_test)

    extern unsigned int midi_monophonic_len;
    extern unsigned char midi_monophonic_mid[];


    unsigned int get_microseconds_per_tick(double beats_per_minute) {
        double micros_per_beat = 60000000.0 / beats_per_minute;
        unsigned int micros_per_tick = micros_per_beat / 480;
        return micros_per_tick;
    }

    BOOST_FIXTURE_TEST_CASE(can_read_basic_midi_file, DefaultTestFixture) {

        SD.setSDCardFileData(reinterpret_cast<char *>(midi_monophonic_mid), midi_monophonic_len);

        SmfWriter writer;
        midireader reader;
        reader.open("1234.mid");

        double microsPerTick = get_microseconds_per_tick(120.0);

        int totalNumNotesRead = 0;
        for (int t = 0; t < reader.getNumTracks(); t++)
        {
            reader.setTrackNumber(t);
            midimessage midiMessage {};
            int i = 0;
            long totalTicks = 0;
            long microseconds = 0;
            while (reader.read(midiMessage)) {
                totalTicks += midiMessage.delta_ticks;
                microseconds += microsPerTick * midiMessage.delta_ticks;
                if (midiMessage.isTempoChange) {
                    printf("tempo change: %f\n", midiMessage.tempo);
                } else 
                printf("%5d: [%2d,%4d]: %6d: delta: %3d\tstatus: 0x%2x\tkey: %3d\tvelocity: %3d\tchannel: %2d\t\n",
                       microseconds/1000,
                       t,
                       i,
                       totalTicks,
                       midiMessage.delta_ticks,
                       midiMessage.status,
                       midiMessage.key,
                       midiMessage.velocity,
                       midiMessage.channel);
                i++;
            }
            totalNumNotesRead += i;
        }

        BOOST_CHECK_EQUAL(reader.getNumTracks(), 1);
        BOOST_CHECK_EQUAL(totalNumNotesRead, 33);
    }

    unsigned int midi_monophonic_len = 255;
    unsigned char midi_monophonic_mid[] = {
            0x4d, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01,
            0x01, 0xe0, 0x4d, 0x54, 0x72, 0x6b, 0x00, 0x00, 0x00, 0xe9, 0x00, 0xff,
            0x20, 0x01, 0x00, 0x00, 0xff, 0x03, 0x05, 0x73, 0x79, 0x6e, 0x74, 0x68,
            0x00, 0xff, 0x04, 0x14, 0x53, 0x74, 0x65, 0x69, 0x6e, 0x77, 0x61, 0x79,
            0x20, 0x47, 0x72, 0x61, 0x6e, 0x64, 0x20, 0x50, 0x69, 0x61, 0x6e, 0x6f,
            0x00, 0xff, 0x58, 0x04, 0x04, 0x02, 0x18, 0x08, 0x00, 0xff, 0x59, 0x02,
            0x00, 0x00, 0x00, 0xff, 0x54, 0x05, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00,
            0xff, 0x51, 0x03, 0x0a, 0x2c, 0x2b, 0x00, 0x90, 0x3c, 0x7f, 0x81, 0x1b,
            0x90, 0x3c, 0x00, 0x82, 0x45, 0x90, 0x3c, 0x7f, 0x81, 0x1b, 0x90, 0x3c,
            0x00, 0x82, 0x45, 0x90, 0x3c, 0x7f, 0x81, 0x1b, 0x90, 0x3c, 0x00, 0x82,
            0x45, 0x90, 0x3c, 0x7f, 0x81, 0x1b, 0x90, 0x3c, 0x00, 0x82, 0x45, 0x90,
            0x3c, 0x7f, 0x81, 0x1b, 0x90, 0x3c, 0x00, 0x82, 0x45, 0x90, 0x3c, 0x7f,
            0x81, 0x1b, 0x90, 0x3c, 0x00, 0x82, 0x45, 0x90, 0x3c, 0x7f, 0x81, 0x1b,
            0x90, 0x3c, 0x00, 0x82, 0x45, 0x90, 0x3d, 0x7f, 0x81, 0x1b, 0x90, 0x3d,
            0x00, 0x82, 0x45, 0x90, 0x38, 0x7f, 0x81, 0x1b, 0x90, 0x38, 0x00, 0x82,
            0x45, 0x90, 0x38, 0x7f, 0x81, 0x1b, 0x90, 0x38, 0x00, 0x82, 0x45, 0x90,
            0x38, 0x7f, 0x81, 0x1b, 0x90, 0x38, 0x00, 0x82, 0x45, 0x90, 0x38, 0x7f,
            0x81, 0x1b, 0x90, 0x38, 0x00, 0x82, 0x45, 0x90, 0x38, 0x7f, 0x81, 0x1b,
            0x90, 0x38, 0x00, 0x82, 0x45, 0x90, 0x38, 0x7f, 0x81, 0x1b, 0x90, 0x38,
            0x00, 0x82, 0x45, 0x90, 0x38, 0x7f, 0x81, 0x1b, 0x90, 0x38, 0x00, 0x82,
            0x45, 0x90, 0x37, 0x7f, 0x81, 0x1b, 0x90, 0x37, 0x00, 0x81, 0x98, 0x45,
            0xff, 0x2f, 0x00
    };

BOOST_AUTO_TEST_SUITE_END()