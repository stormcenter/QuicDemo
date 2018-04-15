// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// A toy client, which connects to a specified port and sends QUIC
// request to that endpoint.

#ifndef NET_TOOLS_QUIC_QUIC_AL_CLIENT_H_
#define NET_TOOLS_QUIC_QUIC_AL_CLIENT_H_

#include <stddef.h>

#include <memory>
#include <string>

#include "base/command_line.h"
#include "base/macros.h"
#include "net/base/ip_address.h"
#include "net/base/ip_endpoint.h"
#include "net/http/http_response_headers.h"
#include "net/log/net_log.h"
#include "net/quic/chromium/quic_chromium_packet_reader.h"
#include "net/quic/chromium/quic_chromium_connection_helper.h"
#include "net/quic/chromium/quic_chromium_alarm_factory.h"
#include "net/quic/core/quic_config.h"
#include "net/quic/core/quic_spdy_stream.h"
#include "net/quic/platform/impl/quic_chromium_clock.h"
#include "quic_client_message_loop_network_helper.h"
#include "quic_client_base.h"
#include "quic_al_client_session.h"

namespace net {

class QuicChromiumAlarmFactory;
class QuicChromiumConnectionHelper;

namespace test {
class QuicClientPeer;
}  // namespace test

class QuicALClient : public QuicClientBase {


 class QuicDataToResend {
   public:
    // |headers| may be null, since it's possible to send data without headers.
    QuicDataToResend(QuicStringPiece body,
                     bool fin);

    virtual ~QuicDataToResend();

    // Must be overridden by specific classes with the actual method for
    // re-sending data.
    virtual void Resend() = 0;

   protected:
    QuicStringPiece body_;
    bool fin_;

   private:
    DISALLOW_COPY_AND_ASSIGN(QuicDataToResend);
 };


 public:
  // Create a quic client, which will have events managed by the message loop.
  QuicALClient(QuicSocketAddress server_address,
                   const QuicServerId& server_id,
                   const ParsedQuicVersionVector& supported_versions,
                   std::unique_ptr<ProofVerifier> proof_verifier);

  ~QuicALClient() override;

 private:
  friend class net::test::QuicClientPeer;

  //  Used by |helper_| to time alarms.
  QuicChromiumClock clock_;

  // Keeps track of any data that must be resent upon a subsequent successful
  // connection, in case the client receives a stateless reject.
  std::vector<std::unique_ptr<QuicDataToResend>> data_to_resend_on_connect_;

 public:
  QuicChromiumConnectionHelper* CreateQuicConnectionHelper();

  QuicChromiumAlarmFactory* CreateQuicAlarmFactory();

  std::unique_ptr<QuicSession> CreateQuicClientSession(
      QuicConnection* connection) override;

  void ResendSavedData() override;

  void ClearDataToResend() override;

  void SendRequest(QuicStringPiece body,
                             bool fin);

  QuicConnectionId GenerateNewConnectionId() override;

  void SendRequestAndWaitForResponse(
      QuicStringPiece body,
      bool fin);

  QuicALClientSession* client_session();

protected:
  int GetNumSentClientHellosFromSession() override;
  int GetNumReceivedServerConfigUpdatesFromSession() override;

  DISALLOW_COPY_AND_ASSIGN(QuicALClient);
};

}  // namespace net

#endif  // NET_TOOLS_QUIC_QUIC_AL_CLIENT_H_
