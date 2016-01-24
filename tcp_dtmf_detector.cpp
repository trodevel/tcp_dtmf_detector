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

// $Revision: 3270 $ $Date:: 2016-01-24 #$ $Author: serge $

#include "tcp_dtmf_detector.h"      // self

#include <mutex>                    // std::mutex

#include "../dtmf_detector/DtmfDetector.hpp"            // dtmf::DtmfDetector

#include "../tcp_data_receiver/i_data_receiver.h"       // IDataReceiver
#include "../tcp_data_receiver/tcp_data_receiver.h"     // TcpDataReceiver
#include "../utils/dummy_logger.h"      // dummy_log
#include "../utils/mutex_helper.h"      // MUTEX_SCOPE_LOCK

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

        detector_.init_callback( callback );

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
            if( size > 2 )
            {
                MUTEX_SCOPE_LOCK( mutex_ );

                detector_.process( reinterpret_cast<const int16_t*>( data ), size / 2 );
            }
        }
        else
        {
            MUTEX_SCOPE_LOCK( mutex_ );

            //detector_.reset();    // TODO reset() not implemented yet
        }
    }

private:

    mutable std::mutex          mutex_;

    tcp_data_receiver::Server   serv_;

    dtmf::DtmfDetector          detector_;
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
