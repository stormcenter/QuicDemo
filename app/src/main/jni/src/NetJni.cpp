//
// Created by zhangchi09 on 18-3-30.
//
#include <stdio.h>
#include <jni.h>
#include <android/log.h>

#include <iostream>
#include <base/android/jni_android.h>
#include "base/android/jni_utils.h"
#include "base/at_exit.h"
#include "base/command_line.h"
#include "base/logging.h"
#include "base/message_loop/message_loop.h"
#include "base/task_scheduler/task_scheduler.h"
#include "net/base/net_errors.h"
#include "net/base/privacy_mode.h"
#include "net/cert/cert_verifier.h"
#include "net/cert/ct_known_logs.h"
#include "net/cert/ct_log_verifier.h"
#include "net/cert/multi_log_ct_verifier.h"
#include "net/http/transport_security_state.h"
#include "net/quic/chromium/crypto/proof_verifier_chromium.h"
#include "net/quic/core/quic_error_codes.h"
#include "net/quic/core/quic_packets.h"
#include "net/quic/core/quic_server_id.h"
#include "net/quic/platform/api/quic_socket_address.h"
#include "net/quic/platform/api/quic_str_cat.h"
#include "net/quic/platform/api/quic_string_piece.h"
#include "net/quic/platform/api/quic_text_utils.h"
#include "net/spdy/chromium/spdy_http_utils.h"
#include "net/spdy/core/spdy_header_block.h"
#include "url/gurl.h"
#include "tools/quic/quic_simple_client.h"
#include "net/base/address_list.h"
#include "tools/quic/synchronous_host_resolver.h"

using net::CertVerifier;
using net::CTPolicyEnforcer;
using net::CTVerifier;
using net::MultiLogCTVerifier;
using net::ProofVerifier;
using net::ProofVerifierChromium;
using net::QuicStringPiece;
using net::QuicTextUtils;
using net::SpdyHeaderBlock;
using net::TransportSecurityState;
using std::cout;
using std::cerr;
using std::endl;
using std::string;

#define TAG "NetUtils"

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))

#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))

#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))

# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))

