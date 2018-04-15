// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_QUIC_QUIC_AL_CLIENT_STREAM_H_
#define NET_TOOLS_QUIC_QUIC_AL_CLIENT_STREAM_H_

#include <stddef.h>
#include <sys/types.h>

#include "base/macros.h"
#include "net/quic/core/quic_packets.h"
#include "net/quic/core/quic_stream.h"
#include "net/quic/platform/api/quic_string.h"
#include "net/quic/platform/api/quic_string_piece.h"


namespace net {

// All this does right now is send an SPDY request, and aggregate the
// SPDY response.
class QuicALClientSession;
class QuicALClientStream : public QuicStream {
 public:
  QuicALClientStream(QuicStreamId id, QuicALClientSession* session);
  ~QuicALClientStream() override;

  bool HasBytesToRead() const;
  virtual int GetReadableRegions(iovec* iov, size_t iov_len) const;

  // QuicStream implementation called by the session when there's data for us.
  void OnDataAvailable() override;
  void MarkConsumed(size_t num_bytes);

  // Serializes the headers and body, sends it to the server, and
  // returns the number of bytes sent.
  size_t SendData(QuicStringPiece body, bool fin);

  // Returns the response data.
  const QuicString& data() { return data_; }

  // While the server's SetPriority shouldn't be called externally, the creator
  // of client-side streams should be able to set the priority.
  using QuicStream::SetPriority;
  using QuicStream::CanWriteNewData;
  using QuicStream::CloseWriteSide;
  using QuicStream::fin_buffered;
  using QuicStream::OnClose;
  using QuicStream::set_ack_listener;
  using QuicStream::WriteMemSlices;
  using QuicStream::WriteOrBufferData;
  using QuicStream::WritevData;

 private:

  QuicString data_;

  //QuicALClientSession* session_;

  DISALLOW_COPY_AND_ASSIGN(QuicALClientStream);
};

}  // namespace net

#endif

