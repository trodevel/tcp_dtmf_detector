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

// $Revision: 3254 $ $Date:: 2016-01-23 #$ $Author: serge $

#include "tcp_dtmf_detector.h"      // self

#include <vector>

#include "../tcp_data_receiver/i_data_receiver.h"       // IDataReceiver
#include "../tcp_data_receiver/tcp_data_receiver.h"     // TcpDataReceiver
#include "stream_dtmf_detector.h"                       // StreamDtmfDetector
#include "../utils/dummy_logger.h"      // dummy_log

#define MODULENAME      "TcpDtmfDetector"

namespace tcp_dtmf_detector
{

class TcpDtmfDetectorImpl: public tcp_data_receiver::IDataReceiver
{
public:
    TcpDtmfDetectorImpl( uint32_t sampling_rate ):
        detector_( sampling_rate )
    {
    }

    bool init( dtmf::IDtmfDetectorCallback * callback, unsigned short port )
    {
        auto b = serv_.init( this, port );

        if( b == false )
        {
            dummy_log_fatal( MODULENAME, "cannot init server" );
        }

        detector_.init( callback );

        dummy_log_info( MODULENAME, "inited: OK, port %u", port );

        return true;
    }

    void shutdown()
    {
        serv_.shutdown();
    }

    void worker()
    {
        serv_.worker();
    }

    // tcp_data_receiver::IDataReceiver
    virtual void on_received( uint32_t size, const char *data )
    {
        if( size > 0 )
        {
            detector_.process( size, data );
        }
        else
        {
            detector_.reset();
        }
    }

private:

    tcp_data_receiver::Server   serv_;

    StreamDtmfDetector          detector_;
};


TcpDtmfDetector::TcpDtmfDetector( uint32_t sampling_rate )
{
    impl_   = new TcpDtmfDetectorImpl( sampling_rate );
}

TcpDtmfDetector::~TcpDtmfDetector()
{
    delete impl_;
}

bool TcpDtmfDetector::init( dtmf::IDtmfDetectorCallback * callback, unsigned short port )
{
    return impl_->init( callback, port );
}

void TcpDtmfDetector::shutdown()
{
    impl_->shutdown();
}

void TcpDtmfDetector::worker()
{
    impl_->worker();
}

} // namespace tcp_dtmf_detector