#define NATIVE_METHOD(className, functionName, signature) \
    { #functionName, signature, reinterpret_cast<void*>(className ## _ ## functionName) }


static void NetUtils_nativeQuicClient(JNIEnv* env, jclass, jstring host, jint port, jstring path) {
    const char* param_host = env->GetStringUTFChars(host, NULL);
    int param_port = port;
    const char* param_path = env->GetStringUTFChars(path, NULL);

    LOGW("Host: %s\n", param_host);
    LOGW("Port: %d\n", param_port);
    LOGW("Path: %s\n", param_path);

//    logging::LoggingSettings settings;
//    settings.logging_dest = logging::LOG_TO_SYSTEM_DEBUG_LOG;
//    CHECK(logging::InitLogging(settings));


    int32_t FLAGS_initial_mtu = 0;
    bool FLAGS_version_mismatch_ok = false;

    base::AtExitManager exit_manager;
    base::MessageLoopForIO message_loop;
    net::QuicIpAddress ip_addr;

    GURL url(param_path);
    string s_host = string(param_host);
    net::AddressList addresses;
    LOGE("ok1 %s",s_host.c_str());
    int rv = net::SynchronousHostResolver::Resolve(s_host, &addresses);
    if (rv != net::OK) {
        LOGE("Unable to resolve %s",param_host);
        return;
    }
    LOGE("ok2");
    ip_addr =
            net::QuicIpAddress(net::QuicIpAddressImpl(addresses[0].address()));
    LOGE("ok3");
    string host_port = net::QuicStrCat(ip_addr.ToString(), ":", port);

    LOGE("host_port %s",host_port.c_str());

    net::QuicServerId server_id(url.host(), url.EffectiveIntPort(),
                                net::PRIVACY_MODE_DISABLED);

    std::unique_ptr<CertVerifier> cert_verifier(CertVerifier::CreateDefault());
    std::unique_ptr<TransportSecurityState> transport_security_state(
            new TransportSecurityState);
    std::unique_ptr<MultiLogCTVerifier> ct_verifier(new MultiLogCTVerifier());
    ct_verifier->AddLogs(net::ct::CreateLogVerifiersForKnownLogs());
    std::unique_ptr<CTPolicyEnforcer> ct_policy_enforcer(new CTPolicyEnforcer());


    std::unique_ptr<ProofVerifier> proof_verifier;
    proof_verifier.reset(new ProofVerifierChromium(
            cert_verifier.get(), ct_policy_enforcer.get(),
            transport_security_state.get(), ct_verifier.get()));

    net::ParsedQuicVersionVector versions = net::AllSupportedVersions();
    net::QuicSimpleClient client(net::QuicSocketAddress(ip_addr, param_port), server_id,
                                 versions, std::move(proof_verifier));
    client.set_initial_max_packet_length(
            FLAGS_initial_mtu != 0 ? FLAGS_initial_mtu : net::kDefaultMaxPacketSize);
    if (!client.Initialize()) {
        LOGE("Failed to initialize client.");
        return;
    }
    LOGE("initialized ");

    if (!client.Connect()) {
        LOGE("Failed to connect to 1 %s",host_port.c_str());

        net::QuicErrorCode error = client.session()->error();
        if (FLAGS_version_mismatch_ok && error == net::QUIC_INVALID_VERSION) {
            cout << "Server talks QUIC, but none of the versions supported by "
                 << "this client: " << ParsedQuicVersionVectorToString(versions)
                 << endl;
            // Version mismatch is not deemed a failure.
            return;
        }
        LOGE("Failed to connect to 2 %s",host_port.c_str());
        return;
    }
    LOGE("Connected to %s" , host_port.c_str());
    string body = "";
    SpdyHeaderBlock header_block;
    header_block[":method"] = body.empty() ? "GET" : "POST";
    header_block[":scheme"] = url.scheme();
    header_block[":authority"] = url.host();
    header_block[":path"] = url.path();

    // Make sure to store the response, for later output.
    client.set_store_response(true);

    // Send the request.
    client.SendRequestAndWaitForResponse(header_block, body, /*fin=*/true);

    string response_body = client.latest_response_body();
    if (!response_body.empty()) {
        LOGI("response_body = %s", response_body.c_str());
    }
}

void NativeInit() {
    if(!base::TaskScheduler::GetInstance()) {
        base::TaskScheduler::CreateAndStartWithDefaultParams("quic_client");
    }
}

int jniRegisterNativeMethods(JNIEnv* env, const char *classPathName, JNINativeMethod *nativeMethods, jint nMethods) {
    jclass clazz;
    clazz = env->FindClass(classPathName);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, nativeMethods, nMethods) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static JNINativeMethod gNetUtilsMethods[] = {
        NATIVE_METHOD(NetUtils, nativeQuicClient, "(Ljava/lang/String;ILjava/lang/String;)V"),
};
void register_com_example_zhangchi09_myapplication_NetUtils(JNIEnv* env) {
    jniRegisterNativeMethods(env, "com/example/zhangchi09/myapplication/NetUtils",
                             gNetUtilsMethods, NELEM(gNetUtilsMethods));
}
// DalvikVM calls this on startup, so we can statically register all our native methods.
jint JNI_OnLoad(JavaVM* vm, void*) {
    base::android::InitVM(vm);
    JNIEnv* env = base::android::AttachCurrentThread();

    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        LOGE("JavaVM::GetEnv() failed");
        abort();
    }
    NativeInit();
    register_com_example_zhangchi09_myapplication_NetUtils(env);

    base::android::InitReplacementClassLoader(env,
                                              base::android::GetClassLoader(env));
    return JNI_VERSION_1_6;
}