#define TaskChanging 
#define MainDebug

#ifdef TaskChanging
    #define TaskChangingPrint(X) Serial.println(X)
#else
    #define TaskChangingPrint(X)
#endif

#ifdef MainDebug
    #define MainDebugPrint(X) Serial.println(X)
#else
    #define MainDebugPrint(X)
#endif