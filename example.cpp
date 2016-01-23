/*

TCP DTMF detector example

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

// $Revision: 3260 $ $Date:: 2016-01-23 #$ $Author: serge $

#include <iostream>
#include <thread>               // std::thread
#include <functional>           // std::bind

#include "tcp_dtmf_detector.h"  // tcp_dtmf_detector
#include "../dtmf_detector/IDtmfDetectorCallback.hpp"   // IDtmfDetectorCallback

class Callback: public dtmf::IDtmfDetectorCallback
{
public:

    virtual void on_detect( dtmf::tone_e button )
    {
        std::cout << "detected '" << static_cast<uint32_t>( button ) << "'" << std::endl;
    }
};

int main( int argc, char* argv[] )
{
    try
    {
        if( argc != 2 && argc != 3 )
        {
            std::cerr << "Usage: " << argv[0] << " <port> <sampling_rate>" << std::endl;
            return 1;
        }

        uint16_t port = atoi( argv[1] );
        uint32_t rate = 8000;

        if( argc == 3 )
        {
            rate = atoi( argv[2] );
        }

        std::cout << "listening on port " << port << ", sampling rate " << rate << std::endl;

        Callback r;

        tcp_dtmf_detector::TcpDtmfDetector s( rate );

        s.init( &r, port );

        auto t = std::thread( std::bind( &tcp_dtmf_detector::TcpDtmfDetector::worker, &s ) );

        t.join();

    } catch( std::exception& e )
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
