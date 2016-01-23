/*

TCP DTMF decoder

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

// $Revision: 3245 $ $Date:: 2016-01-22 #$ $Author: serge $

#ifndef TCP_DTMF_DETECTOR_H
#define TCP_DTMF_DETECTOR_H

namespace dtmf
{
class IDtmfDetectorCallback;
}

namespace tcp_dtmf_detector
{

class TcpDtmfDetectorImpl;

class TcpDtmfDetector
{
public:
    TcpDtmfDetector();
    ~TcpDtmfDetector();

    bool init( dtmf::IDtmfDetectorCallback * callback, unsigned short port );
    void shutdown();
    void worker();

private:

private:

    TcpDtmfDetectorImpl * impl_;
};

} // namespace tcp_dtmf_detector

#endif  // TCP_DTMF_DETECTOR_H
