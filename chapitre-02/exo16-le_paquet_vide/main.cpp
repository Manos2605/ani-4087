#ifdef __ANDROID__

#include <android_native_app_glue.h>

void android_main(struct android_app* app)
{
    (void)app;
}

#else

#include <iostream>

int main()
{
    std::cout << "MaSalle" << std::endl;
    return 0;
}

#endif