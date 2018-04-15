// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "quic_al_client.h"

#include <utility>

#include "base/logging.h"
#include "base/run_loop.h"
#include "base/threading/thread_task_runner_handle.h"
#include "net/base/net_errors.h"
#include "net/http/http_request_info.h"
#include "net/http/http_response_info.h"
#include "net/log/net_log_source.h"
#include "net/log/net_log_with_source.h"
#include "net/quic/chromium/quic_chromium_alarm_factory.h"
#include "net/quic/chromium/quic_chromium_connection_helper.h"
#include "net/quic/chromium/quic_chromium_packet_reader.h"
#include "net/quic/chromium/quic_chromium_packet_writer.h"
#include "net/quic/core/crypto/quic_random.h"
#include "net/quic/core/quic_connection.h"
#include "net/quic/core/quic_packets.h"
#include "net/quic/core/quic_server_id.h"
#include "net/quic/core/spdy_utils.h"
#include "net/quic/platform/api/quic_flags.h"
#include "net/quic/platform/api/quic_ptr_util.h"
#include "net/socket/udp_client_socket.h"
#include "net/spdy/chromium/spdy_http_utils.h"
#include "net/spdy/core/spdy_header_block.h"

using std::string;

namespace net {


QuicALClient::QuicDataToResend::~QuicDataToResend() = default;

QuicALClient::QuicALClient(QuicSocketAddress server_address,
                       const QuicServerId& server_id,
                       const ParsedQuicVersionVector& supported_versions,
                       std::unique_ptr<ProofVerifier> proof_verifier)
    : QuicClientBase(
          server_id,
          supported_versions,
          QuicConfig(),
          CreateQuicConnectionHelper(),
          CreateQuicAlarmFactory(),
          QuicWrapUnique(
              new QuicClientMessageLooplNetworkHelper(&clock_, this)),
          std::move(proof_verifier)) {
    
    set_server_address(server_address);
    }

QuicALClient::~QuicALClient() = default;

QuicChromiumConnectionHelper* QuicALClient::CreateQuicConnectionHelper() {
  return new QuicChromiumConnectionHelper(&clock_, QuicRandom::GetInstance());
}

QuicChromiumAlarmFactory* QuicALClient::CreateQuicAlarmFactory() {
  return new QuicChromiumAlarmFactory(base::ThreadTaskRunnerHandle::Get().get(),
                                      &clock_);
}


std::unique_ptr<QuicSession> QuicALClient::CreateQuicClientSession(
      QuicConnection* connection){


    return QuicMakeUnique<QuicALClientSession>(*config(), connection,
                                               server_id(), crypto_config());
}

QuicALClientSession* QuicALClient::client_session() {
  return static_cast<QuicALClientSession*>(QuicClientBase::session());
}


void QuicALClient::SendRequest(QuicStringPiece body,
                             bool fin) {
  QuicALClientStream* stream = client_session()->CreateOutgoingDynamicStream();
  if (stream == nullptr) {
    QUIC_BUG << "stream creation failed!";
    return;
  }
  stream->SendData(body, fin);
}

QuicConnectionId QuicALClient::GenerateNewConnectionId(){
  return QuicRandom::GetInstance()->RandUint64();
}


void QuicALClient::SendRequestAndWaitForResponse(
      QuicStringPiece body,
      bool fin) {
    SendRequest(body, fin);
    while (WaitForEvents()) {
    }
}

int QuicALClient::GetNumSentClientHellosFromSession() {
  return client_session()->GetNumSentClientHellos();
}

int QuicALClient::GetNumReceivedServerConfigUpdatesFromSession() {
  return client_session()->GetNumReceivedServerConfigUpdates();
}

QuicALClient::QuicDataToResend::QuicDataToResend(
    QuicStringPiece body,
    bool fin)
    : body_(body), fin_(fin) {}



void QuicALClient::ResendSavedData() {
  // Calling Resend will re-enqueue the data, so swap out
  //  data_to_resend_on_connect_ before iterating.
  std::vector<std::unique_ptr<QuicDataToResend>> old_data;
  old_data.swap(data_to_resend_on_connect_);
  for (const auto& data : old_data) {
    data->Resend();
  }
}

void QuicALClient::ClearDataToResend() {
  data_to_resend_on_connect_.clear();
}


}  // namespace net
