/*

TCP Data Receiver example

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

// $Revision: 3246 $ $Date:: 2016-01-22 #$ $Author: serge $

#include <iostream>
#include <thread>               // std::thread
#include <functional>           // std::bind
#include <sstream>              // std::stringstream

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
        if( argc != 2 )
        {
            std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
            return 1;
        }

        Callback r;

        tcp_dtmf_detector::TcpDtmfDetector s;

        s.init( &r, atoi( argv[1] ) );

        auto t = std::thread( std::bind( &tcp_dtmf_detector::TcpDtmfDetector::worker, &s ) );

        t.join();

    } catch( std::exception& e )
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
