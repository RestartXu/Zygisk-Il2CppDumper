#include <cstring>
#include <jni.h>
#include <pthread.h>
#include "hack.h"
#include "zygisk.hpp"
#include <stdio.h>


using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;
int main()
{
    printf("Ssssss");
    pthread_t ntid;
    pthread_create(&ntid, nullptr, hack_thread, nullptr);
return 0;
}
class MyModule : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        env_ = env;
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        if (!args || !args->nice_name) {
            LOGE("Skip unknown process");
            return;
        }
        enable_hack = isGame(env_, args->app_data_dir);
    }

    void postAppSpecialize(const AppSpecializeArgs *) override {
        if (enable_hack) {
            int ret;
            pthread_t ntid;
            if ((ret = pthread_create(&ntid, nullptr, hack_thread, nullptr))) {
                LOGE("can't create thread: %s\n", strerror(ret));
            }
        }
    }

private:
    JNIEnv *env_{};
};

REGISTER_ZYGISK_MODULE(MyModule)
