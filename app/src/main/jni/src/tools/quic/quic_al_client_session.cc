// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quic_al_client_session.h"

#include "net/log/net_log_with_source.h"
#include "net/quic/chromium/crypto/proof_verifier_chromium.h"
#include "net/quic/core/crypto/crypto_protocol.h"
#include "net/quic/core/quic_server_id.h"
#include "net/quic/platform/api/quic_bug_tracker.h"
#include "net/quic/platform/api/quic_logging.h"
#include "net/quic/platform/api/quic_ptr_util.h"
#include "net/quic/platform/api/quic_string.h"

namespace net {

QuicALClientSession::QuicALClientSession(
    const QuicConfig& config,
    QuicConnection* connection,
    const QuicServerId& server_id,
    QuicCryptoClientConfig* crypto_config)
  : QuicSession(connection, nullptr, config),
      server_id_(server_id),
      crypto_config_(crypto_config),
      respect_goaway_(true) {}

QuicALClientSession::~QuicALClientSession() = default;

void QuicALClientSession::Initialize() {
  crypto_stream_ = CreateQuicCryptoStream();
  QuicSession::Initialize();
  CryptoConnect();
}

QuicALClientStream* QuicALClientSession::CreateOutgoingDynamicStream() {
  /*if (!ShouldCreateOutgoingDynamicStream()) {
    return nullptr;
  }*/
  std::unique_ptr<QuicALClientStream> stream = CreateClientStream();
  QuicALClientStream* stream_ptr = stream.get();
  ActivateStream(std::move(stream));
  return stream_ptr;
}

std::unique_ptr<QuicALClientStream>
QuicALClientSession::CreateClientStream() {
  return QuicMakeUnique<QuicALClientStream>(GetNextOutgoingStreamId(), this);
}

QuicCryptoStream* QuicALClientSession::GetMutableCryptoStream() {
  return crypto_stream_.get();
}

const QuicCryptoStream* QuicALClientSession::GetCryptoStream()
    const {
  return crypto_stream_.get();
}

void QuicALClientSession::CryptoConnect() {
  DCHECK(flow_controller());
  crypto_stream_->CryptoConnect();
}

int QuicALClientSession::GetNumSentClientHellos() const {
  return crypto_stream_->num_sent_client_hellos();
}

int QuicALClientSession::GetNumReceivedServerConfigUpdates() const {
  return crypto_stream_->num_scup_messages_received();
}
/*
bool QuicALClientSession::ShouldCreateIncomingDynamicStream(QuicStreamId id) {
  if (!connection()->connected()) {
    QUIC_BUG << "ShouldCreateIncomingDynamicStream called when disconnected";
    return false;
  }
  if (goaway_received() && respect_goaway_) {
    QUIC_DLOG(INFO) << "Failed to create a new outgoing stream. "
                    << "Already received goaway.";
    return false;
  }
  if (id % 2 != 0) {
    QUIC_LOG(WARNING) << "Received invalid push stream id " << id;
    connection()->CloseConnection(
        QUIC_INVALID_STREAM_ID, "Server created odd numbered stream",
        ConnectionCloseBehavior::SEND_CONNECTION_CLOSE_PACKET);
    return false;
  }
  return true;
}

*/

QuicStream* QuicALClientSession::CreateIncomingDynamicStream(
    QuicStreamId id) {
  QuicALClientStream* stream = new QuicALClientStream(id, this);
  stream->CloseWriteSide();
  ActivateStream(QuicWrapUnique(stream));
  return stream;
}

std::unique_ptr<QuicCryptoClientStream>
QuicALClientSession::CreateQuicCryptoStream() {
  return QuicMakeUnique<QuicCryptoClientStream>(server_id_, this, new ProofVerifyContextChromium(0, NetLogWithSource()),crypto_config_, this );
}

bool QuicALClientSession::IsAuthorized(const QuicString& authority) {
  return true;
}

void QuicALClientSession::OnProofValid(
    const QuicCryptoClientConfig::CachedState& /*cached*/) {}

void QuicALClientSession::OnProofVerifyDetailsAvailable(
    const ProofVerifyDetails& /*verify_details*/) {}

}
