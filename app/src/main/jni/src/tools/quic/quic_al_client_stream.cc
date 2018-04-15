// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quic_al_client_stream.h"
#include "iostream"
#include <utility>

#include "net/quic/core/quic_alarm.h"
#include "net/quic/core/quic_client_promised_info.h"
#include "net/quic/core/spdy_utils.h"
#include "net/quic/platform/api/quic_logging.h"
#include "net/spdy/core/spdy_protocol.h"
#include "quic_al_client_session.h"

using std::string;

namespace net {

QuicALClientStream::QuicALClientStream(QuicStreamId id,
                                           QuicALClientSession* session)
  : QuicStream(id, session, false){}

QuicALClientStream::~QuicALClientStream() = default;


int QuicALClientStream::GetReadableRegions(iovec* iov, size_t iov_len) const {
  return sequencer()->GetReadableRegions(iov, iov_len);
}

void QuicALClientStream::MarkConsumed(size_t num_bytes) {
  return sequencer()->MarkConsumed(num_bytes);
}
void QuicALClientStream::OnDataAvailable() {
  // For push streams, visitor will not be set until the rendezvous
  // between server promise and client request is complete.
 /* if (visitor() == nullptr)
    return;
*/
  while (HasBytesToRead()) {
    struct iovec iov;
    if (GetReadableRegions(&iov, 1) == 0) {
      // No more data to read.
      break;
    }

    QUIC_DVLOG(1) << "Client processed " << iov.iov_len << " bytes for stream "
                  << id();

    data_.append(static_cast<char*>(iov.iov_base), iov.iov_len);

    MarkConsumed(iov.iov_len);
  }

  std::cout << "received: " << data_ << std::endl;
  if (sequencer()->IsClosed()) {
    OnFinRead();
  } else {
    sequencer()->SetUnblocked();
  }
}

size_t QuicALClientStream::SendData(
    QuicStringPiece body,
    bool fin) {
  size_t bytes_sent = body.size();

  if (!body.empty()) {
    std::cout << "send: " << body << std::endl;
    WriteOrBufferData(body, fin, nullptr);
  }

  //std::cout << body << "  debugg 1111 " << bytes_sent << std::endl;
  return bytes_sent;
}

bool QuicALClientStream::HasBytesToRead() const {
  return sequencer()->HasBytesToRead();
}

}
