/*

Stream DTMF decoder

Copyright (C) 2016 Sergey Kolevatov

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

*/

// $Revision: 3257 $ $Date:: 2016-01-23 #$ $Author: serge $

#ifndef STREAM_DTMF_DETECTOR_H
#define STREAM_DTMF_DETECTOR_H

#include <vector>
#include <mutex>                    // std::mutex

#include "../dtmf_detector/DtmfDetector.hpp"            // dtmf::DtmfDetector

namespace tcp_dtmf_detector
{

class StreamDtmfDetector
{
public:
    StreamDtmfDetector( uint32_t sampling_rate );

    bool init( dtmf::IDtmfDetectorCallback * callback );

    void process( uint32_t size, const char *data );

    void reset();

private:
    void process_buffer();

private:
    mutable std::mutex          mutex_;

    std::vector<char>           buff_;

    dtmf::DtmfDetector          detector_;
};

} // namespace tcp_dtmf_detector

#endif  // STREAM_DTMF_DETECTOR_H
