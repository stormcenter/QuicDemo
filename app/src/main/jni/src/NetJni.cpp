//
// Created by zhangchi09 on 18-3-30.
//
#include <stdio.h>
#include "net/base/network_delegate_impl.h"
#include <jni.h>
#include <android/log.h>
#include "base/at_exit.h"
#include "base/json/json_writer.h"
#include "base/message_loop/message_loop.h"
#include "base/memory/ptr_util.h"
#include "base/run_loop.h"
#include "base/values.h"
#include "net/http/http_response_headers.h"
#include "net/proxy_resolution/proxy_config_service_fixed.h"
#include "net/proxy_resolution/proxy_config.h"
#include "net/url_request/url_fetcher.h"
#include "net/url_request/url_fetcher_delegate.h"
#include "net/url_request/url_request_context.h"
#include "net/url_request/url_request_context_builder.h"
#include "net/url_request/url_request_context_getter.h"
#include "net/url_request/url_request.h"
#define TAG "NetUtils"

#define LOGI(...) \
  ((void)__android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__))

#define LOGE(...) \
  ((void)__android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__))

#define LOGW(...) \
  ((void)__android_log_print(ANDROID_LOG_WARN, TAG, __VA_ARGS__))

# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
// Simply quits the current message loop when finished.  Used to make
// URLFetcher synchronous.
class QuitDelegate : public net::URLFetcherDelegate {
public:
    QuitDelegate() {}
    ~QuitDelegate() override {}
    // net::URLFetcherDelegate implementation.
    void OnURLFetchComplete(const net::URLFetcher* source) override {
        LOGE("OnURLFetchComplete");
        base::MessageLoop::current()->QuitWhenIdleClosure();
        int responseCode = source->GetResponseCode();
        const net::URLRequestStatus status = source->GetStatus();
        if (status.status() != net::URLRequestStatus::SUCCESS) {
            LOGW("Request failed with error code: %s", net::ErrorToString(status.error()).c_str());
            return;
        }
        const net::HttpResponseHeaders* const headers = source->GetResponseHeaders();
        if (!headers) {
            LOGW("Response does not have any headers");
            return;
        }
        size_t iter = 0;
        std::string header_name;
        std::string date_header;
        while (headers->EnumerateHeaderLines(&iter, &header_name, &date_header)) {
            LOGW("Got %s header: %s\n", header_name.c_str(), date_header.c_str());
        }
        std::string responseStr;
        if(!source->GetResponseAsString(&responseStr)) {
            LOGW("Get response as string failed!");
        }
        LOGI("Content len = %lld, response code = %d, response = %s",
             source->GetReceivedResponseContentLength(),
             source->GetResponseCode(),
             responseStr.c_str());
    }

    void OnURLFetchUploadProgress(const net::URLFetcher* source,
                                  int64_t current,
                                  int64_t total) override {
        LOGE("OnURLFetchUploadProgress");
    }
//private:
//    DISALLOW_COPY_AND_ASSIGN(QuitDelegate);
};

// Builds a URLRequestContext assuming there's only a single loop.
static std::unique_ptr<net::URLRequestContext> BuildURLRequestContext(net::NetLog *net_log) {
    net::URLRequestContextBuilder builder;
    builder.set_net_log(net_log);
//#if defined(OS_LINUX)
    // On Linux, use a fixed ProxyConfigService, since the default one
    // depends on glib.
    //
    // TODO(akalin): Remove this once http://crbug.com/146421 is fixed.
    builder.set_proxy_config_service(
            base::WrapUnique(new net::ProxyConfigServiceFixed(net::ProxyConfigWithAnnotation::CreateDirect())));
//#endif
    std::unique_ptr<net::URLRequestContext> context(builder.Build());
    context->set_net_log(net_log);
    return context;
}
static void nativeSendRequest(JNIEnv* env, jclass, jstring javaUrl) {
    const char* native_url = env->GetStringUTFChars(javaUrl, NULL);
    LOGW("Url: %s", native_url);
    base::AtExitManager exit_manager;
    LOGW("Url: %s", native_url);
    GURL url(native_url);
    if (!url.is_valid() || (url.scheme() != "http" && url.scheme() != "https")) {
        LOGW("Not valid url: %s", native_url);
        return;
    }
    LOGW("Url: %s", native_url);
    base::MessageLoopForIO main_loop;
    QuitDelegate delegate;
    std::unique_ptr<net::URLFetcher> fetcher =
            net::URLFetcher::Create(url, net::URLFetcher::GET, &delegate);
    net::NetLog *net_log = nullptr;
    std::unique_ptr<net::URLRequestContext> url_request_context(BuildURLRequestContext(net_log));
    fetcher->SetRequestContext(
            // Since there's only a single thread, there's no need to worry
            // about when the URLRequestContext gets created.
            // The URLFetcher will take a reference on the object, and hence
            // implicitly take ownership.
            new net::TrivialURLRequestContextGetter(url_request_context.get(),
                                                    main_loop.task_runner()));
    fetcher->Start();
    // |delegate| quits |main_loop| when the request is done.
    // main_loop.
    env->ReleaseStringUTFChars(javaUrl, native_url);
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
        { "nativeSendRequest", "(java/lang/String;)V", (void *) nativeSendRequest },
};
void register_com_example_zhangchi09_myapplication_NetUtils(JNIEnv* env) {
    jniRegisterNativeMethods(env, "com/example/zhangchi09/myapplication/NetUtils",
                             gNetUtilsMethods, NELEM(gNetUtilsMethods));
}
// DalvikVM calls this on startup, so we can statically register all our native methods.
jint JNI_OnLoad(JavaVM* vm, void*) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        LOGE("JavaVM::GetEnv() failed");
        abort();
    }
    register_com_example_zhangchi09_myapplication_NetUtils(env);
    return JNI_VERSION_1_6;
}